#include "wh/nosjob/nosjob.hpp"
#include <sstream>
#include "wh/nosjob/Allocator.hpp"
#include "wh/nosjob/AtomAccess.hpp"
#include <cstring> // strcmp()
#if 0 && !defined(CERR)
#include <iostream> /* only for debuggering */
#define CERR std::cerr << __FILE__ << ":" << std::dec << __LINE__ << " : "
#endif

namespace nosjob {
    Utf8String::ValueType Utf8String::emptyString = Utf8String::ValueType();
    int Utf8String::strcmp( Utf8String const & lhs, Utf8String const & rhs )
    {
        Utf8String::ValueType const & L(lhs.value());
        Utf8String::ValueType const & R(rhs.value());
        return std::strcmp( L.c_str(), R.c_str() );
    }

    //! AtomAPI::Compare impl for Utf8String
    static int Compare_Utf8String(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType( lhs, TypeID::Utf8String );
        TypeID::Types const rt = rhs.typeID();
        switch( rt )
        {
          case TypeID::Utf16String:
              return Utf8String::strcmp( Utf8String::cast(lhs), Utf16String::cast(rhs).utf8Value() );
          case TypeID::Utf8String:
              return Utf8String::strcmp( Utf8String::cast(lhs), Utf8String::cast(rhs) );
          default:
              return (Utf8String::TypeID < rt) ? -1 : 1;
        };
    }

    typedef
        /* Reminder: We don't have access to Utf8String::Impl from
           here, so we duplicate its type.*/
        Detail::RcValue<Utf8String::ValueType>
        StringImplType;
    static StringImplType & EmptyImpl();
    static const RcValuePagedAllocator<StringImplType> stringImplAllocator(128,-1);
    class Utf8String::APIImpl : public AtomAPI
    {
    public:
//         typedef Utf8String::ValueType ValueType;
        typedef Utf8String::Impl Impl;
    private:
        /**
           Implements AtomAPI::ToBool() for Utf8String.

           Returns true only if a is-a string and has a non-empty value.
        */
        static bool apiToBool( Atom const & a )
        {
            if( Utf8String::AtomTypeID != a.typeID() ) return false;
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
           Implements AtomAPI::DataDtor() for Utf8String.
        */
        static void apiDataDtor(Atom & self)
        {
            //assertAtomType( self, Utf8String::AtomTypeID );
            Detail::AtomAccess<true> ac(self);
            void * d = ac.data();
            if( !d || (&EmptyImpl() == d) ) return;
            Utf8String::Impl * v = d ? static_cast<Utf8String::Impl*>( d ) : 0;
            if( v )
            {
                ac.data(0);
                Utf8String::deallocValue(v);
            }
        }

        /**
           Implements AtomAPI::CloneData() for Utf8String.
        */
        static void * apiCloneData( Atom const & self, Atom & dest )
        {
            if( Utf8String::AtomTypeID != self.typeID() )
            {
                throw TypeMismatchError(Utf8String::AtomTypeID, self.typeID() );
            }
            else if( Utf8String::AtomTypeID != dest.typeID() )
            {
                throw TypeMismatchError(Utf8String::AtomTypeID, dest.typeID() );
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
            : AtomAPI(Utf8String::AtomTypeID,
                      apiCloneData,
                      apiDataDtor,
                      apiToBool,
                      Compare_Utf8String )    
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

    AtomAPI const * Utf8String::API()
    {
        return &Utf8String::APIImpl::instance();
    }

    Utf8String::Utf8String() : Atom(&Utf8String::APIImpl::instance())
    {
        this->init(emptyString,&EmptyImpl());
    }
    Utf8String::~Utf8String()
    {
        if( &EmptyImpl() == this->impl() )
        {
            this->dataPtr(0);
        }
    }
    
    Utf8String::Utf8String( ValueType const & v )
        : Atom(&Utf8String::APIImpl::instance())
    {
        this->init(v,0);
    }

    Utf8String & Utf8String::operator=( Utf16String const & v )
    {
        *this = v.utf8Value();
        return *this;
    }
    Utf8String::Utf8String( Utf16String const & v )
        : Atom(&Utf8String::APIImpl::instance())
    {
        this->operator=(v);
//         std::vector<CharType> buf;
//         //buf.reserve( v.lengthChars() );
//         Utf16String::CharType const * begin = v.value();
//         Utf16String::CharType const * end = begin + v.lengthChars();
//         utf8::utf16to8( begin, end, std::back_inserter( buf ) );
//         this->init(v.empty() ? ValueType() : ValueType(buf.begin(),buf.end()),0);
    }
    
    Utf8String::Utf8String( char const * v )
        : Atom(&Utf8String::APIImpl::instance())
    {
        if( v && *v ) {
            this->init( ValueType(v), 0 );
        }
        else {
            this->init( emptyString, &EmptyImpl() );
        }
    }

    Utf8String::Utf8String( char const * v, size_t len )
        : Atom(&Utf8String::APIImpl::instance())
    {
        if( v && *v && len ) {
            this->init( ValueType( v, v+len ), 0 );
        }
        else {
            this->init( emptyString, &EmptyImpl() );
        }
    }
    Utf8String::Utf8String( Utf8String const & v )
        : Atom( &Utf8String::APIImpl::instance() )
    {
        this->init( emptyString, v.impl() );
    }

    bool Utf8String::empty() const
    {
        Impl const * v = this->impl();
        return v ? v->value.empty() : false;
    }
    

    Utf8String::Impl * Utf8String::allocValue( Utf8String::ValueType const & val,
                                               Utf8String::Impl * origin )
    {
        if( origin )
        {
            if( &EmptyImpl() != origin ) origin->incr();
            return origin;
        }
        else
        {
            char const * beg = val.c_str();
            char const * end = beg + val.size();
            char const * invend = utf8::find_invalid( beg, end );
            if( end != invend )
            {
                std::ostringstream os;
                os << "String contains invalid UTF-8 near position "
                   << (invend- beg)
                   << "! String="<<val;
                throw Error(os.str().c_str());
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
    }

    void Utf8String::deallocValue( Utf8String::Impl * val )
    {
        if( val && (&EmptyImpl() != val) )
        {
            stringImplAllocator.dealloc(val);
        }
    }
    
    Utf8String::Impl * Utf8String::impl() const
    {
        return m_data ? static_cast<Impl*>(this->m_data) : 0;
    }

    size_t Utf8String::lengthBytes() const
    {
        return this->m_data
            ? this->impl()->value.size()
            : 0;
    }

    size_t Utf8String::lengthChars() const
    {
        Impl const * d = this->impl();
        return d
            ? static_cast<size_t>( utf8::distance( d->value.begin(), d->value.end() ) )
            : 0;
    }

    bool Utf8String::isUtf8String( Atom const & a )
    {
        return (API() == a.api());
    }
        
    void Utf8String::init( Utf8String::ValueType const & val,
                           Utf8String::Impl * origin )
    {
        this->m_api = &Utf8String::APIImpl::instance();
        Impl * s = Utf8String::allocValue( val, origin );
        if( !s )
        {
            throw AllocError(__FILE__,__LINE__);
        }
        this->m_data = s;
    }    

    Utf8String & Utf8String::operator=( Utf8String const & v )
    {
        if( &v != this ) {
            this->destroyData();
            this->copyOp( v );
        }
        return *this;
    }
    bool Utf8String::operator<( Utf8String const & other ) const
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

    Utf8String::ValueType Utf8String::value() const
    {
        return this->m_data
            ? this->impl()->value
            : emptyString;
    }
    char const * Utf8String::c_str() const
    {
        return this->value().c_str();
    }

    Utf8String Utf8String::cast( Atom const & a )
    {
        switch( a.typeID() )
        {
          case AtomTypeID: {
              Utf8String rv;
              rv.copyOp(a);
              return rv;
          }
          case Utf16String::AtomTypeID:
              return Utf16String::cast(a).utf8Value();
          default:
              assertAtomType( a, AtomTypeID ) /* will throw */;
              break;
        }
        return Utf8String() /* cannot happen*/;
    }

    StringType AtomToString<Utf8String>::Value( Utf8String const & src )
    {
        return StringType(src);
    }

    std::ostream & operator<<( std::ostream & os, Utf8String const & a )
    {
        return os << a.value();
    }
}
