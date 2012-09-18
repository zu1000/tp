#include <comm/service/service.hpp>
#include <comm/service/manager.hpp>

#include <comm/codec/binary/codec.hpp>
#include <comm/protocol/internal/policy.hpp>

#include <comm/io/handler.hpp>

#include <boost/test/unit_test.hpp>

namespace tp {
namespace comm {
namespace service {
namespace unittest {
namespace testmanagerexp {

    class test_serializer
    {
    public:
        static size_t serialize(
                const service& s,
                protocol::internal::service_t& )
        {
            return sizeof(protocol::internal::service_t);
        }

        static size_t deserialize(
                service_ptr& s_ptr,
                const protocol::internal::service_t&)
        {
            s_ptr.reset(
                    new service(
                        get_service_id(),
                        service::TCP, "172.0.0.1", service_port_, false));
            return sizeof(protocol::internal::service_t);
        }

        static size_t get_service_id()
        {
            return add_mode_ ? ++service_id_ : service_id_;
        }

        static void set_add_mode(bool add)
        {
            add_mode_ = add;
        }

        static void set_service_port(uint16_t port)
        {
            service_port_ = port;
        }

        static size_t service_id_;

        static uint16_t service_port_;

        static bool add_mode_;

    };

    size_t
    test_serializer::service_id_ = 1;

    uint16_t
    test_serializer::service_port_ = 10086;

    bool
    test_serializer::add_mode_ = true;

    class test_communicator : public boost::noncopyable
    {
    public:
        test_communicator(
                const service& service,
                const io::receive_handler& r_handler,
                const io::error_handler&   e_handler,
                const io::timer_handler&   t_handler)
            : seconds_(1)
            , r_handler_(r_handler)
            , e_handler_(e_handler)
            , t_handler_(t_handler)
            , run_count_(0)
            , send_count_(0)
        {
        }

        bool start()
        {
            run_count_++;
            return true;
        }

        bool stop()
        {
            return true;
        }

        void start_timer()
        {
        }

        size_t send(const io::const_buffer& buf, io::error_code&)
        {
            send_count_++;
            return boost::asio::buffer_size(buf);
        }

        boost::posix_time::seconds seconds_;
        io::receive_handler r_handler_;
        io::error_handler   e_handler_;
        io::timer_handler   t_handler_;
        size_t run_count_;
        size_t send_count_;
    };

    class manager_fixture : public event_listener
    {
        typedef manager<
            codec::binary::codec<protocol::internal::policy>,
            test_serializer,test_communicator> SEM;
    public:
        manager_fixture()
            : service_(0, service::UDP, "1.1.1.1", 19292, true)
            , service_event_monitor_(service_, *this)
            , communicator_(service_event_monitor_.communicator())
            , service_ready_count_(0)
            , service_changed_count_(0)
            , service_deleted_count_(0)
            , service_t_(protocol::internal::MT_AddService)
        {
        }

        virtual void service_ready(const service&)
        {
            service_ready_count_++;
        }

        virtual void service_changed(const service&)
        {
            service_changed_count_++;
        }

        virtual void service_deleted(const service_id&)
        {
            service_deleted_count_++;
        }

        service_ptr create_service()
        {
            return service_ptr(
                    new service(999, service::TCP, "172.0.0.1", 8000, true));
        }

        service service_;
        SEM     service_event_monitor_;
        size_t  service_ready_count_;
        size_t  service_changed_count_;
        size_t  service_deleted_count_;
        char    dummy_buffer_[io::buffer_size];
        test_communicator& communicator_;
        protocol::internal::service_t service_t_;
    };

    BOOST_FIXTURE_TEST_CASE(test_all_normal_cases, manager_fixture)
    {
        test_serializer::set_add_mode(true);

        service_ptr s_ptr = create_service();

        BOOST_CHECK(service_event_monitor_.publish(s_ptr));

        // should return false, since the service is already there
        BOOST_CHECK(!service_event_monitor_.publish(s_ptr));

        BOOST_CHECK(service_event_monitor_.unpublish(s_ptr->id()));

        BOOST_CHECK(!service_event_monitor_.unpublish(s_ptr->id()));

        service_ptr s_ptr_res;

        BOOST_CHECK(service_event_monitor_.publish(s_ptr));

        // We have published one service and the service should be sent when
        // timeout happens, i.e. sent_count_ should be 1
        communicator_.t_handler_(boost::system::error_code());

        BOOST_CHECK(communicator_.send_count_ == 1u);

        BOOST_CHECK(!service_event_monitor_.has_service(s_ptr->id(), s_ptr_res));

    }

    BOOST_FIXTURE_TEST_CASE(test_error_cases, manager_fixture)
    {
        test_serializer::set_add_mode(true);

        service_ptr s_ptr = create_service();

        BOOST_CHECK(service_event_monitor_.publish(s_ptr));
        BOOST_CHECK(!service_event_monitor_.publish(s_ptr));

        BOOST_CHECK(service_event_monitor_.unpublish(s_ptr->id()));
        BOOST_CHECK(!service_event_monitor_.unpublish(s_ptr->id()));
    }

    BOOST_FIXTURE_TEST_CASE(publish_services, manager_fixture)
    {

        service_ptr s_ptr1(
                    new service(999, service::TCP, "172.0.0.1", 10086, true));
        service_ptr s_ptr2(
                    new service(998, service::TCP, "172.0.0.1", 10085, false));
        service_ptr s_ptr3(
                    new service(997, service::TCP, "172.0.0.1", 10086, true));

        service_event_monitor_.publish(s_ptr1);
        service_event_monitor_.publish(s_ptr2);
        service_event_monitor_.publish(s_ptr3);

        communicator_.t_handler_(io::error_code());

        BOOST_CHECK_EQUAL(communicator_.run_count_, 1u);
        BOOST_CHECK_EQUAL(communicator_.send_count_, 1u);

        service_ptr s_ptr4(
                    new service(996, service::TCP, "172.0.0.1", 10086, true));
        service_event_monitor_.publish(s_ptr4);

        communicator_.t_handler_(io::error_code());

        BOOST_CHECK_EQUAL(communicator_.send_count_, 2u);
    }

    BOOST_FIXTURE_TEST_CASE(get_one_service, manager_fixture)
    {
        // Emulate that we got a service from network... set add_mode to false,
        // so that serivce id is not increased.
        test_serializer::set_add_mode(false);
        size_t id = test_serializer::get_service_id();
        service_event_monitor_.interested_service_id(id);
        std::memcpy(dummy_buffer_, &service_t_, sizeof(service_t_));
        communicator_.r_handler_(
                io::const_buffer(dummy_buffer_, io::buffer_size), 44);

        service_ptr s_ptr_res;
        BOOST_CHECK(service_event_monitor_.has_service(id, s_ptr_res));
        BOOST_CHECK(service_ready_count_ == 1u);

        BOOST_CHECK_EQUAL(communicator_.run_count_, 1u);
        BOOST_CHECK_EQUAL(communicator_.send_count_, 0u);

        BOOST_CHECK_EQUAL(service_ready_count_, 1u);
    }

    BOOST_FIXTURE_TEST_CASE(get_multi_services, manager_fixture)
    {
        test_serializer::set_add_mode(false);
        size_t id = test_serializer::get_service_id();
        service_event_monitor_.interested_service_id(id+1);
        service_event_monitor_.interested_service_id(id+2);

        test_serializer::set_add_mode(true);
        for (int i = 0; i < 3; ++i)
        {
            std::memcpy(
                    dummy_buffer_+i*sizeof(service_t_),
                    &service_t_, sizeof(service_t_));
        }

        communicator_.r_handler_(
                io::const_buffer(dummy_buffer_, io::buffer_size),
                sizeof(protocol::internal::service_t)*3);

        BOOST_CHECK_EQUAL(communicator_.run_count_, 1u);
        BOOST_CHECK_EQUAL(communicator_.send_count_, 0u);

        BOOST_CHECK_EQUAL(service_ready_count_, 2u);

    }

    BOOST_FIXTURE_TEST_CASE(get_service_changed, manager_fixture)
    {
        test_serializer::set_add_mode(false);
        size_t id = test_serializer::get_service_id();
        service_event_monitor_.interested_service_id(id);

        communicator_.r_handler_(
                io::const_buffer(dummy_buffer_, io::buffer_size),
                sizeof(protocol::internal::service_t));

        BOOST_CHECK_EQUAL(communicator_.run_count_, 1u);
        BOOST_CHECK_EQUAL(communicator_.send_count_, 0u);

        BOOST_CHECK_EQUAL(service_ready_count_, 1u);

        for (int i = 0; i < 3; ++i)
        {
            std::memcpy(
                    dummy_buffer_+i*sizeof(service_t_),
                    &service_t_, sizeof(service_t_));
        }

        test_serializer::set_service_port(10099);
        communicator_.r_handler_(
                io::const_buffer(dummy_buffer_, io::buffer_size),
                sizeof(protocol::internal::service_t)*3);


        BOOST_CHECK_EQUAL(service_ready_count_, 1u);
        BOOST_CHECK_EQUAL(service_changed_count_, 1u);

        test_serializer::set_add_mode(true);
    }

    BOOST_FIXTURE_TEST_CASE(get_wrong_buffer_size, manager_fixture)
    {
        communicator_.r_handler_(
                io::const_buffer(dummy_buffer_, io::buffer_size), 0);

        BOOST_CHECK_EQUAL(communicator_.run_count_, 1u);
        BOOST_CHECK_EQUAL(communicator_.send_count_, 0u);

        BOOST_CHECK_EQUAL(service_ready_count_, 0u);

    }

}}}}}//tp::comm::service::unittest::testmanagerexp
