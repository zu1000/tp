#ifndef __tp_gateways_shfe_session_hpp__
#define __tp_gateways_shfe_session_hpp__

#include <boost/scoped_ptr.hpp>

#include <set>

class CThostFtdcMdApi;

namespace tp {
namespace gateways {
namespace shfe {

    class session_configuration;
    class exchange_md_handler;
    class session_state_handler;
    class event_handler;

    class session
    {
    public:
        session(std::auto_ptr<session_configuration> configuration,
                session_state_handler& state_handler,
                event_handler& event_handler);

        bool start();
        bool stop();

        bool subscribe(const std::string& security);
        bool unsubscribe(const std::string& security);

    private:
        CThostFtdcMdApi*                       api_;
        std::auto_ptr<session_configuration>   configuration_;
        session_state_handler&                 state_handler_;
        event_handler&                         event_handler_;
        std::set<std::string>                  subscribed_;
        boost::scoped_ptr<exchange_md_handler> md_handler_;
    };

}}} // namespace tp::gateways::shfe

#endif//__tp_gateways_shfe_session_hpp__
