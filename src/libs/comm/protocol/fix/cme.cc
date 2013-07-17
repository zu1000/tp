#include "fix.h"
#include <iostream>

using namespace blade;

enum
{
    //Header
    BEG_STRING      = 8,
    BODY_LENGTH     = 9,
    MSG_TYPE        = 35,
    SENDER_CMP_ID   = 49,
    SENDER_SUB_ID   = 50,
    TARGET_CMP_ID   = 56,
    TARGET_SUB_ID   = 57,
    SEND_SEQ_NO     = 34,
    RECV_SEQ_NO     = 369,
    SENDING_TIME    = 21,
    SENDER_LOC_ID   = 142,
    APP_SYS_NAME    = 1603,
    APP_SYS_VERSION = 1604,
    APP_SYS_VENDOR  = 1605,

    //Login
    RAW_DATA_LENGTH = 95,
    RAW_DATA        = 96,
    ENCRYPT_METHOD  = 98,
    HEARTBEAT_INTV  = 108,
    RESET_SEQ_NO    = 141,

    // Heartbeat
    // TestRequest
    TEST_REQ_ID     = 112,

    // ResendRequest
    BEG_SEQ_NO      = 7,
    END_SEQ_NO      = 16,

    // SequenceReset
    NEW_SEQ_NO      = 36,

    // Logout
    TEXT            = 58,
    NEXT_EXP_SEQ_NO = 789,

    //NewOrder
    ACCOUNT         = 1,
    CLORDID         = 11,
    HANDLE_INST     = 21,
    ORDER_QTY       = 38,
    ORDER_TYPE      = 40,
    PRICE           = 44,
    SIDE            = 54,
    SYMBOL          = 55,
    TIME_IN_FORCE   = 59,
    TRANSACTION_TIME= 60,
    MANUAL_ORDER_IND= 1028,
    POSITION_EFFECT = 77,
    SECURITY_DESC   = 107,
    SECURITY_TYPE   = 167,
    CUSTOMER_OR_FIRM= 204,
    MAX_SHOW        = 210,
    CTI_CODE        = 9702,
    CORREL_CLORDID  = 9717,

    //CancelOrder
    //ReplaceOrder
    ORDID           = 17,
    ORIG_CLORDID    = 41,

    // MassQuote
    QUOTE_REQ_ID    = 131,
    QUOTE_ID        = 117,
    MM_ACCOUNT      = 9771,
    MM_PROT_RESET   = 9773,
    NO_QUOTE_SETS   = 296,
    QUOTE_SET_ID    = 302,
    UL_SEC_DESC     = 307,
    TOT_QUOTE_ENTRY = 304,
    NO_QUOTE_ENTRY  = 295,
    QUOTE_ENTRY_ID  = 299,
    BID_PX          = 132,
    BID_SIZE        = 134,
    OFFER_PX        = 133,
    OFFER_SIZE      = 133,
};

struct app_info
{
    app_info(
            const std::string& name,
            const std::string& version,
            const std::string& vendor)
        : name_(name)
        , version_(version)
        , vendor_(vendor)
    {}

    std::string name_;
    std::string version_;
    std::string vendor_;
};

struct session_info
{
    session_info(
        const std::string& sender_cmp_id,
        const std::string& sender_sub_id,
        const std::string& target_cmp_id,
        const std::string& target_sub_id,
        const std::string& sender_loc_id,
        const std::string& password)
            : sender_cmp_id_(sender_cmp_id)
            , sender_sub_id_(sender_sub_id)
            , target_cmp_id_(target_cmp_id)
            , target_sub_id_(target_cmp_id)
            , sender_loc_id_(sender_loc_id)
            , password_(password)
    {}

    std::string sender_cmp_id_;
    std::string sender_sub_id_;
    std::string target_cmp_id_;
    std::string target_sub_id_;
    std::string sender_loc_id_;
    std::string password_;
};

template <typename builder_type>
void create_header(
        builder_type& sb,
        const session_info& s_info,
        const app_info* a_info)
{
    sb.push_tag(BEG_STRING, 7);
    sb.push_tag(BODY_LENGTH, 9);
    sb.push_tag(MSG_TYPE, 1);
    sb.push_tag(SENDER_CMP_ID, s_info.sender_cmp_id_.size());
    sb.push_tag(SENDER_SUB_ID, s_info.sender_sub_id_.size());
    sb.push_tag(TARGET_CMP_ID, s_info.target_cmp_id_.size());
    sb.push_tag(TARGET_SUB_ID, s_info.target_sub_id_.size());
    sb.push_tag(SENDER_LOC_ID, s_info.sender_loc_id_.size());
    sb.push_tag(SEND_SEQ_NO, 9);
    sb.push_tag(RECV_SEQ_NO, 9);
    sb.push_tag(SENDING_TIME, 21);

    if (a_info)
    {
        sb.push_tag(APP_SYS_NAME, a_info->name_.size());
        sb.push_tag(APP_SYS_VERSION, a_info->version_.size());
        sb.push_tag(APP_SYS_VENDOR, a_info->vendor_.size());
    }
}

// SEQNO/SENDING_TIME is not set
template <typename builder_type>
void init_header(
        builder_type& sb,
        const std::string& type,
        const session_info& s_info,
        uint64_t body_length,
        const app_info* a_info)
{
    sb.set_value(BEG_STRING, "FIX 4.2", 7);
    sb.set_value(BODY_LENGTH, body_length);
    sb.set_value(MSG_TYPE, type);
    sb.set_value(SENDER_CMP_ID, s_info.sender_cmp_id_);
    sb.set_value(SENDER_SUB_ID, s_info.sender_sub_id_);
    sb.set_value(TARGET_CMP_ID, s_info.target_cmp_id_);
    sb.set_value(TARGET_SUB_ID, s_info.target_sub_id_);
    sb.set_value(SENDER_LOC_ID, s_info.sender_loc_id_);

    if (a_info)
    {
        sb.set_value(APP_SYS_NAME, a_info->name_);
        sb.set_value(APP_SYS_VERSION, a_info->version_);
        sb.set_value(APP_SYS_VENDOR, a_info->vendor_);
    }
}

template<typename builder_type>
struct base
{
    void set_seqno(uint64_t send_seqno, uint64_t recv_seqno)
    {
        sb_.set_value(SEND_SEQ_NO, send_seqno);
        sb_.set_value(RECV_SEQ_NO, recv_seqno);
    }

    void set_send_time(const char* time, size_t sz)
    {
        sb_.set_value(SENDING_TIME, time, sz);
    }

protected:
    builder_type sb_;
};

typedef struct base< fix::simple_builder<1024> > simple_builder_base;

class logon : public simple_builder_base
{
public:
    logon(const session_info& s_info,
          const app_info& a_info)
    {
        create_header(sb_, s_info, &a_info);

        sb_.push_tag(RAW_DATA_LENGTH, 2);
        sb_.push_tag(RAW_DATA, s_info.password_.size());
        sb_.push_tag(ENCRYPT_METHOD, 1);
        sb_.push_tag(HEARTBEAT_INTV, 3);
        sb_.push_tag(RESET_SEQ_NO, 1);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(RAW_DATA_LENGTH);
        init_header(sb_, "A", s_info, body_length, &a_info);

        sb_.set_value(RAW_DATA_LENGTH, uint64_t(s_info.password_.size()));
        sb_.set_value(RAW_DATA, s_info.password_);
        sb_.set_value(ENCRYPT_METHOD, "0", 1);
        sb_.set_value(HEARTBEAT_INTV, uint64_t(60));
        sb_.set_value(RESET_SEQ_NO, "N", 1);
    }

    template<typename S>
    bool send(S& s)
    {
        sb_.send(s);
    }
};

class test_request : public simple_builder_base
{
public:
    test_request(const session_info& s_info)
    {
        create_header(sb_, s_info, NULL);

        sb_.push_tag(TEST_REQ_ID, 20);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(TEST_REQ_ID);
        init_header(sb_, "1", s_info, body_length, NULL);
    }

    void set_test_req_id(const char* id, size_t sz)
    {
        sb_.set_value(TEST_REQ_ID, id, sz);
    }

    void set_test_req_id(uint64_t id)
    {
        sb_.set_value(TEST_REQ_ID, id);
    }
};

class heartbeat : public simple_builder_base
{
public:
    heartbeat(const session_info& s_info)
    {
        // Create Header
        create_header(sb_, s_info, NULL);

        sb_.finalize();

        // Set Init Value
        init_header(sb_, "0", s_info, 0, NULL);
    }
};

class resend_request : public simple_builder_base
{
public:
    resend_request(const session_info& s_info)
    {
        // Create Header
        create_header(sb_, s_info, NULL);

        sb_.push_tag(BEG_SEQ_NO, 9);
        sb_.push_tag(END_SEQ_NO, 9);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(BEG_SEQ_NO);
        init_header(sb_, "2", s_info, body_length, NULL);
    }

    void set_seqno_range(uint64_t beg, uint64_t end)
    {
        sb_.set_value(BEG_SEQ_NO, beg);
        sb_.set_value(END_SEQ_NO, end);
    }
};

class sequence_reset : public simple_builder_base
{
public:
    sequence_reset(const session_info& s_info)
    {
        create_header(sb_, s_info, NULL);
        sb_.push_tag(NEW_SEQ_NO, 9);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(NEW_SEQ_NO);
        init_header(sb_, "4", s_info, body_length, NULL);
    }
};

class logout : public simple_builder_base
{
    logout(const session_info& s_info)
    {
        create_header(sb_, s_info, NULL);
        sb_.push_tag(NEXT_EXP_SEQ_NO, 9);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(NEXT_EXP_SEQ_NO);
        init_header(sb_, "5", s_info, body_length, NULL);
    }
};

struct static_order_info : public simple_builder_base
{
public:
    static_order_info(
            const std::string& account,
            const std::string& symbol,
            const std::string& sec_desc,
            const std::string& sec_type,
            bool  has_max_show)
        : account_(account)
        , symbol_(symbol)
        , sec_desc_(sec_desc)
        , sec_type_(sec_type)
        , has_max_show_(has_max_show)
    {}

    std::string account_;
    std::string symbol_;
    std::string sec_desc_;
    std::string sec_type_;
    bool        has_max_show_;
};

class new_order : public simple_builder_base
{
public:
    new_order(const session_info& s_info,
              const static_order_info& s_o_info)
    {
        create_header(sb_, s_info, NULL);
        sb_.push_tag(ACCOUNT, s_o_info.account_.size());
        sb_.push_tag(CLORDID, 20);
        sb_.push_tag(HANDLE_INST, 1);
        sb_.push_tag(ORDER_QTY, 9);
        sb_.push_tag(ORDER_TYPE, 1);
        sb_.push_tag(PRICE, 20);
        sb_.push_tag(SIDE, 1);
        sb_.push_tag(SYMBOL, 6);
        sb_.push_tag(TIME_IN_FORCE, 1);
        sb_.push_tag(TRANSACTION_TIME, 21);
        sb_.push_tag(MANUAL_ORDER_IND, 1);
        if (s_o_info.sec_type_ == "OPT")
            sb_.push_tag(POSITION_EFFECT, 1);
        sb_.push_tag(SECURITY_DESC, s_o_info.sec_desc_.size());
        sb_.push_tag(SECURITY_TYPE, s_o_info.sec_type_.size());
        sb_.push_tag(CUSTOMER_OR_FIRM, 1);
        if (s_o_info.has_max_show_)
            sb_.push_tag(MAX_SHOW, 9);
        sb_.push_tag(CTI_CODE, 1);
        sb_.push_tag(CORREL_CLORDID, 20);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(ACCOUNT);
        init_header(sb_, "D", s_info, body_length, NULL);
        sb_.set_value(ACCOUNT, s_o_info.account_);
        sb_.set_value(HANDLE_INST, 1ULL);
        sb_.set_value(SYMBOL, s_o_info.symbol_);
        sb_.set_value(SECURITY_DESC, s_o_info.sec_desc_);
        sb_.set_value(SECURITY_TYPE, s_o_info.sec_type_);
        sb_.set_value(MANUAL_ORDER_IND, "N");
        sb_.set_value(CTI_CODE, 1ULL);
    }

    void set_clordid(const std::string& clordid)
    {
        sb_.set_value(CLORDID, clordid);
        //sb_.set_value(CORREL_CLORDID, clordid);
    }

    void set_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(CLORDID, clordid, sz);
        //sb_.set_value(CORREL_CLORDID, clordid, sz);
    }

    void set_clordid(uint64_t clordid)
    {
        sb_.set_value(CLORDID, clordid);
        //sb_.set_value(CORREL_CLORDID, clordid);
    }

    void set_price(double price, bool pad = false)
    {
        sb_.set_value<12,7>(PRICE, price, pad);
    }

    void set_qty(uint64_t qty, bool pad = false)
    {
        sb_.set_value(ORDER_QTY, qty, pad);
    }

    void set_side(const char side)
    {
        sb_.set_value(SIDE, &side, 1);
    }

    void set_position_effect(const char open_close)
    {
        sb_.set_value(POSITION_EFFECT, &open_close, 1);
    }

    void set_order_type(const char type)
    {
        sb_.set_value(ORDER_TYPE, &type, 1);
    }

    void set_time_in_force(const char tif)
    {
        sb_.set_value(TIME_IN_FORCE, &tif, 1);
    }

    void set_transaction_time(const char* trans_time, size_t sz)
    {
        sb_.set_value(TRANSACTION_TIME, trans_time, sz);
    }
};

class cancel_order : public simple_builder_base
{
public:
    cancel_order(const session_info& s_info,
              const static_order_info& s_o_info)
    {
        create_header(sb_, s_info, NULL);
        sb_.push_tag(ACCOUNT, s_o_info.account_.size());
        sb_.push_tag(CLORDID, 20);
        sb_.push_tag(ORDID, 17);
        sb_.push_tag(ORIG_CLORDID, 20);
        sb_.push_tag(SIDE, 1);
        sb_.push_tag(SYMBOL, 6);
        sb_.push_tag(TRANSACTION_TIME, 21);
        sb_.push_tag(MANUAL_ORDER_IND, 1);
        sb_.push_tag(SECURITY_DESC, s_o_info.sec_desc_.size());
        sb_.push_tag(SECURITY_TYPE, s_o_info.sec_type_.size());
        sb_.push_tag(CORREL_CLORDID, 20);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(ACCOUNT);
        init_header(sb_, "D", s_info, body_length, NULL);
        sb_.set_value(ACCOUNT, s_o_info.account_);
        sb_.set_value(SYMBOL, s_o_info.symbol_);
        sb_.set_value(SECURITY_DESC, s_o_info.sec_desc_);
        sb_.set_value(SECURITY_TYPE, s_o_info.sec_type_);
        sb_.set_value(MANUAL_ORDER_IND, "N");
    }

    void set_clordid(const std::string& clordid)
    {
        sb_.set_value(CLORDID, clordid);
    }

    void set_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(CLORDID, clordid, sz);
    }

    void set_clordid(uint64_t clordid)
    {
        sb_.set_value(CLORDID, clordid);
    }

    void set_correlate_clordid(const std::string& clordid)
    {
        sb_.set_value(CORREL_CLORDID, clordid);
    }

    void set_correlate_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(CORREL_CLORDID, clordid, sz);
    }

    void set_correlate_clordid(uint64_t clordid)
    {
        sb_.set_value(CORREL_CLORDID, clordid);
    }


    void set_orig_clordid(const std::string& clordid)
    {
        sb_.set_value(ORIG_CLORDID, clordid);
    }

    void set_orig_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(ORIG_CLORDID, clordid, sz);
    }

    void set_orig_clordid(uint64_t clordid)
    {
        sb_.set_value(ORIG_CLORDID, clordid);
    }

    void set_ordid(uint64_t ordid, bool pad = true)
    {
        sb_.set_value(ORDID, ordid, pad);
    }

    void set_side(const char side)
    {
        sb_.set_value(SIDE, &side, 1);
    }

    void set_transaction_time(const char* trans_time, size_t sz)
    {
        sb_.set_value(TRANSACTION_TIME, trans_time, sz);
    }

};

class replace_order : public simple_builder_base
{
public:
    replace_order(const session_info& s_info,
                const static_order_info& s_o_info)
    {
        create_header(sb_, s_info, NULL);
        sb_.push_tag(ACCOUNT, s_o_info.account_.size());
        sb_.push_tag(CLORDID, 20);
        sb_.push_tag(ORDID, 17);
        sb_.push_tag(HANDLE_INST, 1);
        sb_.push_tag(ORDER_QTY, 9);
        sb_.push_tag(ORDER_TYPE, 1);
        sb_.push_tag(ORIG_CLORDID, 20);
        sb_.push_tag(PRICE, 20);
        sb_.push_tag(SIDE, 1);
        sb_.push_tag(SYMBOL, 6);
        sb_.push_tag(TIME_IN_FORCE, 1);
        sb_.push_tag(MANUAL_ORDER_IND, 1);
        sb_.push_tag(TRANSACTION_TIME, 21);
        if (s_o_info.sec_type_ == "OPT")
            sb_.push_tag(POSITION_EFFECT, 1);
        sb_.push_tag(SECURITY_DESC, s_o_info.sec_desc_.size());
        sb_.push_tag(SECURITY_TYPE, s_o_info.sec_type_.size());
        sb_.push_tag(CUSTOMER_OR_FIRM, 1);
        if (s_o_info.has_max_show_)
            sb_.push_tag(MAX_SHOW, 9);
        sb_.push_tag(CTI_CODE, 1);
        sb_.push_tag(CORREL_CLORDID, 20);

        sb_.finalize();

        // Set Init Value
        size_t body_length = sb_.get_length_from(ACCOUNT);
        init_header(sb_, "D", s_info, body_length, NULL);
        sb_.set_value(ACCOUNT, s_o_info.account_);
        sb_.set_value(HANDLE_INST, 1ULL);
        sb_.set_value(SYMBOL, s_o_info.symbol_);
        sb_.set_value(SECURITY_DESC, s_o_info.sec_desc_);
        sb_.set_value(SECURITY_TYPE, s_o_info.sec_type_);
        sb_.set_value(MANUAL_ORDER_IND, "N");
        sb_.set_value(CTI_CODE, 1ULL);
    }

    void set_clordid(const std::string& clordid)
    {
        sb_.set_value(CLORDID, clordid);
    }

    void set_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(CLORDID, clordid, sz);
    }

    void set_clordid(uint64_t clordid)
    {
        sb_.set_value(CLORDID, clordid);
    }

    void set_correlate_clordid(const std::string& clordid)
    {
        sb_.set_value(CORREL_CLORDID, clordid);
    }

    void set_correlate_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(CORREL_CLORDID, clordid, sz);
    }

    void set_correlate_clordid(uint64_t clordid)
    {
        sb_.set_value(CORREL_CLORDID, clordid);
    }

    void set_orig_clordid(const std::string& clordid)
    {
        sb_.set_value(ORIG_CLORDID, clordid);
    }

    void set_orig_clordid(const char* clordid, size_t sz)
    {
        sb_.set_value(ORIG_CLORDID, clordid, sz);
    }

    void set_orig_clordid(uint64_t clordid)
    {
        sb_.set_value(ORIG_CLORDID, clordid);
    }

    void set_ordid(uint64_t ordid, bool pad = true)
    {
        sb_.set_value(ORDID, ordid, pad);
    }

    void set_price(double price, bool pad = false)
    {
        sb_.set_value<12,7>(PRICE, price, pad);
    }

    void set_qty(uint64_t qty, bool pad = false)
    {
        sb_.set_value(ORDER_QTY, qty, pad);
    }

    void set_side(const char side)
    {
        sb_.set_value(SIDE, &side, 1);
    }

    void set_position_effect(const char open_close)
    {
        sb_.set_value(POSITION_EFFECT, &open_close, 1);
    }

    void set_order_type(const char type)
    {
        sb_.set_value(ORDER_TYPE, &type, 1);
    }

    void set_time_in_force(const char tif)
    {
        sb_.set_value(TIME_IN_FORCE, &tif, 1);
    }

    void set_transaction_time(const char* trans_time, size_t sz)
    {
        sb_.set_value(TRANSACTION_TIME, trans_time, sz);
    }
};

struct quote_entry_info
{
    quote_entry_info(
            const std::string symbol,
            const std::string sec_desc,
            const std::string sec_type)
    {}

    std::string symbol_;
    std::string sec_desc_;
    std::string sec_type_;
};

class quote_entry
{
    quote_entry(const quote_entry_info& q_e_info)
    {
        sb_.push_tag(QUOTE_ENTRY_ID, 10);
        sb_.push_tag(SYMBOL, q_e_info.symbol_.size());
        sb_.push_tag(SECURITY_DESC, q_e_info.sec_desc_.size());
        sb_.push_tag(SECURITY_TYPE, q_e_info.sec_type_.size());
        sb_.push_tag(BID_PX, 20);
        sb_.push_tag(BID_SIZE, 9);
        sb_.push_tag(OFFER_PX, 20);
        sb_.push_tag(OFFER_SIZE, 9);

        sb_.finalize();

        sb_.set_value(SYMBOL, q_e_info.symbol_);
        sb_.set_value(SECURITY_DESC, q_e_info.sec_desc_);
        sb_.set_value(SECURITY_TYPE, q_e_info.sec_type_);
    }

    void set_bid_px(double px, bool pad=false)
    {
        sb_.set_value<12, 7>(BID_PX, px, pad);
    }

    void set_bid_size(uint64_t sz, bool pad=false)
    {
        sb_.set_value(BID_SIZE, sz, pad);
    }

    void set_offer_px(double px, bool pad=false)
    {
        sb_.set_value<12, 7>(OFFER_PX, px, pad);
    }

    void set_offer_size(uint64_t sz, bool pad=false)
    {
        sb_.set_value(OFFER_SIZE, sz, pad);
    }

private:
    fix::simple_builder<1024> sb_;
};

class quote_set
{
public:
    quote_set(const std::string& underlying_sec_desc)
    {
        sb_.push_tag(QUOTE_SET_ID, 3);
        if (!underlying_sec_desc.empty())
            sb_.push_tag(UL_SEC_DESC, underlying_sec_desc.size());
        sb_.push_tag(TOT_QUOTE_ENTRY, 3);
        sb_.push_tag(NO_QUOTE_ENTRY, 3);

        sb_.finalize();

        if (!underlying_sec_desc.empty())
            sb_.set_value(UL_SEC_DESC, underlying_sec_desc);
    }

    void set_quote_set_id(uint64_t id, bool pad = false)
    {
        sb_.set_value(QUOTE_SET_ID, id, pad);
    }

    void set_no_entries(uint64_t no, bool pad = false)
    {
        sb_.set_value(TOT_QUOTE_ENTRY, no, pad);
        sb_.set_value(NO_QUOTE_ENTRY, no, pad);
    }
private:
    fix::simple_builder<1024> sb_;
};

typedef struct base< fix::extensible_builder<4096> > extensible_builder_base;

class mass_quote : public extensible_builder_base
{
public:
    mass_quote(
            const session_info& s_info,
            const std::string& mm_account)
        : body_length_()
    {
        create_header(sb_, s_info, NULL);
        //sb_.push_tag(QUOTE_REQ_ID, 23);
        sb_.push_tag(QUOTE_ID, 10);
        sb_.push_tag(MM_ACCOUNT, mm_account.size());
        sb_.push_tag(MANUAL_ORDER_IND, 1);
        sb_.push_tag(CUSTOMER_OR_FIRM, 1);
        sb_.push_tag(CTI_CODE, 1);
        sb_.push_tag(MM_PROT_RESET, 1);

        sb_.finalize();

        // body length is not determined here yet... so...
        body_length_ = sb_.get_length_from(QUOTE_ID);
        init_header(sb_, "i", s_info, body_length_, NULL);

        sb_.set_value(MM_ACCOUNT, mm_account);
    }

    template<typename S>
    bool send(S& s)
    {
        sb_.send(s);
    }

private:
    size_t body_length_;
};

struct dummy_sender
{
    void send(char*, size_t sz)
    {}
};

int main(void)
{
    session_info s_info(
            "ABC", "SXDHEA", "CME", "G", "NLOPTNLCME", "helloworld");

    app_info a_info("CMEEML","1.0beta","OPTNL");

    dummy_sender s;

    logon lg(s_info, a_info);

    lg.set_seqno(1, 2);
    lg.set_send_time("20130628-21:29:16.000", 21);
    lg.send(s);

    mass_quote mq(s_info, "MM");

    mq.send(s);
}
