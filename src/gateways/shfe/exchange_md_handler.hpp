#ifndef __tp_gateways_shfe_exchange_md_handler_hpp__
#define __tp_gateways_shfe_exchange_md_handler_hpp__

#include <utils/simple_logger.hpp>

extern "C"
{
#include <ThostFtdcMdApi.h>
}

namespace tp {
namespace gateways {
namespace shfe {

    class session_configuration;
    class session_state_handler;
    class event_handler;

    class exchange_md_handler : public CThostFtdcMdSpi
    {
    public:
        exchange_md_handler(
                const session_configuration& configuration,
                session_state_handler& state_handler,
                event_handler& event_handler,
                CThostFtdcMdApi& api);

        // Override the method from CThostFtdcMdSpi
        void OnFrontConnected();

        void OnFrontDisconnected(int nReason);

        void OnHeartBeatWarning(int nTimeLapse);

        void OnRspUserLogin(
                CThostFtdcRspUserLoginField *pRspUserLogin,
                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspUserLogout(
                CThostFtdcUserLogoutField *pUserLogout,
                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspError(
                CThostFtdcRspInfoField *pRspInfo,
                int nRequestID, bool bIsLast);

        void OnRspSubMarketData(
                CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRspUnSubMarketData(
                CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        void OnRtnDepthMarketData(
                CThostFtdcDepthMarketDataField *pDepthMarketData);

    private:
        const session_configuration& configuration_;
        utils::simple_logger         logger_;
        session_state_handler&       state_handler_;
        event_handler&               event_handler_;
        CThostFtdcMdApi&             api_;
    };

}}} // tp::gateways::shfe

#endif//__tp_gateways_shfe_exchange_md_handler_hpp__
