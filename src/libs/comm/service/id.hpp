/*
 * This header file defines the type of service_id
 *
 * If the service_id is definde as a customized type and has no default "<"
 * operator, we need to define "<" operator here.
 */

#ifndef __tp_comm_service_id_h__
#define __tp_comm_service_id_h__

extern "C" {
#include <stdint.h>
}

namespace tp {
namespace comm {
namespace service {

    enum service_type
    {
        SrvId_Logger,
        SrvId_Statistic,
        // Put all known service before this tag
        SrvId_Unknown
    };

    typedef uint32_t service_id;

}}} //tp::comm::service

#endif//__tp_comm_service_id_h__
