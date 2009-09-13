//////////////////////////////
// BPTransaction.h
//
#ifndef BPTRANSACTION_H_
#define BPTRANSACTION_H_

#include "bputil/bptypeutil.h"
#include "bpserviceapi/bpcfunctions.h"    // from BrowserPlus Service SDK


namespace bplus {
namespace service {


class Transaction
{
public:    
    Transaction( const BPCFunctionTable* pCoreFuncs, unsigned int tid );

public:
    void            complete( const bplus::Object& result ) const;
    
    void            error( const char* szError = 0,
                           const char* szVerboseError = 0 ) const;

    /**
     * Invoke a callback back into the service's client.
     * \param cb The callback to call.
     *           Note: we accept type Object& for caller convenience, but
     *           this arg must be of type CallBack& at runtime.
     * \param args The arguments to the callback.
     */
    void            invokeCallback( const bplus::Object& cb,
                                    const bplus::Object& args ) const;
    
    unsigned int    promptUser( const std::string& sUtf8PathToHTMLDialog,
                                const bplus::Object& args,
                                BPUserResponseCallbackFuncPtr responseCallback,
                                void* context );
    
private:
    const BPCFunctionTable* m_pCoreFuncs;
    unsigned int            m_nTid;
};


inline
Transaction::Transaction( const BPCFunctionTable* pCoreFuncs,
                          unsigned int tid ) :
m_pCoreFuncs( pCoreFuncs ),
m_nTid( tid )
{
}


inline void
Transaction::complete( const bplus::Object& oResult ) const
{
    m_pCoreFuncs->postResults( m_nTid, oResult.elemPtr() );
}


inline void
Transaction::error( const char* szError,
                    const char* szVerboseError ) const
{
    m_pCoreFuncs->postError( m_nTid, szError, szVerboseError );
}


inline void
Transaction::invokeCallback( const bplus::Object& ocb,
                             const bplus::Object& args ) const
{
    if (ocb.type() != BPTCallBack) {
		m_pCoreFuncs->log( BP_ERROR, "Invalid callback reference." );
        return;
    }

    m_pCoreFuncs->invoke( m_nTid, CallBack(ocb).value(), args.elemPtr() );
}


inline unsigned int
Transaction::promptUser( const std::string& sUtf8PathToHTMLDialog,
                         const bplus::Object& args,
                         BPUserResponseCallbackFuncPtr responseCallback,
                         void* context )
{
    return m_pCoreFuncs->prompt( m_nTid, sUtf8PathToHTMLDialog.c_str(),
                                 args.elemPtr(), responseCallback, context );
}


} // service
} // bplus


#endif // BPTRANSACTION_H_
