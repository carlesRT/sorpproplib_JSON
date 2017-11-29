#ifndef wanderinghorse_net_nosjob_allocator_HPP_INCLUDED
#define wanderinghorse_net_nosjob_allocator_HPP_INCLUDED 1

namespace nosjob {

    
    /**
       A simple allocator which allocates Detail::RcValue<X> objects.

       RcType must be a Detail::RcValue<> type.

       This type's name comes from an older version which used a
       page-based allocator internally.
    */
    template <typename RcType>
    struct RcValuePagedAllocator
    {
    public:
        typedef RcType Type;
        typedef Type * PointerType;
    public:
        RcValuePagedAllocator(uint16_t/*ignored*/ = 0,int/*ignored*/ = 0)
//             : impl()
        {}
        PointerType alloc() const
        {
            return new Type;
        }
        void dealloc( PointerType v ) const
        {
            if( v ) v->decr();
        }
    };

}

#endif /* wanderinghorse_net_nosjob_allocator_HPP_INCLUDED */
