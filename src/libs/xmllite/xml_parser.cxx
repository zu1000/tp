#include "xml_parser.hpp"

#include <fstream>

namespace tp {
namespace xmllite {

    static const std::size_t max_buffer_size = 0xffffff;

    xml_parser::xml_parser(xml_handler& handler)
        : collecting_string_(false)
        , state_(Start)
        , handler_(handler)
    {
    }

    bool
    xml_parser::parsing(const std::string& filename)
    {
        std::ifstream input;
        input.open(filename.c_str());
        return parsing(input);
    }

    bool
    xml_parser::parsing(std::istream& is)
    {
        is.seekg(0, std::ios::end);
        std::size_t size = is.tellg();
        is.seekg(0, std::ios::beg);

        std::size_t size_to_alloc = size > max_buffer_size
                             ? max_buffer_size : size;

        // alloc one more byte...
        size_to_alloc++;

        boost::scoped_ptr<char> buffer_(new char[size_to_alloc]);

        try
        {
            while (is.good())
            {
                is.getline(buffer_.get(), size_to_alloc);
                if (!parsing_line(buffer_.get(), size_to_alloc))
                    return false;
            }

            return true;
        }
        catch(const std::ios_base::failure& e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    bool
    xml_parser::parsing_line(const char* line_pos, std::size_t length)
    {
        while(length && *line_pos)
        {
            switch(*line_pos)
            {
                case '<':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    switch (state_)
                    {
                        case Start:
                        case ExpectData:
                        {
                            state_ = ExpectTagName;
                            break;
                        }
                        case Comment:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                        {
                            return false;
                            break;
                        }
                    }
                    break;
                }
                case '>':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    switch(state_)
                    {
                        case ExpectCommentEnd:
                        {
                            handler_.comment(chars_);
                            chars_.clear();
                            state_ = Start;
                            break;
                        }
                        case ExpectTagEnd:
                        {
                            if (!chars_.empty())
                            {
                                tag_ = chars_;
                                chars_.clear();
                            }

                            if (data_stack_.empty())
                                return false;

                            handler_.end_element(tag_, data_stack_.top());
                            data_stack_.pop();
                            state_ = Start;
                            break;
                        }
                        case ExpectTagName:
                        {
                            tag_ = chars_;
                        }
                        case ExpectTagAttrValue:
                        {
                            if (!key_stack_.empty())
                            {
                                if (chars_.empty())
                                    return false;

                                attrs_[key_stack_.top()] = chars_;
                                key_stack_.pop();
                            }
                        }
                        case ExpectTagAttrKey:
                        {
                            handler_.start_element(tag_, attrs_);
                            tag_.clear();
                            chars_.clear();
                            attrs_.clear();
                            data_stack_.push("");
                            state_ = ExpectData;
                            break;
                        }
                        case XmlProperty:
                        {
                            state_ = Start;
                            tag_.clear();
                            chars_.clear();
                            break;
                        }
                        case Comment:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                        {
                            return false;
                            break;
                        }
                    }
                    break;
                }
                case '"':
                {
                    switch(state_)
                    {
                        case ExpectData:
                        {
                            data_stack_.top() += *line_pos;
                            break;
                        }
                        case ExpectTagAttrValue:
                        {
                            collecting_string_ = !collecting_string_;
                            if (collecting_string_)
                                break;
                            else
                            if (!chars_.empty())
                            {
                                attrs_[key_stack_.top()] = chars_;
                                key_stack_.pop();
                                chars_.clear();
                                state_ = ExpectTagAttrKey;
                            }
                            else
                                return false;
                            break;
                        }
                        case Comment:
                        case XmlProperty:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                        {
                            return false;
                            break;
                        }
                    }
                    break;
                }
                case '/':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    switch(state_)
                    {
                        case ExpectData:
                        {
                            data_stack_.top() += *line_pos;
                            break;
                        }
                        case ExpectTagAttrKey:
                        {
                            handler_.start_element(tag_, attrs_);
                            data_stack_.push("");
                            state_ = ExpectTagEnd;
                            break;
                        }
                        case ExpectTagName:
                        {
                            if (!chars_.empty())
                            {
                                tag_ = chars_;
                                handler_.start_element(tag_, attrs_);
                                chars_.clear();
                                data_stack_.push("");
                            }
                            state_ = ExpectTagEnd;
                            break;
                        }
                        case Comment:
                        case XmlProperty:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                        {
                            return false;
                            break;
                        }
                    }

                    break;
                }
                case '=':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    switch(state_)
                    {
                        case ExpectData:
                        {
                            data_stack_.top() += *line_pos;
                            break;
                        }
                        case ExpectTagAttrKey:
                        {
                            key_stack_.push(chars_);
                            chars_.clear();
                            state_ = ExpectTagAttrValue;
                            break;
                        }
                        case Comment:
                        case XmlProperty:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                        {
                            // All other possibility should be invalid, so stop
                            // and return false.
                            return false;
                            break;
                        }
                    }

                    break;
                }
                case ' ':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    switch(state_)
                    {
                        case ExpectTagName:
                        {
                            if (!chars_.empty())
                            {
                                tag_ = chars_;
                                chars_.clear();
                                state_ = ExpectTagAttrKey;
                            }
                            break;
                        }
                        case ExpectTagAttrValue:
                        {
                            if (!chars_.empty())
                            {
                                attrs_[key_stack_.top()] = chars_;
                                key_stack_.pop();
                                chars_.clear();
                                state_ = ExpectTagAttrKey;
                            }
                            break;
                        }
                        case Comment:
                        case XmlProperty:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case '!':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    if (state_ == ExpectTagName && chars_.empty())
                        state_ = Comment;
                    else
                    if (state_ == Comment)
                        state_ = ExpectCommentEnd;
                    else
                        chars_ += *line_pos;
                    //Otherwise, ignore this !

                    break;
                }
                case '?':
                {
                    if (collecting_string_)
                    {
                        chars_ += *line_pos;
                        break;
                    }

                    switch(state_)
                    {
                        case ExpectTagName:
                        {
                            if (chars_.empty())
                                state_ = XmlProperty;
                            else
                                return false;
                            break;
                        }
                        case ExpectData:
                        {
                            if (data_stack_.empty())
                                return false;
                            data_stack_.top() += *line_pos;
                            break;
                        }
                        case XmlProperty:
                        {
                            chars_.clear();
                            break;
                        }
                        case Comment:
                        {
                            chars_ += *line_pos;
                            break;
                        }
                        default:
                            return false;
                    }
                    break;
                }
                default:
                {
                    switch(state_)
                    {
                        case ExpectData:
                        case Start:
                        {
                            if (data_stack_.empty())
                                return false;

                            data_stack_.top() += *line_pos;
                            break;
                        }
                        case ExpectCommentEnd:
                        {
                            state_ = Comment;
                        }
                        default:
                        {
                            chars_ += *line_pos;
                        }
                    }
                    break;
                }
            }

            line_pos++;
            length--;
        }

        return true;
    }

}}//tp::xmllite
