#include "exchange_md_handler.hpp"
#include "session_configuration.hpp"
#include "session_state_handler.hpp"
#include "event_handler.hpp"
#include "utils.hpp"

#include <comm/protocol/internal/statistic.hpp>

#include <iostream>

#include <boost/lexical_cast.hpp>

extern "C"
{
#include <ThostFtdcMdApi.h>
}

namespace tp {
namespace gateways {
namespace shfe {

    exchange_md_handler::exchange_md_handler(
            const session_configuration& configuration,
            session_state_handler& state_handler,
            event_handler& event_handler,
            CThostFtdcMdApi& api)
        : configuration_(configuration)
        , logger_(std::cout)
        , state_handler_(state_handler)
        , event_handler_(event_handler)
        , api_(api)
    {}

    void exchange_md_handler::OnFrontConnected()
    {
        CThostFtdcReqUserLoginField login;
        fill_field(
                login.TradingDay,
                configuration_.trading_day());
        fill_field(
                login.BrokerID,
                configuration_.broker_id());
        fill_field(
                login.UserID,
                configuration_.user_id());
        fill_field(
                login.Password,
                configuration_.password());
        fill_field(
                login.UserProductInfo,
                configuration_.user_product_info());
        fill_field(
                login.InterfaceProductInfo,
                configuration_.interface_product_info());
        fill_field(
                login.ProtocolInfo,
                configuration_.protocol_info());
        fill_field(
                login.MacAddress,
                configuration_.mac_address());
        fill_field(
                login.OneTimePassword,
                configuration_.one_time_password());
        fill_field(
                login.ClientIPAddress,
                configuration_.client_ip_address());

        logger_ << "Sending UserLogonReq:\n" << login;

        switch(abs(api_.ReqUserLogin(&login, -1)))
        {
        default:
        case 0:
            // Do nothing, request is passed successfully
            break;
        case 1:
            state_handler_.stopped_on_error("network error");
            break;
        case 2:
            state_handler_.stopped_on_error("too many pending requests");
            break;
        case 3:
            state_handler_.stopped_on_error("too many requests per second");
            break;
        }
    }

    void exchange_md_handler::OnFrontDisconnected(int nReason)
    {
        logger_ << "Get Disconnected\n";
        switch(nReason)
        {
        default:
        case 0:
            state_handler_.stopped();
            break;
        case 0x1001:
        case 0x1002:
        case 0x2001:
        case 0x2002:
        case 0x2003:
            // TODO: check if we need to do anything here
            state_handler_.stopped_on_error(
                    boost::lexical_cast<std::string>(nReason));
            break;
        }
    }

    void exchange_md_handler::OnHeartBeatWarning(int nTimeLapse)
    {
        logger_ << "heartbeat warning [" << nTimeLapse << "]\n";
    }

    void exchange_md_handler::OnRspUserLogin(
            CThostFtdcRspUserLoginField *pRspUserLogin,
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        logger_ << "Got UserLogonRsp...\n" << *pRspUserLogin;

        if (pRspInfo && pRspInfo->ErrorID)
            state_handler_.stopped_on_error(pRspInfo->ErrorMsg);
        else
            state_handler_.started();
    }

    void exchange_md_handler::OnRspUserLogout(
            CThostFtdcUserLogoutField *pUserLogout,
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        // UserLogout...
    }

    void exchange_md_handler::OnRspError(
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
    }

    void exchange_md_handler::OnRspSubMarketData(
            CThostFtdcSpecificInstrumentField *pSpecificInstrument,
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        logger_ << "subscribed: " << pSpecificInstrument->InstrumentID << "\n";
    }

    void exchange_md_handler::OnRspUnSubMarketData(
            CThostFtdcSpecificInstrumentField *pSpecificInstrument,
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        logger_ << "unsubscribed: " << pSpecificInstrument->InstrumentID << "\n";
    }

    void exchange_md_handler::OnRtnDepthMarketData(
            CThostFtdcDepthMarketDataField *pDepthMarketData)
    {
        logger_ << "Got MarketDepthData\n";
        comm::protocol::internal::statistic_t statistic(~0u);

        statistic.scale_ = scale;

        statistic.high_         = convert(pDepthMarketData->HighestPrice);
        statistic.low_          = convert(pDepthMarketData->LowestPrice);
        statistic.open_         = convert(pDepthMarketData->OpenPrice);
        statistic.close_        = convert(pDepthMarketData->ClosePrice);
        statistic.settle_       = convert(pDepthMarketData->SettlementPrice);
        statistic.last_         = convert(pDepthMarketData->LastPrice);
        statistic.last_quantity_= pDepthMarketData->Volume;
        // Set utc_timestamp...
        event_handler_.handle(statistic, pDepthMarketData->InstrumentID);
    }

}}} // tp::gateways::shfe
