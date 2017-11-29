#include "wh/nosjob/nosjob.hpp"
#include "wh/nosjob/Allocator.hpp"
#include "wh/nosjob/AtomAccess.hpp"
#include <limits> /* numeric_limits<> */
#include <algorithm> /* find() */
namespace nosjob {

    static const RcValuePagedAllocator<Array::ValueType> arrayImplAllocator(128);
    //! AtomAPI::Compare impl for Array
    static int Compare_Array(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType(lhs, TypeID::Array);
        if( ! Array::isArray(rhs) ) {
            return (Array::TypeID < rhs.typeID()) ? -1 : 1;
        }
        Array const & lo( Array::cast(lhs) );
        Array const & ro( Array::cast(rhs) );
        if( Detail::AtomAccess<false>(lo).data() == Detail::AtomAccess<false>(ro).data() )
        {
            // Same object reference.
            return 0;
        }
        if( lo.empty() && ro.empty() ) return 0;
        size_t const lsz = lo.size();
        size_t const rsz = ro.size();
        if( lsz < rsz ) return -1;
        else if( lsz > rsz ) return 1;
        Array::ConstIterator li = lo.begin();
        Array::ConstIterator ri = ro.begin();
        Array::ConstIterator end = lo.end();
        for( ; li != end; ++li, ++ri )
        {
            int cmp = (*li).compare(*ri);
            if( cmp < 0 ) return -1;
            else if( 0 < cmp ) return 1;
        }
        return 0;
    }
    /**
       Returns the "empty array" marker. Not yet really used.
    */
    static Array::ValueType & EmptyArrayImpl();

    struct Array::Impl
    {
        mutable Array::ContainerType map;
        Impl() {}
        void reserve( Array::IndexType sz )
        {
            if( map.size() < sz )
            {
                map.reserve(sz);
                size_t n = sz - map.size();
                for( size_t i = 0; i < n; ++i ) {
                    map.push_back(Atom::Undefined);
                }
            }
        }
        void set( Array::IndexType key, Atom const & val )
        {
            reserve(key+1);
            map.at(key) = val;
        }
        Atom const & get( Array::IndexType key, Atom const & defaultVal ) const
        {
            return (key>=map.size())
                ? defaultVal
                : map.at(key)
                ;
        }
        void remove( Array::IndexType key )
        {
            const size_t sz = map.size();
            if( key < sz )
            {
                map.erase(map.begin() + key);
            }
        }
    };

    class Array::APIImpl : public AtomAPI
    {
    public:
        typedef Array::ValueType Impl;
        friend class Array;
        /**
           Deallocates a value allocated via allocValue().
        */
        static void deallocValue( Impl * val )
        {
            if( val && (val != &EmptyArrayImpl()) )
            {
                arrayImplAllocator.dealloc(val);
            }
        }

        /**
           Implements AtomAPI::CloneData() for Array.
        */
        static void * apiCloneData( Atom const & self, Atom & dest )
        {
            assertAtomType( self, Array::AtomTypeID );
            assertAtomType( dest, Array::AtomTypeID );
            Impl * y = 0;
            Impl const * x = static_cast<Impl const *>( self.dataPtr() );
            Detail::AtomAccess<true> AD(dest);
            if( x )
            {
                if( &EmptyArrayImpl() != x ) x->incr();
                y = const_cast<Impl*>(x);
                //CERR << "Shallowly cloned Object @"<<x<<" to @"<<y << '\n';
            }
            AD.data( y );
            return AD.data();
        }

        /**
           Implements AtomAPI::DataDtor() for Array.
        */
        static void apiDataDtor(Atom & self)
        {
            assertAtomType( self, Array::AtomTypeID );
            Detail::AtomAccess<true> ac(self);
            void * d = ac.data();
            if( !d || (&EmptyArrayImpl() ==d) ) return;
            Impl * v = d ? static_cast<Impl*>( d ) : 0;
            if( v )
            {
                //CERR << "Destructing Array @"<<v << " = "<<*v<<'\n';
                ac.data(0);
                deallocValue( v );
            }
        }
        static bool apiToBool(Atom const & self)
        {
            return true;
        }
        APIImpl()
            : AtomAPI( Array::AtomTypeID,
                       apiCloneData,
                       apiDataDtor,
                       apiToBool,
                       Compare_Array )
        {}
    public:
        static AtomAPI const & instance()
        {
            static APIImpl bob;
            return bob;
        }

    };
    Array::ValueType & EmptyArrayImpl()
    {
        static Array::ValueType rv;
        return rv;
    }

    AtomAPI const * Array::API()
    {
        return &Array::APIImpl::instance();
    }

    const Array::IndexType Array::InvalidIndex = std::numeric_limits<IndexType>::min();
    void Array::allocImpl( Array::ValueType * origin ) const
    {
        ValueType * v = origin;
        if( origin )
        {
            if( origin != &EmptyArrayImpl() ) origin->incr();
        }
        else
        {
            v = arrayImplAllocator.alloc();
            if( !v ) throw AllocError(__FILE__,__LINE__);
        }
        this->m_data = v;
    }

    void Array::init( Array::ValueType * origin )
    {
        this->m_api = Array::API();
        //if( origin )
        // Lazy instantiation plays havoc with cast().
            allocImpl( origin );
    }

    
    Array::Array() : Atom( Array::API() ),
                     Array::ListParentType()
    {
        this->init(0);
    }

    Array::Array( IndexType length )
        : Atom( Array::API() ),
          Array::ListParentType()
    {
        this->init(0);
        if( length > 0 )
        {
            ContainerType & c = impl()->value.map;
            c.reserve(length);
            for( IndexType i = 0; i < length; ++i )
            {
                c.push_back(Atom::Null);
            }
        }
    }

    Array::Array( Array const & other )
        : Atom(Array::API()),
          Array::ListParentType()
    {
        this->operator=(other);
    }

    Array::~Array()
    {
    }

    Array & Array::operator=( Array const & other )
    {
        if( &other != this )
        {
            if( ! other.impl() ) other.allocImpl(0)
                /** Unfortunately necessary to avoid that 2 empty
                    objects assigned from the same origin handle
                    do not end up with different impl objects.
                */
                ;
            assert( 0 != other.impl() );
            assert( 0 != other.atomList() );
            this->destroyData();
            this->allocImpl( other.impl() );
            assert( this->m_data == other.m_data );
            assert( this->atomList() );
        }
        return *this;
    }
    Array::ValueType * Array::impl() const
    {
        return this->m_data
            ? static_cast<ValueType*>(this->m_data)
            : 0;
    }
    bool Array::isArray( Atom const & a )
    {
        return (API() == a.api());
    }

    Array Array::cast( Atom const & a )
    {
        if( Array::isArray(a) )
        {
            Array rv;
            // FIXME: default Array ctor should not allocate, because we throw it away here:
            // However, we HAVE to allocate in the default dtor just for
            // this cast() operation to work :(
            rv.copyOp( a );
            assert( rv.impl() );
            //rv.container( rv.impl()->value.map );
            return rv;
        }
        else
        {
              assertAtomType( a, AtomTypeID ) /* will throw. */;
        }
        return Array() /* cannot happen. */;
    }
    
    Atom Array::get( Array::IndexType key, Atom const & defaultValue ) const
    {
        if( ! this->impl() ) return defaultValue;
        Array::ValueType const * impl = this->impl();
        return impl ? impl->value.get(key,defaultValue) : defaultValue;
    }
    void Array::set( Array::IndexType key, Atom const & val )
    {
        Array::ValueType * impl = this->impl();
        if( ! impl ) this->allocImpl(0);
        impl->value.set(key,val);
    }
    void Array::remove( Array::IndexType key )
    {
        if( this->impl() )
        {
            this->impl()->value.remove(key);
        }
    }

    Array::IndexType Array::push( Atom const & a )
    {
        if( ! this->impl() ) this->allocImpl(0);
        ValueType * impl = this->impl();
        impl->value.map.push_back(a);
        return impl->value.map.size()-1;
    }

#if 0
    Array::IndexType Array::unshift( Atom const & a )
    {
        throw Error("FIXME: Array::unshift() index numbering is broken vis-a-vis unsigned int.");
        // FIXME: increment indexes of all elements
        ValueType * impl = this->impl();
        if( !impl ) return InvalidIndex;
        else {
            Array::ContainerType::iterator it = impl->value.map.begin();
            const IndexType ndx = ( it == impl->value.map.end() )
                ? 0
                : (*it).first -1;
            impl->value.set( ndx, a );
            return ndx;
        }
    }
#endif

    Atom Array::pop()
    {
        ValueType * impl = this->impl();
        if( ! impl ) return Atom::Undefined;
        else
        {
            Array::ContainerType::reverse_iterator it = impl->value.map.rbegin();
            if( it == impl->value.map.rend() ) return Atom::Undefined;
            else
            {
                const Atom rc = (*it);
                impl->value.map.erase( it.base() );
                return rc;
            }
        }
    }

    std::ostream & operator<<( std::ostream & os, Array const & a )
    {
        std::ostream_iterator<char> oit = std::ostream_iterator<char>(os);
        JsonStreamer<Array>::streamOut( a, oit );
        return os;
    }

    Array::ContainerType * Array::atomList() const
    {
        ValueType const * imp = impl();
        return imp ? &imp->value.map : 0;
    }
    
}
