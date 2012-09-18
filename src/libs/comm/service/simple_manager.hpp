#ifndef __tp_comm_service_simple_manager_hpp__
#define __tp_comm_service_simple_manager_hpp__

#include "serializer.hpp"
#include "communicator.hpp"
#include "manager.hpp"

#include <comm/codec/binary/codec.hpp>
#include <comm/codec/util/byte_order_policy.hpp>

#include <comm/protocol/internal/policy.hpp>

#include <boost/smart_ptr.hpp>

namespace tp {
namespace comm {
namespace service {

    typedef serializer<codec::util::byte_order_policy> serializer_type;
    typedef codec::binary::codec<protocol::internal::policy> codec_type;

    typedef manager<codec_type, serializer_type, communicator> simple_manager;
    typedef boost::scoped_ptr<simple_manager>        simple_manager_ptr;

}}}//tp::comm::service

#endif//__tp_comm_service_simple_manager_hpp__
