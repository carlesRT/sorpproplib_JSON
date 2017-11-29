#include "wh/nosjob/nosjob.hpp"

namespace nosjob {
    const Boolean Boolean::True = Boolean( true );
    const Boolean Boolean::False = Boolean( false );

    //! AtomAPI::Compare impl for Boolean
    static int Compare_Boolean(Atom const & lhs, Atom const & rhs)
    {
        assertAtomType( lhs, TypeID::Boolean );
        bool const l = lhs.boolValue();
        const bool r = rhs.boolValue();
        if( l < r ) return -1;
        else if( l == r ) return 0;
        else return 1;
    }
    
    class Boolean::APIImpl : public AtomAPI
    {
        APIImpl()
            : AtomAPI( Boolean::AtomTypeID,
                       AtomAPI::CopyValue_copy_ptr,
                       0,
                       AtomAPI::ToBool_voidptr,
                       Compare_Boolean )
        {}
    public:
        static AtomAPI const & instance()
        {
            static const APIImpl bob;
            return bob;
        }
    };
    AtomAPI const * Boolean::API()
    {
        return &Boolean::APIImpl::instance();
    }

    Boolean::Boolean() : Atom( Boolean::API() )
    {
    }

    void Boolean::setValue( bool b )
    {
        this->m_data = b ? this : 0;//reinterpret_cast<void *>(b ? 1 : 0);
    }

    Boolean::Boolean( bool v )
        : Atom(
               &Boolean::APIImpl::instance()
               //Boolean::API /* WTF IS THIS NULL??? */
               )
    {
        this->setValue(v);
    }

    Boolean::Boolean( Boolean const & v )
        : Atom( Boolean::API() )
    {
        m_data = v.m_data;
    }

    Boolean::Boolean( Atom const & v )
        : Atom( Boolean::API() )
    {
        this->setValue( v.boolValue() );
    }
    Boolean::~Boolean()
    {
    }
    Boolean & Boolean::operator=( Boolean const & v )
    {
        this->m_data = v.m_data;
        return *this;
    }

    Boolean & Boolean::operator=( bool v )
    {
        this->m_data = reinterpret_cast<void *>(v ? 1 : 0);
        return *this;
    }

    bool Boolean::value() const
    {
        return this->boolValue();
    }

    bool Boolean::isBoolean( Atom const & a )
    {
        return (AtomTypeID == a.typeID())
            && (API() == a.api());
    }
    
    Boolean Boolean::cast( Atom const & a )
    {
        return Boolean( a.boolValue() );
    }

    StringType AtomToString<Boolean>::Value( Boolean const & src )
    {
        return src.boolValue() ? StaticStrings.True : StaticStrings.False;
    }

    std::ostream & operator<<( std::ostream & os, Boolean const & a )
    {
        return os << a.value();
    }

}
