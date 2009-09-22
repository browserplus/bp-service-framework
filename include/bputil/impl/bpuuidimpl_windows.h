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

/**
 *  bpuuidimpl_windows.h
 * 
 *  Inline implementation file for bpuuid.h (windows version).
 *
 *  Note: This file is included by bpuuid.h.
 *        It is not intended for direct inclusion by client code.
 *       
 *  Created by David Grigsby on 4/29/08.
 *  
 *  Copyright 2008 Yahoo! Inc. All rights reserved.
 */
#ifndef BPUUIDIMPLWINDOWS_H_
#define BPUUIDIMPLWINDOWS_H_

#include <Rpc.h>

namespace bplus {
namespace uuid {


inline bool
generate( std::string& sUuid )
{
    UUID uuid;
    if (UuidCreate( &uuid ) != RPC_S_OK)
        return false;

    unsigned char* pch = NULL;
    if (UuidToStringA( &uuid, &pch ) != RPC_S_OK)
        return false;

    if (pch)
        sUuid = reinterpret_cast<char*>(pch);
    
    RpcStringFreeA( &pch );
    return true;
}

   
} // uuid
} // bplus


#endif // BPUUIDIMPLWINDOWS_H_
