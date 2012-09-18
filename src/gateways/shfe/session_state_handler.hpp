#ifndef __tp_gateways_shfe_session_state_handler_hpp__
#define __tp_gateways_shfe_session_state_handler_hpp__

#include <string>

namespace tp {
namespace gateways {
namespace shfe {

    class session_state_handler
    {
    public:
        virtual void started() = 0;
        virtual void stopped() = 0;
        virtual void stopped_on_error(const std::string& error) = 0;
    };

}}}

#endif//__tp_gateways_shfe_session_state_handler_hpp__
