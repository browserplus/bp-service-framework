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
 *  bppathstring.h
 *
 *  Created by David Grigsby on 12/01/10.
 *  Copyright 2008 Yahoo! Inc. All rights reserved.
 *
 */

#ifndef BPPATHSTRING_H_
#define BPPATHSTRING_H_

#include <string>


namespace bplus {

#if defined(WIN32) || defined(WINDOWS) || defined(_WINDOWS)
//    typedef std::wstring tPathString;
    typedef std::string tPathString;
#else
    typedef std::string tPathString;
#endif

}

#endif // BPPATHSTRING_H_
