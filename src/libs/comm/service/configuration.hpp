#ifndef __tp_comm_service_configuration_hpp__
#define __tp_comm_service_configuration_hpp__

// local
#include "service.hpp"

// libs/xmllite
#include <xmllite/xml_handler.hpp>

// boost
#include <boost/smart_ptr.hpp>

// std
#include <string>
#include <istream>

namespace tp {
namespace comm {
namespace service {

    class configuration : public xmllite::xml_handler
    {
    public:
        // !\throw std::runtime_error
        configuration(std::istream& input_stream);
        configuration(const std::string& file_name);

        const service_id_to_ptr_map& get_service_to_publish() const;
        const service_id_to_ptr_map& get_known_service() const;
        const service_id_set& get_service_to_subscribe() const;
        const service& get_service_manager_configuration() const;

    private:
        // virtual functions from xmllite::xml_handler
        void comment(const std::string& comment);
        void start_element(
                const std::string& key, const xmllite::xml_attrs& attrs);
        void end_element(
                const std::string& key, const std::string& data);

    private:
        service_id_to_ptr_map   service_to_publish_;
        service_id_to_ptr_map   known_service_;
        service_id_set          service_to_subscribe_;
        service_ptr             service_manager_configuration_;
    };

    typedef boost::scoped_ptr<configuration> configuration_ptr;

}}} // tp::comm::service

#endif//__tp_comm_service_configuration_hpp__
