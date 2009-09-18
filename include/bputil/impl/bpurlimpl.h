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
 *  bpurlimpl.h
 * 
 *  Inline implementation file for bpurl.h.
 *
 *  Note: This file is included by bpurl.h.
 *        It is not intended for direct inclusion by client code.
 *
 *  Implements the Url class and related items.
 *
 *  Created by David Grigsby on 8/05/08.
 *  Portions based on code by Lloyd Hilaiel
 *  
 *  Copyright 2008 Yahoo! Inc. All rights reserved.
 */
#ifndef BPURLIMPL_H_
#define BPURLIMPL_H_

#include <sstream>


namespace bplus {
namespace url {


inline bool isFileUrl( const std::string& sIn )
{
    Url url;
    return url.parse( sIn ) && url.scheme() == "file";
}


inline bool isHttpOrHttpsUrl( const std::string& sIn )
{
    Url url;
    return url.parse( sIn ) &&
           (url.scheme() == "http" || url.scheme() == "https");
}


inline bool isUrl( const std::string& sIn )
{
    Url url;
    return url.parse( sIn );
}


inline int portFromScheme( const std::string& sScheme ) 
{
    // TODO: build a table here to handle more than http!
    if (sScheme == "http")
        return 80;
    else if (sScheme == "https" )
        return 443;
    else
        return -1;
}


inline Url::Url() :
    m_sScheme(),
    m_sUser(),
    m_sPassword(),
    m_sHost(),
    m_nPort(),
    m_sPath(),
    m_sQuery(),
    m_sFrag()
{
    
}    


/*
inline Url::Url( const std::string& sUrl ) :
    m_sScheme(),
    m_sUser(),
    m_sPassword(),
    m_sHost(),
    m_nPort(),
    m_sPath(),
    m_sQuery(),
    m_sFrag()
{
    std::string sErr;
    if (!parse( sUrl, sErr ))
    {
        BP_THROW_TYPE( ParseError, sErr );
    }
}    
*/

inline bool Url::parse( const std::string& sIn )
{
    std::string sErr;
    return parse( sIn, sErr );
}


// See: http://www.freesoft.org/CIE/RFC/1808/4.htm
inline bool Url::parse( const std::string& sIn, std::string& sErr )
{
    m_sScheme.clear();
    m_sUser.clear();
    m_sPassword.clear();
    m_sHost.clear();
    m_nPort = -1;
    m_sPath.clear();
    m_sQuery.clear();
    m_sFrag.clear();
    
    if (sIn.empty())
    {
        sErr = "empty input";
        return false;
    }

    int port = -1;
    const char* x;
    const char* buf = sIn.c_str();
    const char* start = buf;

    /* first a scheme */
    x = buf;

    /* scheme = 1*( alpha | digit | "+" | "-" | "." ) */
    while (*x && (isalnum(*x) || *x == '+' || *x == '-' || *x == '.'))
        x++;

    if (*x != ':')
    {
        sErr = "expect ':' after string";
        return false;
    }

    m_sScheme = sIn.substr(buf - start, x - buf);

    buf = ++x;
    if (strncmp(x, "//", strlen("//")) != 0)
    {
        sErr = "expect '//'";
        return false;
    }
    x += strlen("//");

    buf = x;
    x += strcspn(x, "/:");

    if (*x == ':')
    {
        m_sHost = sIn.substr(buf - start, x - buf);

        buf = ++x;
        while (isdigit(*x))
            x++;
        if ((x - buf) == 0 || (x - buf) > 5)
        {
            sErr = "port is malformed";
            return false;
        }

        port = atoi(buf);
        if (port > (unsigned short int) -1)
        {
            sErr = "port is out of range";
            return false;
        }
    }
    
    if (*x)
    {
        if (*x != '/')
        {
            sErr = "expect '/' after host[:port]";
            return false;
        }
        
        if (m_sHost.empty()) 
            m_sHost = sIn.substr(buf - start, x - buf);
        buf = x;
    }
    else
    {
        if (m_sHost.empty())
            m_sHost = sIn.substr(buf - start);
        buf = x;
    }
    m_sPath.append(sIn.substr(buf - start));

    m_nPort = (port != -1) ? port : portFromScheme(m_sScheme);

    return true;
}


inline std::string Url::toString()
{
    std::string url;

    // scheme
    url.append(m_sScheme.empty() ? "none" : m_sScheme);

    // XXX: what's the danger of assuming resource specifier is always '://'?
    url.append("://");

    if (!m_sHost.empty())
        url.append(m_sHost);

    if (m_nPort != -1 &&
        (m_sScheme.empty() || m_nPort != (int) portFromScheme(m_sScheme)))
    {
        std::stringstream ss;
        ss << ":" << m_nPort;
        url.append(ss.str());
    }

    url.append(pathAndQueryString());
    
    return url;
}


inline std::string Url::pathAndQueryString()
{
    std::string sRet = m_sPath;

    // Ensure we have a leading "/".
    if (sRet.empty() || sRet[0] != '/')
    {
        sRet = std::string("/") + sRet;
    }
    
    // Handle query string if present.
    if (!m_sQuery.empty())
    {
        sRet += "?" + m_sQuery;
    }

    return sRet;
}


inline std::string Url::friendlyHostPortString()
{
    std::stringstream ss;
    ss << host();
    if (port() != 80)
    {
        ss << ":" << port();
    }

    return ss.str();
}


inline bool Url::dirname()
{
    if (m_sPath.empty())
        return false;

    if (m_sPath[m_sPath.length() - 1] == '/')
    {
        m_sPath.erase(m_sPath.length() - 1);
    }
    
    int x = m_sPath.find_last_of('/');
    if (x > 0) {
        m_sPath.erase(x+1);
    } else {
        m_sPath.clear();
    }
    return true;
}


inline bool Url::appendPath( const std::string & inPath )
{
    if (inPath.empty())
        return false;
    
    if (inPath[0] == '/')
    {
        setPath( inPath );
    }
    else
    {
        if (m_sPath.empty())
        {
            m_sPath.append( "/" );
        }
        else if (m_sPath[m_sPath.length() - 1] != '/')
        {
            dirname();
        }
        else
        {

        }

        m_sPath.append( inPath );
    }
    
    return true;
}


inline std::string Url::scheme() const
{
    return m_sScheme;
}


inline void Url::setScheme( const std::string& sScheme )
{
    m_sScheme = sScheme;
}


inline std::string Url::host() const
{
    return m_sHost;
}


inline void Url::setHost( const std::string& sHost )
{
    m_sHost = sHost;
}


inline int Url::port() const
{
    return m_nPort;
}


inline void Url::setPort( int nPort )
{
    m_nPort = nPort;
}


inline std::string Url::path() const
{
    return m_sPath;
}


inline void Url::setPath( const std::string& sPath )
{
    m_sPath = sPath;
}


inline std::string Url::query() const
{
    return m_sQuery;
}


inline void Url::setQuery( const std::string& sQuery )
{
    m_sQuery = sQuery;
}


inline std::string Url::frag() const
{
    return m_sFrag;
}


inline void Url::setFrag( const std::string& sFrag )
{
    m_sFrag = sFrag;
}


} // namespace url
} // namespace bp


#endif // BPUTLIMPL_H_
