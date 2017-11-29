#include "wh/nosjob/nosjob.hpp"

namespace nosjob {

    //! AtomAPI::Compare impl for Integer
    static int Compare_Integer(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType( lhs, TypeID::Integer );
        TypeID::Types const rt = rhs.typeID();
        Integer::ValueType const lv = Integer::cast(lhs).value();
        switch( rt )
        {
          case TypeID::Boolean:
          case TypeID::Double:
          case TypeID::Integer: {
              Integer::ValueType const rv = Integer::cast(rhs).value();
              if( lv < rv ) return -1;
              else if( lv == rv ) return 0;
              else return 1;
          };
          default: 
              return ( Integer::TypeID < rt ) ? -1 : 1;
        };
    }

    class Integer::APIImpl : public AtomAPI
    {
    public:
        APIImpl()
            : AtomAPI( Integer::AtomTypeID,
                       AtomAPI::CopyValue_copy_ptr,
                       0/*dtor*/,
                       AtomAPI::ToBool_voidptr,
                       Compare_Integer )
        {}
    };
    AtomAPI const * Integer::API()
    {
        static const Integer::APIImpl bob;
        return &bob;
    }

    void Integer::init( Integer::ValueType v )
    {
        this->m_api = Integer::API();
        this->m_data = reinterpret_cast<void*>(v);
    }
    
    Integer::Integer() : Atom( Integer::API() ){this->init(ValueType(0));}
//     Integer::Integer( int8_t v ) : Atom( Integer::API() ){this->init(v);}
//     Integer::Integer( int16_t v ) : Atom( Integer::API() ) {this->init(v);}
//     Integer::Integer( int32_t v ) : Atom( Integer::API() ) {this->init(v);}
//     Integer::Integer( uint8_t v ) : Atom( Integer::API() ) {this->init(v);}
//     Integer::Integer( uint16_t v ) : Atom( Integer::API() ) {this->init(v);}
//     Integer::Integer( uint32_t v ) : Atom( Integer::API() ) {this->init(v);}
    Integer::Integer( Integer::ValueType v ) : Atom( Integer::API() ) {this->init(v);}
    Integer::~Integer()
    {
    }

    Integer Integer::cast( Atom const & a )
    {
        switch( a.typeID() )
        {
          case Integer::AtomTypeID: {
              Integer rv;
              rv.copyOp( a );
              return rv;
          }
          case Double::AtomTypeID: {
              const Double::ValueType d = Double::cast( a ).value();
              return Integer( Integer::ValueType( d ) );
          }
          case Boolean::AtomTypeID:
              return Integer( Integer::ValueType( a.boolValue() ? 1 : 0 ) );
          default:
              assertAtomType( a, AtomTypeID ) /* will throw. */;
              break;
        }
        return Integer() /* cannot happen. */;
    }
    
    Integer::ValueType Integer::value() const
    {
        return ValueType(this->m_data);
    }

    bool Integer::isInteger( Atom const & a )
    {
        return (a.api() == API());
    }

    StringType AtomToString<Integer>::Value( Integer const & v )
    {
        enum { Len = 32 };
        char buf[Len];
        memset( buf, 0, Len );
        JsonStreamer<Integer>::streamOut( v, buf );
        //sprintf( buf, Detail::PrnScnFmt<Integer::ValueType>::pfmt, v.value() );
        return StringType( buf );
    }

    std::ostream & operator<<( std::ostream & os, Integer const & a )
    {
        return os << a.value();
    }

}
