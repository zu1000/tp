#include "database.hpp"

#include <comm/protocol/internal/statistic.hpp>
#include <database/sqlite3_client.hpp>
#include <utils/number_to_string.hpp>

#include <boost/unordered_set.hpp>
#include <boost/bind.hpp>

using namespace tp::comm::protocol::internal;

namespace tp {
namespace database {
namespace ticker {

    struct tick_t
    {
        tick_t(
                uint64_t security_id=0,
                int64_t  last=int64_min,
                uint64_t last_quantity=0,
                uint64_t timestamp=0)
            : security_id_(security_id)
            , last_(last)
            , last_quantity_(last_quantity)
            , timestamp_(timestamp)
        {}

        uint64_t security_id_;
        int64_t  last_;
        uint64_t last_quantity_;
        uint64_t timestamp_;
    };

    struct tick_list_t
    {
        tick_list_t()
            : start_(0u)
            , end_(1u)
            , ticks_()
            , in_database_(false)
        {}

        bool push(const tick_t& tick)
        {
            if ((end_+1)%1000 == start_)
                return false;

            ticks_[end_] = tick;
            end_ = (end_+1)%1000;
            return true;
        }

        bool pop(tick_t& tick)
        {
            if ((start_+1)%1000 == end_)
                return false;

            start_ = (start_+1)%1000;
            tick = ticks_[start_];
            return true;
        }

        uint32_t                   start_;
        uint32_t                   end_;
        boost::array<tick_t, 1000> ticks_;
        bool                       in_database_;
    };

    static boost::scoped_ptr<boost::thread> update_thread;
    static boost::unordered_set<uint64_t>   tables;
    static tick_list_t tick_list;
    static bool stop_update_thread = false;
    static sql_table table("");

    const char* last_column_name = "last";
    const char* last_quantity_column_name = "quantity";
    const char* last_time_column_name = "time";

    static inline void
    prepare_tick_table()
    {
        table.create(true);
        table.set_column(last_column_name, false);
        table.set_column(last_quantity_column_name);
        table.set_column(last_time_column_name);
    }

    static bool
    check_update_tick_table(const tick_t& tick, client& client)
    {
        char num[30] = {0};
        utils::uint64_to_string(tick.security_id_, num);
        if( tables.find(tick.security_id_) == tables.end())
        {
            table.set_name(num);
            if (client.table(table))
                tables.insert(tick.security_id_);
            else
                return false; // stop and skip
        }

        sql_insert insert(num);
        utils::int64_to_string(tick.last_, num);
        insert.set_column_value(column_value(last_column_name, num));
        utils::uint64_to_string(tick.last_quantity_, num);
        insert.set_column_value(column_value(last_quantity_column_name, num));
        utils::uint64_to_string(tick.timestamp_, num);
        insert.set_column_value(column_value(last_time_column_name, num));

        return client.insert(insert);
    }

    database::database(tp::utils::simple_logger& logger)
        : logger_(logger)
    {
        stop_update_thread = false;
        prepare_tick_table();
        tables.clear();
    }

    database::~database()
    {
        stop();
    }

    bool
    database::open(const std::string name)
    {
        try
        {
            stop();
            client_.reset(new sqlite3_client(name));
            update_thread.reset(
                    new boost::thread(
                        boost::bind(&database::batch_updates, this)));

        }
        catch(std::exception& e)
        {
            return false;
        }
        return true;
    }

    bool
    database::update_tick(const statistic_t& statistic)
    {
        return tick_list.push(
                tick_t(
                    statistic.security_id_,
                    statistic.last_,
                    statistic.last_quantity_,
                    statistic.utc_timestamp_));
    }

    void
    database::batch_updates()
    {
        tick_t tick;
        do
        {
            usleep(1000);
            client_->begin_transaction();
            while(tick_list.pop(tick))
                check_update_tick_table(tick, *client_);
            client_->end_transaction();
        }while(!stop_update_thread);

        client_->begin_transaction();
        while(tick_list.pop(tick))
            check_update_tick_table(tick, *client_);
        client_->end_transaction();
    }

    void
    database::stop()
    {
        if (update_thread)
        {
            stop_update_thread = true;
            if (update_thread->joinable())
                update_thread->join();
            stop_update_thread = false;
        }
    }

}}}
