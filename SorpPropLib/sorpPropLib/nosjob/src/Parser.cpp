#include <cassert>
#include <sstream>
#include <deque>
#if 0 && !defined(CERR)
#include <iostream> /* only for debuggering */
#define CERR std::cerr << __FILE__ << ":" << std::dec << __LINE__ << " : "
#endif

#include "wh/nosjob/nosjob.hpp"
#include "parser/JSON_parser.h"
namespace nosjob {

    void JsonParser::ParseError::init(JsonParser const &p,char const * msg)
    {
        m_line = p.line();
        m_column = p.column();
        m_offset = p.charOffset();
        m_err = p.errorCode();
        std::ostringstream os;
        os << "Parse error number #"<<p.errorCode()
           << " ("<<p.errorCodeString()<<")"
           << " at character offset "<<p.charOffset()
           << ", line "<<p.line()
           << ", column "<<p.column()
           <<'.';
        if( msg ) {
            os << ' ' << msg;
        }
        this->what(os.str());
    }
    JsonParser::ParseError::ParseError(JsonParser const &p)
    {
        init(p,0);
    }

    JsonParser::ParseError::ParseError(JsonParser const &p, char const * msg)
    {
        init(p,msg);
    }
    uint32_t JsonParser::ParseError::line() const
    {
        return m_line;
    }
    uint32_t JsonParser::ParseError::column() const
    {
        return m_column;
    }
    uint32_t JsonParser::ParseError::charOffset() const
    {
        return m_offset;
    }
    int JsonParser::ParseError::errorCode() const
    {
        return m_err;
    }
    class JsonParser::Impl
    {
    public:
        JSON_config config;
        Atom root;
        Atom node;
        JSON_parser p;
        typedef std::deque<Atom> StackType;
        StackType stack;
        StringType key;
        bool hasKey;
        /** JSON_parser error code (JSON_E_xxx). */
        int errNo;
        Impl()
            : config(),
              root(),
              node(),
              p(0),
              stack(),
              key(),
              hasKey(false),
              errNo(0)
        {
        }

        ~Impl()
        {
            destroyParser();
        }

        void clear()
        {
            //destroyParser();
            //JSON_parser_reset(p);
            JSON_parser ps = p;
            JSON_config conf = config;
            *this = Impl();
            config = conf;
            p = ps;
        }

        void destroyParser()
        {
            if( p )
            {
                delete_JSON_parser(p);
                p = 0;
            }
        }
        void createParser()
        {
            if( 0 == p )
            {
                p = new_JSON_parser(&config);
                if( ! p )
                {
                    throw AllocError(__FILE__,__LINE__);
                }
            }
            else
            {
                JSON_parser_reset(p);
            }
            errNo = 0;
        }
        void resetParser()
        {
            clear();
            createParser();
        }
    };
    void JsonParser::init( Config const & c )
    {
        m_impl->config.allow_comments = c.allowComments;
        m_impl->config.depth = c.maxDepth;
        m_impl->config.callback_ctx = this;
        m_impl->config.handle_floats_manually = 0;
        m_impl->config.callback = JsonParser::callback;
    }
    JsonParser::JsonParser(Config const &c)
        : m_impl(new Impl),
          m_line(1),
          m_column(0),
          m_offset(0)
    {
        init(c);
    }
    JsonParser::JsonParser()
        : m_impl(new Impl),
          m_line(1),
          m_column(0),
          m_offset(0)
    {
        init(Config());
    }
    JsonParser::~JsonParser()
    {
        delete m_impl;
    }
    bool JsonParser::parseChar(int ch)
    {
        const bool b = JSON_parser_char(m_impl->p,ch);
        if( ! b )
        {
            m_impl->errNo = JSON_parser_get_last_error(m_impl->p);
        }
        return b;

    }

    Atom JsonParser::root() const
    {
        return m_impl->root;
    }

    bool JsonParser::parseDone()
    {
        return JSON_parser_done(m_impl->p);
    }

    void JsonParser::pushValue( Atom const & a )
    {
        if( m_impl->hasKey )
        {
            this->setCurrentKey(a);
        }
        else if( Array::isArray(this->m_impl->node) )
        {
            Array::cast(this->m_impl->node).push( a );
        }
        else
        {
            m_impl->errNo = E_INTERNAL;
            throw ParseError(*this,"Tried to push value to Array when not in Array mode.");
        }
    }
    void JsonParser::setCurrentKey( Atom const & a )
    {
        if( m_impl->hasKey && Object::isObject(this->m_impl->node) )
        {
            Object obj = Object::cast(this->m_impl->node);
            obj.set( m_impl->key, a );
            m_impl->hasKey = false;
            m_impl->key = StaticStrings.Empty;
        }
        else
        {
            m_impl->errNo = E_INTERNAL;
            throw ParseError(*this,"Tried to set a value while not in an Object.");
        }
    }
    int JsonParser::callback( void * cx, int type, JSON_value const * value )
    {
        JsonParser * self = static_cast<JsonParser*>(cx);
        Impl * imp = self->m_impl;
        switch(type) {
          case JSON_T_ARRAY_BEGIN:
          case JSON_T_OBJECT_BEGIN: {
              Atom obj;
              if( JSON_T_ARRAY_BEGIN == type ) obj = Array();
              else obj = Object();
              if( ! imp->root.hasValue() ) {
                  imp->root = obj;
                  imp->node = obj;
                  imp->stack.push_back(obj);
              }
              else {
                  imp->stack.push_back(obj);
                  self->pushValue(obj);
                  imp->node = obj;
              }
              break;
          }
          case JSON_T_ARRAY_END:
          case JSON_T_OBJECT_END: {
              assert(!imp->hasKey);
              if( imp->stack.empty() )
              {
                  self->m_impl->errNo = JSON_E_UNBALANCED_COLLECTION;
                  throw ParseError(*self);
              }
              imp->stack.pop_back();
              imp->node = imp->stack.empty()
                  ? Atom::Undefined
                  : imp->stack.back();
              break;
          }
          case JSON_T_INTEGER: {
              self->pushValue( Integer(value->vu.integer_value ) );
              break;
          }
          case JSON_T_FLOAT: {
              self->pushValue( Double(value->vu.float_value ) );
              break;
          }
          case JSON_T_NULL: {
              self->pushValue( Atom::Null );
              break;
          }
          case JSON_T_TRUE: {
              self->pushValue( Boolean::True );
              break;
          }
          case JSON_T_FALSE: {
              self->pushValue( Boolean::False );
              break;
          }
          case JSON_T_KEY: {
              imp->hasKey = true;
              imp->key = StringType( value->vu.str.value, value->vu.str.length );
              break;
          }
          case JSON_T_STRING: {
              self->pushValue( StringType( value->vu.str.value, value->vu.str.length ) );
              break;
          }
          default:
              assert(0);
              return 0;
        }
        return 1;
    }

    void JsonParser::clearNodes()
    {
        m_impl->stack.clear();
        m_impl->key = StringType();
        m_impl->root = m_impl->node = Atom::Undefined;
    }

//     void JsonParser::clear()
//     {
//         // TODO: impl->root = Atom::Undefined
//         m_impl->clear();
//         m_line = 1;
//         m_column = 0;
//     }
    void JsonParser::reset()
    {
        this->clearNodes();
        m_impl->resetParser();
        m_line = 1;
        m_column = 0;
        m_offset = 0;
    }

    uint32_t JsonParser::line() const
    {
        return m_line;
    }
    uint32_t JsonParser::column() const
    {
        return m_column;
    }

    uint32_t JsonParser::charOffset() const
    {
        return m_offset;
    }

    int JsonParser::errorCode() const
    {
        switch( m_impl->errNo ) {
          case JSON_E_NONE: return E_NONE;
          case JSON_E_INVALID_CHAR: return E_INVALID_CHAR;
          case JSON_E_INVALID_KEYWORD: return E_INVALID_KEYWORD;
          case JSON_E_INVALID_ESCAPE_SEQUENCE: return E_INVALID_ESCAPE_SEQUENCE;
          case JSON_E_INVALID_UNICODE_SEQUENCE: return E_INVALID_UNICODE_SEQUENCE;
          case JSON_E_INVALID_NUMBER: return E_INVALID_NUMBER;
          case JSON_E_NESTING_DEPTH_REACHED: return E_NESTING_DEPTH_REACHED;
          case JSON_E_UNBALANCED_COLLECTION: return E_UNBALANCED_COLLECTION;
          case JSON_E_EXPECTED_KEY: return E_EXPECTED_KEY;
          case JSON_E_EXPECTED_COLON: return E_EXPECTED_COLON;
          case JSON_E_OUT_OF_MEMORY: return E_OUT_OF_MEMORY;
          default:
              return E_UNKNOWN;
        }
    }

    char const * JsonParser::errorCodeString(int code)
    {
        switch( code ) {
          case JSON_E_NONE: return "OK";
          case JSON_E_INVALID_CHAR: return "Invalid character";
          case JSON_E_INVALID_KEYWORD: return "Invalid keyword";
          case JSON_E_INVALID_ESCAPE_SEQUENCE: return "Invalid escape sequence";
          case JSON_E_INVALID_UNICODE_SEQUENCE: return "Invalid unicode sequence";
          case JSON_E_INVALID_NUMBER: return "Invalid number";
          case JSON_E_NESTING_DEPTH_REACHED: return "Nesting depth limit reached";
          case JSON_E_UNBALANCED_COLLECTION: return "Unbalanced collection";
          case JSON_E_EXPECTED_KEY: return "Expected key";
          case JSON_E_EXPECTED_COLON: return "Expected colon";
          case JSON_E_OUT_OF_MEMORY: return "Out of memory";
          default:
              return "Unknown";
        }
    }
    char const * JsonParser::errorCodeString() const
    {
        return errorCodeString(m_impl->errNo);
    }
    void JsonParser::toss()
    {
        this->clearNodes();
        if( 0 == m_impl->errNo ) m_impl->errNo = E_UNKNOWN;
        throw ParseError(*this);
    }

    Atom JsonParser::parse( std::istream & in )
    {
        typedef std::istream_iterator<char> Iter;
        in >> std::noskipws;
        return this->parse( Iter(in), Iter() );
    }

    Atom JsonParser::parse( Utf8String const & in )
    {
        return this->parse(in.value());
    }
    
    Atom JsonParser::parse( char const * in )
    {
        return parse( in, in ? (in+strlen(in)) : 0 );
    }

    Atom JsonParser::parse( std::string const & in )
    {
        return parse( in.begin(), in.end() );
    }

} /* ::nosjob */
