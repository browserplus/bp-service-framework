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
//#include "bpserviceapi/bptypes.h"
//#include "bputil/bpstrutil.h"


namespace bplus {

/*
class PathString
{
public:
    explicit PathString( const std::string& sPath ) : m_sPath(sPath) {}
    
    explicit PathString( const BPPath path )
    {
#if defined(WIN32) || defined(WINDOWS) || defined(_WINDOWS)
        m_sPath = strutil::wideToUtf8(path);
#else
        m_sPath = path;
#endif
    }

    const BPPath asBPPath()
    {
#if defined(WIN32) || defined(WINDOWS) || defined(_WINDOWS)
        return const_cast<BPPath>(strutil::utf8ToWide(m_sPath).c_str());
#else
        return m_sPath.c_str();
#endif
    }
        
private:    
    std::string m_sPath;
};
*/

#if defined(WIN32) || defined(WINDOWS) || defined(_WINDOWS)
    typedef std::wstring tPathString;
#else
    typedef std::string tPathString;
#endif

}

#endif // BPPATHSTRING_H_
