#ifndef __tp_gateways_shfe_utils_hpp__
#define __tp_gateways_shfe_utils_hpp__

#include <utils/simple_logger.hpp>

#include <application/options.hpp>

#include <string>
#include <cstring>

extern "C"
{
#include <ThostFtdcUserApiStruct.h>
}

namespace tp {
namespace gateways {
namespace shfe {

    template <size_t size>
    inline void fill_field(char(&field)[size], const std::string& str)
    {
        std::strncpy(field, str.c_str(), size);
    }

    static const uint64_t scale = 1e6;
    inline int64_t convert(double price)
    {
        return round(price * scale);
    }

    template<typename T>
    inline void get_mandatory_option(
            tp::application::options& options, const std::string& name, T& t)
    {
        if (!options.has(name) || !options.get(name, t))
            throw std::invalid_argument(
                    "Mandatory option " + name + " not found!");
    }

    template<typename T>
    inline void get_optional_option(
            tp::application::options& options, const std::string& name, T&t)
    {
        if (options.has(name))
            options.get(name, t);
    }

#define LOG(N) logger << #N  << ':' << msg.N<< '\n';

    inline utils::simple_logger::client_type& operator << (
            utils::simple_logger::client_type& logger,
            const CThostFtdcReqUserLoginField& msg)
    {
        logger << "CThostFtdcReqUserLoginField:\n";
        LOG(TradingDay)
        LOG(BrokerID)
        LOG(UserID)
        LOG(Password)
        LOG(UserProductInfo)
        LOG(InterfaceProductInfo)
        LOG(ProtocolInfo)
        LOG(MacAddress)
        LOG(OneTimePassword)
        LOG(ClientIPAddress)

        return logger;
    }

    inline utils::simple_logger::client_type& operator << (
            utils::simple_logger::client_type& logger,
            const CThostFtdcRspUserLoginField& msg)
    {
        logger << "CThostFtdcRspUserLoginField:\n";
        LOG(TradingDay)
        LOG(LoginTime)
        LOG(BrokerID)
        LOG(UserID)
        LOG(SystemName)
        LOG(FrontID)
        LOG(SessionID)
        LOG(MaxOrderRef)
        LOG(SHFETime)
        LOG(DCETime)
        LOG(CZCETime)
        LOG(FFEXTime)

        return logger;
    }

    inline utils::simple_logger::client_type& operator << (
            utils::simple_logger::client_type& logger,
            const CThostFtdcRspInfoField& msg)
    {
        logger << "CThostFtdcRspInfoField:\n";
        LOG(ErrorID)
        LOG(ErrorMsg)

        return logger;
    }

    inline utils::simple_logger::client_type& operator << (
            utils::simple_logger::client_type& logger,
            const CThostFtdcDepthMarketDataField& msg)
    {
        logger << "CThostFtdcDepthMarketDataField:\n";
        LOG(TradingDay)
        LOG(InstrumentID)
        LOG(ExchangeID)
        LOG(ExchangeInstID)
        LOG(LastPrice)
        LOG(PreSettlementPrice)
        LOG(PreClosePrice)
        LOG(PreOpenInterest)
        LOG(OpenPrice)
        LOG(HighestPrice)
        LOG(LowestPrice)
        LOG(Volume)
        LOG(Turnover)
        LOG(OpenInterest)
        LOG(ClosePrice)
        LOG(SettlementPrice)
        LOG(UpperLimitPrice)
        LOG(LowerLimitPrice)
        LOG(PreDelta)
        LOG(CurrDelta)
        LOG(UpdateTime)
        LOG(UpdateMillisec)
        LOG(BidPrice1)
        LOG(BidVolume1)
        LOG(AskPrice1)
        LOG(AskVolume1)
        LOG(BidPrice2)
        LOG(BidVolume2)
        LOG(AskPrice2)
        LOG(AskVolume2)
        LOG(BidPrice3)
        LOG(BidVolume3)
        LOG(AskPrice3)
        LOG(AskVolume3)
        LOG(BidPrice4)
        LOG(BidVolume4)
        LOG(AskPrice4)
        LOG(AskVolume4)
        LOG(BidPrice5)
        LOG(BidVolume5)
        LOG(AskPrice5)
        LOG(AskVolume5)
    }

#undef LOG

}}} // tp::gateways::shfe

#endif//__tp_gateways_shfe_utils_hpp__
