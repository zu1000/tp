/*
 * Define the message ids for all available messages 
 *
 */

#ifndef __tp_comm_protocol_type_h__
#define __tp_comm_protocol_type_h__

namespace tp {
namespace comm {
namespace protocol {
namespace internal {

    // the message type will be used for header::type_
    enum message_type
    {
        MT_AddService,
        MT_ChgService,
        MT_DelService,
        MT_Statistic,
        MT_Market,
        MT_Unknown
    };

}}}}//tp::comm::protocol::internal

#endif//__tp_comm_protocol_type_h__
