#include "wh/nosjob/nosjob.hpp"
#include "wh/nosjob/Allocator.hpp"
#include "wh/nosjob/AtomAccess.hpp"
#include <stdlib.h> /*malloc(), free()*/
namespace nosjob {
    //! AtomAPI::Compare impl for Object
    static int Compare_Object(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType(lhs, TypeID::Object);
        if( ! Object::isObject(rhs) ) {
            return (Object::TypeID < rhs.typeID()) ? -1 : 1;
        }
        Object const & lo( Object::cast(lhs) );
        Object const & ro( Object::cast(rhs) );
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
        Object::ConstIterator li = lo.begin();
        Object::ConstIterator ri = ro.begin();
        Object::ConstIterator end = lo.end();
        for( ; li != end; ++li, ++ri )
        {
            Object::Entry const & le = (*li);
            Object::Entry const & re = (*ri);
            int cmp = le.key.compare(re.key);
            if( cmp < 0 ) return -1;
            else if( 0 < cmp ) return 1;
            cmp = le.value.compare(re.value);
            if( cmp < 0 ) return -1;
            else if( 0 < cmp ) return 1;
        }
        return 0;
    }
    /**
       Returns the "empty object" marker. Not yet really used.
       
    */
    static Object::ValueType & EmptyImpl();

    struct Object::Impl
    {
        Object::ContainerType map;
        Impl() {}
        void set( Object::KeyType const & key, Atom const & val )
        {
            map[key] = val;
        }
        Atom const & get( Object::KeyType const & key, Atom const & defaultVal ) const
        {
            Object::ContainerType::const_iterator it = map.find(key);
            return (map.end() == it) ? defaultVal : (*it).second;
        }
        bool has( Object::KeyType const & key ) const
        {
            Object::ContainerType::const_iterator it = map.find(key);
            return map.end() != it;
        }
        bool remove( Object::KeyType const & key )
        {
            Object::ContainerType::iterator it = map.find(key);
            return (map.end() != it)
                ? (map.erase(it),true)
                : false
                ;
        }
    };

    static const RcValuePagedAllocator<Object::ValueType> objImplAllocator(128,-1);
    
    class Object::APIImpl : public AtomAPI
    {
    public:
        typedef Object::RcImpl Impl;
        friend class Object;
        /**
           Deallocates a value allocated via allocValue().
        */
        static void deallocValue( Impl * val )
        {
            if( val )
            {
                objImplAllocator.dealloc(val);
            }
        }

        /**
           Implements AtomAPI::CloneData() for Object.
        */
        static void * apiCloneData( Atom const & self, Atom & dest )
        {
            assertAtomType( self, Object::AtomTypeID );
            assertAtomType( dest, Object::AtomTypeID );
            Impl * y = 0;
            Impl const * x = static_cast<Impl const *>( self.dataPtr() );
            Detail::AtomAccess<true> AD(dest);
            if( x )
            {
                if( &EmptyImpl() != x ) x->incr();
                y = const_cast<Impl*>(x);
                //CERR << "Shallowly cloned Object @"<<x<<" to @"<<y << '\n';
            }
            AD.data( y );
            return AD.data();
        }

        /**
           Implements AtomAPI::DataDtor() for Object.
        */
        static void apiDataDtor(Atom & self)
        {
            assertAtomType( self, Object::AtomTypeID );
            Detail::AtomAccess<true> ac(self);
            void * d = ac.data();
            if( !d || (&EmptyImpl() ==d) ) return;
            Impl * v = d ? static_cast<Impl*>( d ) : 0;
            if( v )
            {
                //CERR << "Destructing Object @"<<v << " = "<<*v<<'\n';
                ac.data(0);
                deallocValue( v );
            }
        }
        static bool apiToBool(Atom const & self)
        {
            return true;
        }

    public:
        APIImpl()
            : AtomAPI(Object::AtomTypeID,
                      apiCloneData,
                      apiDataDtor,
                      apiToBool,
                      Compare_Object)
        {}
        static AtomAPI const & instance()
        {
            static APIImpl bob;
            return bob;
        }

    };
    Object::ValueType & EmptyImpl()
    {
        static Object::ValueType rv;
        return rv;
    }

    AtomAPI const * Object::API()
    {
        return &Object::APIImpl::instance();
    }

    std::ostream & operator<<( std::ostream & os, Object const & a )
    {
        std::ostream_iterator<char> oit = std::ostream_iterator<char>(os);
        JsonStreamer<Object>::streamOut( a, oit );
        return os;
    }

    void Object::allocImpl( Object::RcImpl * origin ) const
    {
        if( this->m_data ) throw Error("Object::allocImpl() was called when we already have an impl object.");
        RcImpl * v = origin;
        if( origin )
        {
            origin->incr();
        }
        else
        {
            v = objImplAllocator.alloc();
            if( !v ) throw AllocError(__FILE__,__LINE__);
        }
        this->dataPtr( v );
    }
    
//     void Object::dealyedAlloc()
//     {
//         RcImpl * i = this->impl();
//         if( !i || (&EmptyImpl() == i)  )
//         {
//             this->allocValue(0);
//         }
//     }
    void Object::init( Object::RcImpl * origin )
    {
        this->m_api = Object::API();
        //if( origin )
        // Lazy instantiation plays havoc with cast().
            this->allocImpl(origin);
    }

    
    Object::Object() : Atom( Object::API() ),
                       MapParentType()
    {
        this->init(0);
    }

    Object::~Object()
    {
    }

    Object::Object( Object const & other )
        : Atom(Object::API()),
          MapParentType()
    {
        this->operator=(other);
        assert( this->m_data == other.m_data );
        assert( 0 != this->atomMap() );
    }

    Object & Object::operator=( Object const & other )
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
            assert( 0 != other.atomMap() );
            this->destroyData();
            this->allocImpl( other.impl() );
            assert( this->m_data == other.m_data );
            assert( 0 != this->atomMap() );
        }
        return *this;
    }

    Object Object::cast( Atom const & a )
    {
        if( Object::isObject(a) )
        {
            Object rv;
            rv.copyOp( a );
            //rv.allocImpl( static_cast<RcImpl const *>(a.dataPtr()) );
            assert( rv.impl() );
            //rv.atomMap( rv.impl()->map );
            return rv;
        }
        else
        {
            assertAtomType( a, AtomTypeID ) /* will throw. */;
        }
        return Object() /* cannot happen. */;
    }
    
    Object::RcImpl * Object::impl() const
    {
        return this->m_data
            ? static_cast<RcImpl*>(this->m_data)
            : 0;
    }

    bool Object::isObject( Atom const & a )
    {
        return (API() == a.api());
    }

    void Object::set( Atom const & key, Atom const & val )
    {
        if( ! impl() ) this->allocImpl(0);
        impl()->value.set( atomToString(key), val );
    }
    Atom Object::get( Atom const & key, Atom const & defaultValue ) const
    {
        
        return this->m_data ?
            impl()->value.get( atomToString(key), defaultValue )
            : defaultValue;
    }
    bool Object::remove( Atom const & key )
    {
        return this->m_data ? impl()->value.remove(atomToString(key)) : false;
    }
    bool Object::has( Atom const & key ) const
    {
        return this->m_data ? impl()->value.has( atomToString(key) ) : false;
    }
    Atom Object::take( Atom const & key )
    {
        if( ! this->m_data ) return Atom::Undefined;
        RcImpl * imp = this->impl();
        Object::ContainerType::iterator it =imp->value.map.find( atomToString(key) );
        if( imp->value.map.end() == it ) return Atom::Undefined;
        else
        {
            const Atom rc((*it).second);
            imp->value.map.erase(it);
            return rc;
        }
    }
    Object::ContainerType * Object::atomMap() const
    {
        RcImpl * imp = impl();
        return imp ? &imp->value.map : 0;
    }

}
