#include <cassert>
#include <sstream>

// #ifndef CERR
// #include <iostream> /* only for debuggering */
// #define CERR std::cerr << __FILE__ << ":" << std::dec << __LINE__ << " : "
// #endif

#include "wh/nosjob/nosjob.hpp"
namespace nosjob {

    /** Compare() impl for Undefined type. lhs MUST be Undefined.
        Undefined compares less than to all types but itself.
    */
    static int Compare_Undefined(Atom const & lhs, Atom const & rhs )
    {
        assertAtomType(lhs, TypeID::Undefined);
        // See notes in Compare_Null() for why we can't just use rhs.boolValue()
        return (rhs.typeID() == TypeID::Undefined)
            ? 0
            : ((rhs.typeID() == TypeID::Null)
               ? 0
               : -1);
    }
    
    /** Compare() impl for Null type. lhs MUST be Null.  Null
        compares less than any type except itself or Undefined
        (which is considered less than Null).
    */
    static int Compare_Null(Atom const & lhs, Atom const & rhs )
    {
        assertAtomType(lhs, TypeID::Null);
        /*
          In JS:

          (null==false) == true

          (null==undefined) == true

          (null=="") == false

          So we can't use:

          return rhs.boolValue() ? 0 : -1;
        */
        return (rhs.typeID() == TypeID::Null)
            ? 0
            : ((rhs.typeID() == TypeID::Undefined)
               ? 1
               : -1);
    }

    const Atom Atom::Null = Atom( &AtomAPI::Null() );
    const Atom Atom::Undefined = Atom( &AtomAPI::Undefined() );
#define PF(T,PF,SF) const char * const Detail::PrnScnFmt< T >::pfmt = "%"PF; \
        const char * const Detail::PrnScnFmt< T >::sfmt = "%"SF
        PF(int32_t,PRIi32,SCNi32);
        PF(int64_t,PRIi64,SCNi64);
        //     PF(uint32_t,PRIu32,SCNu32);
        //     PF(uint64_t,PRIu64,SCNu64);
        PF(double,"f","f");
#undef PF
  
    void assertAtomType( Atom const & a, TypeID::Types t )
    {
        if( a.typeID() != t )
        {
            std::ostringstream os;
            os << "Atom(type="<<a.typeID()<<"): TypeID check failed: expecting "<< t<<'!';
            throw Error( os.str().c_str() );
        }
    }

    Error::Error( char const * what )
        : m_what( (what&&*what) ? what : "<unspecified error>" )
    {
    }
    Error::Error() throw()
        : m_what()
    {}
    
    Error::~Error() throw()
    {
    }
    char const * Error::what() const throw()
    {
        return this->m_what.c_str();
    }

    AllocError::AllocError() throw()
        : Error(),
          m_file("UNKNOWN"),
          m_line(-1)
    {}
    AllocError::AllocError( char const * file, int line ) throw()
        : Error(),
          m_file((file&&*file) ? file : "UNKNOWN"),
          m_line(line)

    {
    }
    AllocError::~AllocError() throw()
    {}

    char const * AllocError::file() const throw()
    { return this->m_file; }
    int AllocError::line() const throw()
    { return this->m_line; }

    char const * AllocError::what() const throw()
    {
        return "Memory-allocation error! (We cannot report more info without allocating space for it!)";
    }

    TypeMismatchError::TypeMismatchError( TypeID::Types expected, TypeID::Types got )
        : Error()
    {
        std::ostringstream os;
        os << "TypeID mismatch: expected "<< expected << " but got "<<got <<'!';
        this->m_what = os.str();
    }
    TypeMismatchError::~TypeMismatchError() throw()
    {
    }
    

    AtomAPI const & AtomAPI::Undefined()
    {
        static AtomAPI bob( TypeID::Undefined,
                            AtomAPI::CopyValue_copy_ptr,
                            0,
                            AtomAPI::ToBool_voidptr,
                            Compare_Undefined);
        return bob;
    }
    AtomAPI const & AtomAPI::Null()
    {
        static AtomAPI bob( TypeID::Null,
                            AtomAPI::CopyValue_copy_ptr,
                            0,
                            AtomAPI::ToBool_voidptr,
                            Compare_Null);
        return bob;
    }
    static AtomAPI const * AtomAPI_Default = &AtomAPI::Undefined();
    //static AtomAPI const * AtomAPI_Null = &AtomAPI::Null();
    
    bool AtomAPI::ToBool_voidptr( Atom const & self )
    {
        return (0 != self.dataPtr());
    }
    void AtomAPI::DataDtor_noop(Atom & self)
    {
    }
    bool AtomAPI::ToBool_false(Atom const & self)
    {
        return false;
    }

    void * AtomAPI::CopyValue_copy_ptr( Atom const & self, Atom & dest )
    {
        dest.destroyData();
        return dest.m_data = self.m_data;
    }
    
    Atom::Atom()
        : m_api(AtomAPI_Default),
          m_data(0)
    {
    }
//     Atom::Atom( TypeID::Types t ) :
//         m_api(AtomAPI_Default),
//         m_data(0)
//     {
//         if( ! TypeID::isAtom(t) && !TypeID::isCompound(t) )
//         {
//             std::ostringstream os;
//             os << "Atom(TypeID="<<t<<"): TypeID is-not-a Atom or Compound type!";
//             throw Error( os.str().c_str() );
//         }
//     }

    Atom::Atom( AtomAPI const * api ) :
        m_api(api),
        m_data(0)
    {
        if( ! TypeID::isAtom(api->typeID) && !TypeID::isCompound(api->typeID) )
        {
            std::ostringstream os;
            os << "Atom(TypeID="<<api->typeID<<"): TypeID is-not-a Atom or Compound type!";
            throw Error( os.str().c_str() );
        }
    }
    Atom::Atom( Atom const & a )
        : m_api(AtomAPI_Default),
          m_data(0)
    {
        this->copyOp(a);
    }
    Atom::~Atom()
    {
        this->destroyData();
    }
    TypeID::Types Atom::typeID() const
    {
        return this->m_api ? this->m_api->typeID : TypeID::Undefined;
    }

    Atom & Atom::operator=( Atom const & a )
    {
        return this->copyOp( a );
    }
    Atom & Atom::copyOp( Atom const & rhs )
    {
        if( &rhs != this )
        {
            this->destroyData();
            this->m_api = rhs.m_api;
            this->m_data = 0;
            if( rhs.m_data && rhs.m_api && rhs.m_api->cloneData )
            {
                if( ! rhs.m_api->cloneData( rhs, *this ) )
                {
                    throw Error("Atom copy op: cloning data failed!");
                }
            }
            else
            {
                this->m_data = rhs.m_data;
            }
        }
        return *this;
    }
    AtomAPI const * Atom::api() const
    {
        if( this->m_api ) return this->m_api;
        throw std::runtime_error("Atom is missing its api member!");
        // ^^^ FIXME: a more descriptive error message: this-ptr, type ID, flags
        return 0;
    }

    bool Atom::isA( TypeID::Types id ) const
    {
        return this->typeID() == id;
    }

    void Atom::destroyData()
    {
        //CERR << "Atom@"<<this<<"::destroyData() data@"<<this->m_data<<"."<<std::endl;
        if( this->m_data && this->m_api && this->m_api->dtor )
        {
            //CERR << "Atom@"<<this<<" passing @"<<this->m_data<<" to data dtor.\n";
            this->m_api->dtor( *this );
        }
        this->m_data = 0;
    }

    bool Atom::boolValue() const
    {
        return this->m_api
            && this->m_api->toBool
            && this->m_api->toBool( *this )
            ;
    }

    bool Atom::isUndefined() const
    {
        return this->m_api->typeID == TypeID::Undefined;
    }

    bool Atom::isNull() const
    {
        return this->m_api->typeID == TypeID::Null;
    }

    bool Atom::hasValue() const
    {
        return !isUndefined() && !isNull();
    }
    
    StringType atomToString( Atom const & v )
    {
        switch( v.typeID() ) {
          case TypeID::Undefined:
               return StaticStrings.Undefined;
               break;
          case TypeID::Null:
              return StaticStrings.Null;
              break;
          case TypeID::Boolean:
              return AtomToString<Boolean>::Value( Boolean::cast(v) );
              break;
          case TypeID::Integer:
              return AtomToString<Integer>::Value( Integer::cast(v) );
              break;
          case TypeID::Double:
              return AtomToString<Double>::Value( Double::cast(v) );
              break;
          case TypeID::Utf8String:
              return AtomToString<Utf8String>::Value( Utf8String::cast(v) );
              break;
          case TypeID::Utf16String:
              return AtomToString<Utf16String>::Value( Utf16String::cast(v) );
              break;
          case TypeID::Object:
          case TypeID::Array:
              return atomToJSON(v);
          default:
              throw Error("Unexpected Atom type passed to atomToString()");
        }
    }

    StringType atomToJSON( Atom const & v )
    {
        std::ostringstream os;
        JsonStreamer<Atom>::streamOut( v, std::ostream_iterator<char>(os) );
        std::string const & str = os.str();
        return StringType( str.c_str(), str.size() );
    }
    void atomToJSON( Atom const & a, std::ostream & os, uint8_t spacing )
    {
        atomToJSON( a, std::ostream_iterator<char>(os), spacing );
    }
    
    /**
       Due to static construction ordering, this MUST come after
       StringType::API has been set up!
     */
    const struct StaticStrings_ StaticStrings = {
    StringType("undefined",9),
    StringType("null",4),
    StringType("true",4),
    StringType("false",5),
    StringType(/*Empty*/)
    };

    std::ostream & operator<<( std::ostream & os, Atom const & a )
    {
        switch( a.typeID() )
        {
          case TypeID::Undefined:
              return os << "undefined";
          case TypeID::Null:
              return os << "null";
          case TypeID::Boolean:
              return os << Boolean::cast(a);
          case TypeID::Integer:
              return os << Integer::cast(a);
          case TypeID::Double:
              return os << Double::cast(a);
          case TypeID::Utf8String:
              return os << Utf8String::cast(a);
          case TypeID::Utf16String:
              return os << Utf16String::cast(a);
          case TypeID::Object:
              return os << Object::cast(a);
          case TypeID::Array:
              return os << Array::cast(a);
          default:
              throw new Error("Unhandled type ID passed to JsonStreamer<Atom>::streamOut()");
        }
    }

    
} /* ::nosjob */
