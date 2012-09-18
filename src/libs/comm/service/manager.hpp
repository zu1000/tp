/*
 * service event monitor is in charge of two things:
 *      - monitor the service event on network
 *      - publish the service belongs to me
 */

#ifndef __tp_comm_service_manager_h__
#define __tp_comm_service_manager_h__

#include "service.hpp"
#include "event_listener.hpp"
#include "configuration.hpp"

#include <comm/io/handler.hpp>

#include <comm/protocol/internal/service.hpp>
#include <comm/protocol/internal/policy.hpp>
#include <comm/protocol/internal/message_handler.hpp>

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

namespace tp {
namespace comm {
namespace service {

    /*
     * Template Params should meet:
     *  - S (Serializer):
     *      :bool serialize(const service&, service_t&)
     *      :bool deserialize(service_ptr&, const service_t&)
     *
     * - D (Codec)
     *      : D(message_handler&)
     *      : decode(char*, size_t)
     *      : size_t encode(T, buffer);
     *
     * - C (Communicator):
     *
     *      : C(service&, receive_handler&, error_handler&, timer_handler&)
     *      :bool start()
     *      :bool stop()
     *      :void start_timer()
     *      :std::size_t send(
     *          io::const_buffer&, io::error_code&)
     *
     */

    template<typename D, typename S, typename C>
    class manager : public boost::noncopyable,
                    private protocol::internal::message_handler
    {
        typedef manager<D, S, C> this_type;
    public:
        manager(
                const service& service,
                event_listener& listener);

        manager(
                const configuration& config,
                event_listener& listener);

        ~manager()
        {
            communicator_.stop();
        }

        // Management functionalities
        bool publish(const service_ptr& service);
        bool republish(const service_ptr& service);
        bool unpublish(const service_id& id);
        void interested_service_id(const service_id& id);
        bool has_service(const service_id& id, service_ptr& ptr);

        void handle_configuration(const configuration& config);

        C& communicator()
        {
            return communicator_;
        }

    private:
        bool event_publisher_handler(
                const io::error_code& ec);

        void service_event_handler(
                const protocol::internal::service_t& s);

        bool error_handler(
                const io::error_code& ec);

    private:
        service_id_set            services_to_get_;
        service_id_to_ptr_map     services_;
        service_id_to_ptr_map     own_services_;
        event_listener&           listener_;
        D                         codec_;
        C                         communicator_;
        boost::mutex              mutex_;
        boost::array
            <unsigned char, io::buffer_size>  event_publisher_buffer_;
    };
    
    //=========================================================================

    /*
     *  The start of implementation
     */

    template<typename D, typename S, typename C>
    inline
    manager<D, S, C>::
    manager(
            const service& service,
            event_listener& listener)
        : listener_(listener)
        , codec_(*this)
        , communicator_(
                service,
                boost::bind(&D::decode, boost::ref(codec_), _1, _2),
                boost::bind(&manager::error_handler, this, _1),
                boost::bind(&manager::event_publisher_handler, this, _1))
    {
        set_service_handler(
                boost::bind(&this_type::service_event_handler, this, _1));
        communicator_.start();
    }

    template<typename D, typename S, typename C>
    inline
    manager<D, S, C>::manager(
            const configuration& config,
            event_listener& listener)
        : listener_(listener)
        , codec_(*this)
        , communicator_(
                config.get_service_manager_configuration(),
                boost::bind(&D::decode, boost::ref(codec_), _1, _2),
                boost::bind(&manager::error_handler, this, _1),
                boost::bind(&manager::event_publisher_handler, this, _1))
    {
        set_service_handler(
                boost::bind(&this_type::service_event_handler, this, _1));
        handle_configuration(config);
        communicator_.start();
    }

    template<typename D, typename S, typename C>
    inline bool
    manager<D, S, C>::publish(const service_ptr& service)
    {
        boost::mutex::scoped_lock l(mutex_);

        if (!service) return false;

        return own_services_.insert(
            service_id_to_ptr_map::value_type(service->id(), service)).second;
    }

    template<typename D, typename S, typename C>
    inline bool
    manager<D, S, C>::republish(const service_ptr& service)
    {
        boost::mutex::scoped_lock l(mutex_);

        if (!service) return false;

        service_id_to_ptr_map::iterator find =
            own_services_.find(service->id());

        if (find == own_services_.end()) return false;

        // TODO: republish the services
        return true;
    }

    template<typename D, typename S, typename C>
    inline bool
    manager<D, S, C>::unpublish(const service_id& id)
    {
        boost::mutex::scoped_lock l(mutex_);
        // TODO: send service remove notification
        return own_services_.erase(id) > 0;
    }

    template<typename D, typename S, typename C>
    inline void
    manager<D, S, C>::interested_service_id(const service_id& id)
    {
        service_ptr ptr;
        if (has_service(id, ptr))
        {
            listener_.service_ready(*ptr);
            return;
        }

        boost::mutex::scoped_lock l(mutex_);
        services_to_get_.insert(id);
    }

    template<typename D, typename S, typename C>
    inline bool
    manager<D, S, C>::has_service(
            const service_id& id, service_ptr& ptr)
    {
        boost::mutex::scoped_lock l(mutex_);
        service_id_to_ptr_map::iterator find = services_.find(id);

        if (find == services_.end())
            return false;

        // get the service obj
        ptr = find->second;

        return true;
    }

    template<typename D, typename S, typename C>
    inline void
    manager<D, S, C>::handle_configuration(const configuration& config)
    {
        const service_id_to_ptr_map& own_services = config.get_service_to_publish();
        const service_id_to_ptr_map& known_services = config.get_known_service();
        const service_id_set         set = config.get_service_to_subscribe();

        for (service_id_to_ptr_map::const_iterator it = own_services.begin();
             it != own_services.end(); ++it)
        {
            own_services_.insert(*it);
        }

        for (service_id_to_ptr_map::const_iterator it = known_services.begin();
             it != known_services.end(); ++it)
        {
            services_.insert(*it);
        }

        for (service_id_set::const_iterator it = set.begin();
             it != set.end(); ++it)
        {
            services_to_get_.insert(*it);
        }

        if (!services_.empty())
        {
            for (service_id_to_ptr_map::const_iterator it = services_.begin();
                 it != services_.end(); ++it)
                listener_.service_ready(*it->second);
        }
    }

    template<typename D, typename S, typename C>
    inline void
    manager<D, S, C>::service_event_handler(
            const protocol::internal::service_t& s)
    {
        service_ptr ptr;

        if (S::deserialize(ptr, s))
        {
            bool change = false;
            bool ready  = false;

            // using do...while to make it a real scope
            do
            {
                boost::mutex::scoped_lock l(mutex_);

                const service_id& id = ptr->id();

                if (own_services_.find(id) != own_services_.end())
                    continue;

                if (services_to_get_.erase(id) > 0)
                {
                    ready = true;
                    services_[ptr->id()] = ptr;
                }
                else
                {
                    service_id_to_ptr_map::iterator find = services_.find(id);
                    if (find != services_.end() &&
                        (*find->second != *ptr))
                    {
                        change = true;
                        find->second = ptr;
                    }
                }
            }while(0);

            if (change)
                listener_.service_changed(*ptr);

            if (ready)
                listener_.service_ready(*ptr);
        }
    }

    template<typename D, typename S, typename C>
    inline bool
    manager<D, S, C>::event_publisher_handler(
            const io::error_code& ec)
    {
        boost::mutex::scoped_lock l(mutex_);
        service_id_to_ptr_map::iterator it = own_services_.begin();

        boost::asio::mutable_buffer
            send_buffer(event_publisher_buffer_.data(), io::buffer_size);

        size_t send_size = 0;

        for(; it != own_services_.end(); ++it)
        {
            service& s = *it->second;
            protocol::internal::service_t s_t(protocol::internal::MT_AddService);

            if (S::serialize(s, s_t))
            {
                size_t encoded_size = codec_.encode(s_t, send_buffer);
                send_size   += encoded_size;
                send_buffer = send_buffer + encoded_size;
            }
        }

        if (!send_size) return true;

        io::error_code send_ec;
        size_t transferred_size =
            communicator_.send(
                io::const_buffer(
                    event_publisher_buffer_.data(), send_size), send_ec);

        communicator_.start_timer();

        return true;
    }

    template<typename D, typename S, typename C>
    inline bool
    manager<D, S, C>::error_handler(
            const io::error_code& ec)
    {
        return true;
    }

}}}//tp::comm::service

#endif//__tp_comm_service_manager_h__
