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
 *  bpuuid.h
 *
 *  Created by David Grigsby on 4/29/08.
 *  Copyright 2008 Yahoo! Inc. All rights reserved.
 *
 */

#ifndef BPUUID_H_
#define BPUUID_H_

#include <string>

// NOTE:  all pathnames are UTF8 encoded std::strings

namespace bplus {
namespace uuid {

bool generate( std::string& sUuid );

}
}


// #include the inline implementations
#ifdef WIN32
#include "impl/bpuuidimpl_windows.h"
#else
#include "impl/bpuuidimpl_unix.h"
#endif


#endif // _BPUUID_H

