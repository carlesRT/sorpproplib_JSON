#include "wh/nosjob/nosjob.hpp"
#include "wh/nosjob/Allocator.hpp"
#include "wh/nosjob/AtomAccess.hpp"
#include <sstream>
#include <cstring> // strcmp()
#if 1 && !defined(CERR)
#include <iostream> /* only for debuggering */
#define CERR std::cerr << __FILE__ << ":" << std::dec << __LINE__ << " : "
#endif
namespace nosjob {

    int Utf16String::strcmp( Utf16String const & lhs, Utf16String const & rhs )
    {
        Utf16String::ValueType const & lv = lhs.value();
        Utf16String::ValueType const & rv = rhs.value();
        size_t const lsz = lv.size();
        size_t const rsz = rv.size();
        if( lsz < rsz ) return -1;
        else if( lsz > rsz ) return 1;
        Utf16String::CharType const * lc = lv.c_str();
        Utf16String::CharType const * rc = rv.c_str();
        for( ; *lc && *rc; ++lc, ++rc )
        {
            if( *lc < *rc ) return -1;
            else if( *lc > *rc ) return 1;
        }
        if( *lc < *rc ) return -1;
        else if( *lc > *rc ) return 1;
        else return 0;
    }

    //! AtomAPI::Compare impl for Utf16String
    static int Compare_Utf16String(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType( lhs, TypeID::Utf16String );
        TypeID::Types const rt = rhs.typeID();
        switch( rt )
        {
          case TypeID::Utf8String:
              return Utf16String::strcmp( Utf16String::cast(lhs), Utf16String(Utf8String::cast(rhs)) );
          case TypeID::Utf16String:
              return Utf16String::strcmp( Utf16String::cast(lhs), Utf16String::cast(rhs) );
          default:
              return (Utf16String::TypeID < rt) ? -1 : 1;
        };
    }

    typedef
        /* Reminder: We don't have access to Utf8String::Impl from
           here, so we duplicate its type.*/
        Detail::RcValue<Utf16String::ValueType>
        StringImplType;
    static StringImplType & EmptyImpl();

    static const RcValuePagedAllocator<StringImplType> stringImplAllocator(128,-1);

    class Utf16String::APIImpl : public AtomAPI
    {
    public:
        typedef Utf16String::ValueType ValueType;
    private:
        typedef Utf16String::Impl Impl;
        /**
           Implements AtomAPI::ToBool() for Utf16String.

           Returns true only if a is-a string and has a non-empty value.
        */
        static bool apiToBool( Atom const & a )
        {
            if( Utf16String::AtomTypeID != a.typeID() ) return false;
            else
            {
                Detail::AtomAccess<false> AA(a);
                Impl const * v = static_cast<Impl const *>( AA.data() );
                return v
                    ? !v->value.empty()
                    : false;
            }
        }
        /**
           Implements AtomAPI::DataDtor() for Utf16String.
        */
        static void apiDataDtor(Atom & self)
        {
            //assertAtomType( self, Utf16String::AtomTypeID );
            Detail::AtomAccess<true> ac(self);
            void * d = ac.data();
            if( !d || (&EmptyImpl() == d) ) return;
            Utf16String::Impl * v = d ? static_cast<Utf16String::Impl*>( d ) : 0;
            //CERR << "Destructing StringType::ValueType @"<<v << " = "<<*v<<'\n';
            if( v )
            {
                ac.data(0);
                Utf16String::deallocValue( v );
            }
        }

        /**
           Implements AtomAPI::CloneData() for Utf16String.
        */
        static void * apiCloneData( Atom const & self, Atom & dest )
        {
            if( Utf16String::AtomTypeID != self.typeID() )
            {
                throw TypeMismatchError( Utf16String::AtomTypeID, self.typeID() );
            }
            else if( Utf16String::AtomTypeID != dest.typeID() )
            {
                throw TypeMismatchError( Utf16String::AtomTypeID, dest.typeID() );
            }
            Impl * y = 0;
            void const * data = Detail::AtomAccess<false>(self).data();
            Impl const * x = data ? static_cast<Impl const *>( data ) : 0;
            Detail::AtomAccess<true> AD(dest);
            if( x )
            {
                if( &EmptyImpl() != x ) x->incr();
                y = const_cast<Impl *>(x);
            }
            else {
                assert(0 && "This should never happen.");
                y = stringImplAllocator.alloc();
                // FIXME: also add this to self, to allow lazy instantiation. Need
                // to make self.data(void*) non-const for that.
            }
            AD.data( y );
            return AD.data();
        }

    public:
        APIImpl()
            : AtomAPI(Utf16String::AtomTypeID,
                      apiCloneData,
                      apiDataDtor,
                      apiToBool,
                      Compare_Utf16String )    
        {}
        static AtomAPI const & instance()
        {
            static APIImpl bob;
            return bob;
        }
    };
    StringImplType & EmptyImpl()
    {
        static StringImplType bob;
        return bob;
    }

    AtomAPI const * Utf16String::API()
    {
        return &Utf16String::APIImpl::instance();
    }
    Utf16String::Impl * Utf16String::allocValue( Utf16String::ValueType const & val,
                                                      Utf16String::Impl * origin )
    {
        if( origin )
        {
            origin->incr();
            return origin;
        }
        else
        {
            Impl * rv = stringImplAllocator.alloc();
            if( ! rv )
            {
                throw AllocError(__FILE__,__LINE__);
            }
            else
            {
                rv->value = val;
            }
            return rv;
        }
    }

    Utf16String::ValueType Utf16String::value() const
    {
        Impl const * v = this->impl();
        if( ! v )
        {
            throw Error("Internal error: Utf16String has no internal value set!");
        }
        else
        {
            return v->value;
        }
    }

    Utf16String::CharType const * Utf16String::c_str() const
    {
        return value().c_str();
    }

    
    void Utf16String::deallocValue( Utf16String::Impl * val )
    {
        if( val && (&EmptyImpl() != val) )
        {
            stringImplAllocator.dealloc(val);
        }
    }

    void Utf16String::init( Utf16String::ValueType const & val,
                            Utf16String::Impl * origin )
    {
        this->m_api = &Utf16String::APIImpl::instance();
        Impl * s = Utf16String::allocValue( val, origin );
        if( !s )
        {
            throw AllocError(__FILE__,__LINE__);
        }
        this->m_data = s;
    }    

    Utf16String::Utf16String() : Atom(&Utf16String::APIImpl::instance())
    {
        this->init(ValueType(),&EmptyImpl());
    }
    Utf16String::~Utf16String()
    {
    }
    Utf16String::Utf16String( Utf16String::ValueType const & v )
        : Atom(&Utf16String::APIImpl::instance())
    {
        this->init(v,0);
    }

    void Utf16String::init( char const * v, size_t len )
    {
        size_t const sl = (v && *v && len) ? len : 0;
        if( ! sl )
        {
            this->init( ValueType(), 0 );
            return;
        }
        char const * end = v+sl;
#if 1
        char const * invend = utf8::find_invalid( v, end );
        if( end != invend )
        {
            std::ostringstream os;
            os << "Utf16String: contains invalid UTF8 near byte #"
               << (invend- v)
               << "! String="<<v;
            throw Error(os.str().c_str());
        }
#endif
        CERR << "Initing via vector of size "<<sl<<"\n";
        ValueType vec;
        vec.reserve( sl );
        utf8::utf8to16( v, end, std::back_inserter( vec ) );
        this->init( vec, 0);
    }

    Utf16String::Utf16String( char const * v, size_t len )
        : Atom(&Utf16String::APIImpl::instance())
    {
        size_t const sl = (v&&*v) ? len : 0;
        if( 0 == sl )
        {
            this->init( ValueType(), 0 );
        }
        else
        {
            this->init( v, sl );
        }
    }

    Utf16String::Utf16String( char const * v )
        : Atom(&Utf16String::APIImpl::instance())
    {
        size_t const sl = v ? strlen( v ) : 0;
        if( 0 == sl )
        {
            this->init( ValueType(), 0 );
        }
        else
        {
            this->init( v, sl );
        }
    }

    void Utf16String::init( wchar_t const * v, size_t len )
    {
        size_t const sl = (v&&len) ? len : 0;
        if( ! sl )
        {
            this->init( ValueType(), 0 );
            return;
        }
        ValueType vec;
        vec.reserve( sl );
        //std::copy( v, v+sl, std::back_inserter(vec) );
        vec.assign( v, v+sl );
        this->init( vec, 0);
    }

    Utf16String::Utf16String( wchar_t const * v )
        : Atom(&Utf16String::APIImpl::instance())
    {
        size_t sl = 0;
        wchar_t const * x = v;
        for( ; x && *x; ++x ) {++sl; }
        if( 0 == sl )
        {
            this->init( ValueType(), 0 );
        }
        else
        {
            this->init( v, sl );
        }
    }


    void Utf16String::init( Utf16String::CharType const * v, size_t len )
    {
        size_t const sl = (v&&len) ? len : 0;
        if(sl)
        {
            ValueType vec;
            vec.reserve( sl );
            //CERR << "vec.size()=="<<vec.size()<<'\n';
            //std::copy( v, v+sl, std::back_inserter(vec) );
            vec.assign( v, v+sl );
            //CERR << "sl="<<sl<<", vec.size()=="<<vec.size()<<'\n';
            this->init( vec, 0 );
        }
        else
        {
            this->init( ValueType(), &EmptyImpl() );
        }

    }

    Utf16String::Utf16String( wchar_t const * v, size_t len )
        : Atom(&Utf16String::APIImpl::instance())
    {
        size_t const sl = v ? len : 0;
        if( 0 == sl )
        {
            this->init( ValueType(), 0 );
        }
        else
        {
            this->init( v, sl );
        }
    }


    Utf16String::Utf16String( Utf16String::CharType const * v, size_t len )
        : Atom(&Utf16String::APIImpl::instance())
    {
        this->init( v, len );
    }
    Utf16String::Utf16String( Utf16String::CharType const * v )
        : Atom(&Utf16String::APIImpl::instance())
    {
        size_t len = 0;
        for( CharType const * x = v; x && *x; ++x )
        {
            ++len;
        }
        this->init( v, len );
    }


    
    
    Utf16String::Utf16String( Utf16String const & v )
    {
        
        this->init( ValueType(), v.impl() );
    }
    Utf16String & Utf16String::operator=( Utf16String const & v )
    {
        if( &v != this ) this->copyOp( v );
        return *this;
    }
    bool Utf16String::operator<( Utf16String const & other ) const
    {
        if( this->m_data ) {
            return  other.m_data
                ? (this->impl()->value < other.impl()->value)
                : false;
        }
        else {
            return other.m_data ? true : false;
        }
    }
    Utf16String::Utf16String( Utf8String const & v )
    {
        *this = v;
    }

    Utf16String & Utf16String::operator=( Utf8String const & v )
    {
#if 1
        // works:
        Utf8String::ValueType const & str(v.value());
        return *this = Utf16String( str.c_str(), str.length() );
#else
        // doesn't work:
        return *this = Utf16String( v.c_str(), v.lengthBytes() );
#endif
        // ^^^ cannot copyOp(v) b/c that would replace this->m_api with Utf8String::API.
    }

    Utf16String::Utf16String( std::string const & v )
    {
        this->init( v.empty() ? 0 : v.c_str(), v.size() );
    }
    Utf16String::Utf16String( std::wstring const & v )
    {
        this->init( v.empty() ? 0 : v.c_str(), v.size() );
    }

    bool Utf16String::empty() const
    {
        Impl const * v = this->impl();
        return v ? v->value.empty() : false;
    }

    size_t Utf16String::lengthBytes() const
    {
        return this->m_data
            ? this->impl()->value.size() * sizeof(Utf16String::CharType)
            : 0;
    }
    size_t Utf16String::lengthChars() const
    {
        return this->m_data
            ? this->impl()->value.size()
            : 0;
    }

    bool Utf16String::isUtf16String( Atom const & a )
    {
        return (API() == a.api());
    }

    Utf16String Utf16String::cast( Atom const & a )
    {
        switch( a.typeID() )
        {
          case Utf8String::AtomTypeID:
              return Utf16String( Utf8String::cast( a ) );
          case AtomTypeID: {
              Utf16String rv;
              rv.copyOp( a );
              return rv;
          }
          default:
              assertAtomType( a, AtomTypeID ) /* will throw */;
              break;
        }
        return Utf16String() /* cannot happen*/;
    }

    Utf16String::Impl * Utf16String::impl() const
    {
        return this->m_data
            ? static_cast<Impl *>(this->m_data)
            : 0;
    }

    Utf8String Utf16String::utf8Value() const
    {
        Impl const * v = this->impl();
        if( ! v || v->value.empty() ) return Utf8String();
        else
        {
            //CERR << "Converting utf16 string value with length="<<v->size()<< " to utf8...\n";
            std::string u8;
            u8.reserve( v->value.end() - v->value.begin() );
            utf8::utf16to8( v->value.begin(), v->value.end(), std::back_inserter( u8 ) );
            //CERR << "u8.size()="<<u8.size() << '\n';//<<"=["<<u8<<"]\n";
            Utf8String rv( u8.c_str(), u8.size() );
            //CERR << "Utf8String="<<rv.size()<<"=["<<rv<<"]\n";
            return rv;
        }
    }

    Utf16String::CharType Utf16String::charCodeAt( uint32_t pos ) const
    {
        Impl const * v = this->impl();
        if( ! v ) return 0;
        else
        {
            return ( pos >= v->value.size() )
                ? 0
                : v->value[pos];
        }
    }

    StringType AtomToString<Utf16String>::Value( Utf16String const & src )
    {
        return StringType(src);
    }

    std::ostream & operator<<( std::ostream & os, Utf16String const & a )
    {
        return os << a.utf8Value();
    }
    

}
