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
 *  bpcallback.h
 *
 *  Created by David Grigsby on 9/1/09.
 *  Copyright 2009 Yahoo! Inc. All rights reserved.
 *
 */
#ifndef BPCALLBACK_H_
#define BPCALLBACK_H_

#include "bptransaction.h"


namespace bplus {
namespace service {


class Callback
{
public:    

    // ctor
    Callback( const Transaction& tran, const bplus::CallBack& cb );

    // This ctor provided for caller convenience (e.g. when pulling
    // args from a bplus::Map).
    // Note the dynamic type of the provided argument must still be a
    // bplus::CallBack, else a std::bad_cast will be thrown.
    Callback( const Transaction& tran, const bplus::Object& cb );
    
    // Invoke the callback, passing it provided arg object.
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


inline
Callback::Callback( const Transaction& tran, const bplus::Object& cb ) :
    m_tran( tran ),
    m_cb( dynamic_cast<const bplus::CallBack&>( cb ) )
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
