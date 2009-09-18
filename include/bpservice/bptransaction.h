/**
 * ***** BEGIN LICENSE BLOCK *****
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * The Original Code is BrowserPlus (tm).
 * 
 * The Initial Developer of the Original Code is Yahoo!.
 * Portions created by Yahoo! are Copyright (C) 2006-2009 Yahoo!.
 * All Rights Reserved.
 * 
 * Contributor(s): 
 * ***** END LICENSE BLOCK *****
 */

/*
 *  bptransaction.h
 *
 *  Created by David Grigsby on 9/1/09.
 *  Copyright 2009 Yahoo! Inc. All rights reserved.
 *
 */
#ifndef BPTRANSACTION_H_
#define BPTRANSACTION_H_

#include "bputil/bptypeutil.h"
#include "bpserviceapi/bpcfunctions.h"


namespace bplus {
namespace service {


//////////////////////////////////////////////////////////////////////
// Transaction
//
// Represents an active service transaction.
//
class Transaction
{
public:    
    // ctor
    Transaction( const BPCFunctionTable* pCoreFuncs, unsigned int tid );

public:
    // End the transaction, indicating success, with the specified result.
    void            complete( const bplus::Object& result ) const;

    // End the transaction, indicating failure.
    // Terse and verbose error descriptions may be provided.
    void            error( const char* szError = 0,
                           const char* szVerboseError = 0 ) const;

    // Invoke a callback back into the service's client.
    // cb:   The callback to call.
    //       Note: we accept type Object& for caller convenience, but
    //       this arg must be of type CallBack& at runtime.
    // args: The arguments to the callback.
    void            invokeCallback( const bplus::Object& cb,
                                    const bplus::Object& args ) const;

    // Prompt the user.
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
