#include "xml_handler.hpp"

#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>
#include <stack>

namespace tp {
namespace xmllite {

    class xml_parser : public boost::noncopyable
    {
        enum xml_parsing_state
        {
            Start,
            ExpectTagEnd,
            ExpectTagName,
            ExpectTagAttrKey,
            ExpectTagAttrValue,
            ExpectString,
            ExpectData,
            Comment,
            ExpectCommentEnd,
            XmlProperty
        };
    public:
        xml_parser(xml_handler& handler);

        // \! throw std::runtime_error
        bool parsing(const std::string& filename);

        // \! throw std::runtime_error
        bool parsing(std::istream& is);

    private:
        bool parsing_line(const char* line_pos, std::size_t length);

    private:
        bool                            collecting_string_;
        std::string                     tag_;
        xml_attrs                       attrs_;
        xml_parsing_state               state_;
        std::stack<std::string>         key_stack_;
        std::stack<std::string>         data_stack_;
        std::string                     chars_;
        xml_handler&                    handler_;
    };

}}//tp::xmllite
