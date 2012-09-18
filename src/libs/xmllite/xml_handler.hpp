#ifndef __tp_comm_xmllite_xml_handler_hpp__
#define __tp_comm_xmllite_xml_handler_hpp__

#include <boost/unordered_map.hpp>

#include <string>
#include <vector>

namespace tp {
namespace xmllite {

    typedef boost::unordered_map<const std::string, std::string> xml_attrs;
    
    class xml_handler
    {
    public:
        virtual ~xml_handler() = 0;

        virtual void comment(
                const std::string& comment) = 0;

        virtual void start_element(
                const std::string& key,
                const xml_attrs& attrs ) = 0;

        virtual void end_element(
                const std::string& key,
                const std::string& data) = 0;
    };

}}//tp::xmllite

#endif//__tp_comm_xmllite_xml_handler_hpp__
