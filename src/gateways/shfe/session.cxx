#include "session.hpp"

#include "exchange_md_handler.hpp"
#include "session_configuration.hpp"
#include "utils.hpp"

extern "C"
{
#include <ThostFtdcMdApi.h>
#include <ThostFtdcUserApiDataType.h>
}

namespace tp {
namespace gateways {
namespace shfe {

    session::session(
            std::auto_ptr<session_configuration> configuration,
            session_state_handler& state_handler,
            event_handler& event_handler)
        : api_()
        , configuration_(configuration)
        , state_handler_(state_handler)
        , event_handler_(event_handler)
        , subscribed_()
        , md_handler_()
    {
    }

    bool session::start()
    {
        // Already started...
        if (api_) return false;

        api_ = CThostFtdcMdApi::CreateFtdcMdApi();
        if (!api_) return false;

        md_handler_.reset(
                new exchange_md_handler(
                    *configuration_, state_handler_, event_handler_, *api_ ));

        api_->RegisterSpi(md_handler_.get());

        // This is dangerous... but... the function doesn't change the string
        // content, it should be fine.
        api_->RegisterFront((char*)configuration_->front().c_str());

        api_->Init();

        return true;
    }

    bool session::stop()
    {
        // Not running
        if (!api_) return false;

        api_->RegisterSpi(NULL);
        api_->Join(); // Should we join???
        api_->Release();

        return true;
    }

    bool session::subscribe(const std::string& security)
    {
        if (!api_) return false;

        if (security.empty() || !subscribed_.insert(security).second)
            return false;

        char id[sizeof(TThostFtdcInstrumentIDType)];
        fill_field(id, security);

        char* ids[1] = { id };
        return api_->SubscribeMarketData(ids, 1) == 0;
    }

    bool session::unsubscribe(const std::string& security)
    {
        if (!api_) return false;

        if (security.empty() || !subscribed_.erase(security))
            return false;
        char id[sizeof(TThostFtdcInstrumentIDType)];
        fill_field(id, security);

        char* ids[1] = { id };
        return api_->UnSubscribeMarketData(ids, 1) == 0;
    }

    bool session::subscribe(const std::vector<std::string>& securities)
    {
        char ** ids = new char*[securities.size()];

        for (size_t i = 0; i < securities.size(); ++i)
        {
            ids[i] = const_cast<char*>(securities.at(i).c_str());
            subscribed_.insert(securities.at(i));
        }

        bool res = api_->SubscribeMarketData(ids, securities.size());

        delete [] ids;

        return res;
    }

}}}
