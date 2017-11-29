#include <cmath> /* isnan() and friends. */
#include <sstream>
#include "wh/nosjob/nosjob.hpp"
#include "wh/nosjob/Allocator.hpp"
#include "wh/nosjob/AtomAccess.hpp"
namespace nosjob {

    typedef RcValuePagedAllocator< Detail::RcValue<Double::ValueType> > DoubleImplAllocator;
    static const DoubleImplAllocator doubleImplAllocator(128,-1);

    //! AtomAPI::Compare impl for Double
    static int Compare_Double(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType( lhs, TypeID::Double );
        TypeID::Types const rt = rhs.typeID();
        Double::ValueType const lv = Double::cast(lhs).value();
        Double::ValueType rv;
        switch( rt )
        {
          case TypeID::Boolean:
              rv = Double::ValueType(lhs.boolValue());
              break;
          case TypeID::Integer:
              rv = Double::ValueType(Integer::cast(rhs).value());
              break;
          case TypeID::Double:
              rv = Double::cast(rhs).value();
              break;
          default: 
              return ( Double::TypeID < rt ) ? -1 : 1;
        };
        if( lv < rv ) return -1;
        else if( lv == rv ) return 0;
        else return 1;
    }
    
    class Double::APIImpl : public AtomAPI
    {
        /**
           TODO: for platforms where sizeof(void*)>=sizeof(double),
           use clone/allocation impls which simply store the
           data in the internal m_data pointer.           
        */
    public:
        typedef Double::Impl Impl;
        static Impl & Zero()
        {
            static Impl bob(0.0);
            return bob;
        }
    private:
        friend class Double;
        /**
           Allocates a double using rcAlloc<ValueType>(). If orig is
           not NULL then the new object is assigned its value, else it
           is assigned val.

           If (origin==&Zero()) or (val==Zero) then &Zero() is returned.

       
           Return the new value (or an old one, re-used), which the
           caller owns and must free it using deallocValue().
        */
        static Impl * allocValue( ValueType val, Impl * origin )
        {
            if( (origin == &Zero()) || (0.0 == val) )
            {
                return &Zero();
            }
            else if( origin )
            {
                origin->incr();
                return origin;
            }
            Impl * rv = doubleImplAllocator.alloc();
            if( rv )
            {
                rv->value = val;
            }
            else
            {
                throw AllocError(__FILE__,__LINE__);
            }
            return rv;
        }
        /**
           Deallocates a value allocated via allocValue().
        */
        static void deallocValue( Impl * val )
        {
            if( val && (val != &Zero() ) )
            {
                doubleImplAllocator.dealloc(val);
            }
        }

        /**
           Implements AtomAPI::AtomToBool() for Double. Returns true
           if a is-a Double and has a non-zero value.
        */
        static bool apiToBool( Atom const & a )
        {
            if( ! Double::isDouble(a) ) return false;
            else
            {
                Detail::AtomAccess<false> AC(a);
                void const * v = AC.data();
                if( ! v ) return false;
                Impl const * impl = static_cast<Impl const *>( v );
                return impl && (0.0 != impl->value);
            }
        }

        /**
           Implements AtomAPI::CloneData() for Double.
        */
        static void * apiCloneData( Atom const & self, Atom & dest )
        {
            assertAtomType( self, Double::AtomTypeID );
            assertAtomType( dest, Double::AtomTypeID );
            Impl * y = 0;
            Impl const * x = static_cast<Impl const *>( self.dataPtr() );
            Detail::AtomAccess<true> AD(dest);
            if( x )
            {
                if( &Zero() != x ) x->incr();
                y = const_cast<Impl*>(x);
            }
            AD.data( y );
            return AD.data();
        }

        /**
           Implements AtomAPI::DataDtor() for Double.
        */
        static void apiDataDtor(Atom & self)
        {
            assertAtomType( self, Double::AtomTypeID );
            Detail::AtomAccess<true> ac(self);
            Impl * v = static_cast<Impl*>( ac.data() );
            if( v && (v != &Zero()))
            {
                doubleImplAllocator.dealloc( v );
            }
        }
    public:
        APIImpl()
            : AtomAPI( Double::AtomTypeID,
                       apiCloneData,
                       apiDataDtor,
                       apiToBool,
                       Compare_Double )
        {}
    };

    AtomAPI const * Double::API()
    {
        static const Double::APIImpl bob;
        return &bob;
    }

    Double::Double() : Atom( Double::API() )
    {
        this->init( 0.0, &Double::APIImpl::Zero() );
    }
    Double::Double( ValueType v ) : Atom( Double::API() )
    {
        this->init(v, 0);
    }
    Double::Double( Double const & d )
        : Atom( Double::API() )
    {
        this->init( d.value(), d.impl() );
    }
    Double::Double( Atom const & v )
        : Atom( Double::API() )
    {
        Detail::AtomAccess<false> AC(v);
        switch( v.typeID() )
        {
          case AtomTypeID:
              this->init( 0.0, const_cast<Impl *>( static_cast<Impl const *>( AC.data() ) ) );
              break;
          case Integer::AtomTypeID:
              this->init( Integer::cast(v).value(), 0 );
              break;
          default: {
              std::ostringstream os;
              os << "Double(Atom) cannot convert atom of type "<<v.typeID()<<".";
              throw Error( os.str().c_str() );
          }; break;
        }
    }
    
    Double & Double::operator=( Double const & v )
    {
        this->copyOp(v);
        return *this;
    }
    Double::Double( Integer const & v )
        : Atom( Double::API() )
    {
        this->init( ValueType(v.value()), 0 );
    }
    Double & Double::operator=( Integer const & v )
    {
        Double tmp( ValueType(v.value()) );
        this->copyOp( tmp );
        return *this;
    }
    Double::~Double()
    {
    }
    double Double::value() const
    {
        Impl const * d = this->impl();
        return d ? d->value : 0;
    }
   
  
    void Double::init( Double::ValueType v, Double::Impl * origin )
    {
        this->m_data = Double::APIImpl::allocValue( v, origin );
        if( ! this->m_data )
        {
            throw AllocError(__FILE__,__LINE__);
        }
        //CERR << "Double@"<<this<<"::init() allocated value @"<<x<<'\n';
    }

    Double Double::cast( Atom const & a )
    {
        switch( a.typeID() )
        {
          case Double::AtomTypeID: {
              Double rv;
              rv.copyOp( a );
              return rv;
          }
          case Integer::AtomTypeID:
              return Double( Double::ValueType( Integer::cast( a ).value() ) );
          case Boolean::AtomTypeID:
              return Double( Double::ValueType( a.boolValue() ? 1.0 : 0.0 ) );
          default:
              assertAtomType( a, AtomTypeID ) /* will throw. */;
              break;
        }
        return Double() /* cannot happen. */;
    }

    Double::Impl * Double::impl() const
    {
        return m_data ? static_cast<Impl *>(m_data) : 0;
    }

    bool Double::isDouble( Atom const & a )
    {
        return (API() == a.api());
    }

    bool Double::isNaN() const
    {
        Impl const * d = this->impl();
        return d
            ? std::isnan( d->value )
            : false;
    }

    bool Double::isInfinity() const
    {
        Impl const * d = this->impl();
        return d
            ? std::isinf( d->value )
            : false;
    }
    
    bool Double::isFinite() const
    {
        Impl const * d = this->impl();
        return d
            ? std::isfinite( d->value )
            : true;
    }
    double Double::remainer( Double const & whenDividedBy )
    {
        return ::remainder( this->value(), whenDividedBy.value() );
    }

    StringType AtomToString<Double>::Value( Double const & v )
    {
        enum { Len = 32 };
        char buf[Len];
        memset( buf, 0, Len );
        const Double::ValueType dv = v.value();
        int sl = sprintf( buf, Detail::PrnScnFmt<Double::ValueType>::pfmt, dv );
        if( sl <= 0 )
        {
            throw Error("Internal error converting Double to String.");
        }
        if(1)
        { /* trim trailing zeros. */
            char * x = (buf + sl -1);
            for( ; x && (x != buf) && ('0'==*x) && ('.' != *(x-1)); --x )
            {
                *x = 0;
                --sl;
            }
        }
        assert( sl > 0 );
        return StringType( buf, static_cast<size_t>(sl) );
    }
    std::ostream & operator<<( std::ostream & os, Double const & a )
    {
        return os << a.value();
    }

}
