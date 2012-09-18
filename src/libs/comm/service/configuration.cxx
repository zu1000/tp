#include "configuration.hpp"

#include <xmllite/xml_parser.hpp>

using namespace tp::xmllite;

namespace tp {
namespace comm {
namespace service {

    static inline bool check_id(
            xml_attrs::const_iterator& id,
            const xml_attrs& attrs)
    {
        id   = attrs.find("id");
        return id != attrs.end() &&
                std::strtoul(id->second.c_str(), NULL, 10);
    }

    static inline bool check_type(
            xml_attrs::const_iterator& type,
            const xml_attrs& attrs)
    {
        type = attrs.find("type");
        return type != attrs.end() &&
                (type->second == "TCP" || type->second == "Multi");
    }

    static inline bool check_addr(
            xml_attrs::const_iterator& addr,
            const xml_attrs& attrs)
    {
        addr = attrs.find("addr");
        return addr != attrs.end();
    }

    static inline bool check_port(
            xml_attrs::const_iterator& port,
            const xml_attrs& attrs)
    {
        port = attrs.find("port");
        return port != attrs.end() &&
                std::strtoul(port->second.c_str(), NULL, 10);
    }

    static inline bool check_service(
            xml_attrs::const_iterator& id,
            xml_attrs::const_iterator& type,
            xml_attrs::const_iterator& addr,
            xml_attrs::const_iterator& port,
            const xml_attrs& attrs)
    {
        return check_id(id, attrs) &&
               check_type(type, attrs) &&
               check_addr(addr, attrs) &&
               check_port(port, attrs);
    }

    configuration::configuration(std::istream& input_stream)
    {
        xmllite::xml_parser parser(*this);
        if (!parser.parsing(input_stream))
            throw std::runtime_error("failed to parsing service config");

        if (!service_manager_configuration_)
            throw std::runtime_error("failed to get service manager config");
    }

    configuration::configuration(const std::string& file_name)
    {
        xmllite::xml_parser parser(*this);
        if (!parser.parsing(file_name))
            throw std::runtime_error("failed to parsing service config");

        if (!service_manager_configuration_)
            throw std::runtime_error("failed to get service manager config");
    }

    const service_id_to_ptr_map&
    configuration::get_service_to_publish() const
    {
        return service_to_publish_;
    }

    const service_id_to_ptr_map&
    configuration::get_known_service() const
    {
        return known_service_;
    }

    const service_id_set&
    configuration::get_service_to_subscribe() const
    {
        return service_to_subscribe_;
    }

    const service&
    configuration::get_service_manager_configuration() const
    {
        return *service_manager_configuration_;
    }

    void
    configuration::comment(const std::string& comment)
    {
        // don't care about the comment
    }

    void
    configuration::start_element(
            const std::string& key, const xmllite::xml_attrs& attrs)
    {
        // only care about the "service" tag
        if (key == "service")
        {
            xml_attrs::const_iterator owner = attrs.find("owner");

            if (owner  == attrs.end()) return;

            // To subscribe
            if (owner->second == "y" || owner->second == "m")
            {
                xml_attrs::const_iterator id, type, addr, port;

                if (!check_service(id, type, addr, port, attrs))
                    return;

                service_ptr s_ptr(
                        new service(
                            std::strtoul(id->second.c_str(), NULL, 10),
                            type->second == "TCP" ? service::TCP
                                                 : service::Multicast,
                            addr->second,
                            std::strtoul(port->second.c_str(), NULL, 10),
                            true));

                if (owner->second == "y")
                    service_to_publish_[s_ptr->id()] = s_ptr;
                else
                    service_manager_configuration_ = s_ptr;
                            
            }
            else
            if (owner->second == "n")
            {
                xml_attrs::const_iterator id, type, addr, port;

                if (!check_id(id, attrs))
                    return;

                if (check_service(id, type, addr, port, attrs))
                {
                    service_ptr s_ptr(
                            new service(
                                std::strtoul(id->second.c_str(), NULL, 10),
                                type->second == "TCP" ? service::TCP
                                                      : service::Multicast,
                                addr->second,
                                std::strtoul(port->second.c_str(), NULL, 10),
                                true));
                    known_service_[s_ptr->id()] = s_ptr;
                }
                else
                    service_to_subscribe_.insert(atoi(id->second.c_str()));
            }
        }
    }

    void
    configuration::end_element(
            const std::string& key, const std::string& data)
    {
        // no data... so don't do anything
    }

}}} // tp::comm::service
