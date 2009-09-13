//////////////////////////////
// bpcallback.h
//
#ifndef BPCALLBACK_H_
#define BPCALLBACK_H_

#include "bptransaction.h"


namespace bplus {
namespace service {


class Callback
{
public:    
    /**
     * \param tran The transaction
     * \param cb The callback method to call.
     *           Note: we accept type Object& for caller convenience, but
     *           this arg must be of type bplus::CallBack& at runtime.
     */
    Callback( const Transaction& tran, const bplus::CallBack& cb );

    void invoke( const bplus::Object& args ) const;

private:
    Transaction      m_tran;
    bplus::CallBack  m_cb;
};


inline
Callback::Callback( const Transaction& tran, const bplus::CallBack& cb ) :
    m_tran( tran ),
    m_cb( cb )
{
}


inline void
Callback::invoke( const bplus::Object& oArgs ) const
{
    m_tran.invokeCallback( m_cb, oArgs );
}


} // service
} // bplus


#endif
