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
 *  bpurl.h
 *
 *  Declares Url and related items.
 *
 *  Created by David Grigsby on 8/04/08.
 *  Copyright 2008 Yahoo! Inc. All rights reserved.
 *
 */
#ifndef BPURL_H_
#define BPURL_H_

#include <string>
//#include "bperrorutil.h"


namespace bplus {
namespace url {


// Returns whether the provided string is a well-formed file url.
bool isFileUrl( const std::string& sIn );

// Returns whether the provided string is a well-formed http or https url.
bool isHttpOrHttpsUrl( const std::string& sIn );

// Returns whether the provided string is a well-formed url.
bool isUrl( const std::string& sIn );

// Returns the well-known port for the specified scheme.
// Returns -1 for unrecognized scheme.
int portFromScheme( const std::string& sScheme );



/*
struct ParseError : public bp::error::Exception
{
    ParseError( const std::string& sDesc ) :  bp::error::Exception( sDesc ) {}   
};
*/

class Url 
{
// Construction/Destruction    
public:    
    // Default ctor - sets all fields to empty.
    Url();

    // Construct from a well-formed url string.
//  Url( const std::string& sUrl ); // throw ParseError


// Operations    
public:
    // Parse provided string into our fields.
    // Returns false on failure.
    bool        parse( const std::string& sIn );

    // Parse provided string into our fields.
    // Returns false and fills in sErr on failure.
    bool        parse( const std::string& sIn, std::string& sErr );
    
    // Return current state as a well-formed url string.
    std::string toString();

    // Return the path and query string portion of the url.
    std::string pathAndQueryString();

    // Return "host" if port == 80.
    // Return "host:port" otherwise.
    std::string friendlyHostPortString();
    
    // TODO: Lloyd and Dave should discuss these next two funcs:
    
	// Strip leaf node (if any) from the path.
	// Returns false if path empty at entry, true otherwise.
    bool        dirname();
    
    // Append a path to the url.
    // The inPath must not be empty().  If the inPath starts
    // with a '/' this is identical to a setPath call.
    // if the URL does not end in '/', then a "basename" operation
    // will be performed.
    // returns false on failure (bad input) 
    bool        appendPath( const std::string& inPath );

    
// Accesors
public:
    std::string scheme() const;
    void        setScheme( const std::string& sScheme );
    
    std::string host() const;
    void        setHost( const std::string& sHost );

    int         port() const;
    void        setPort( int nPort );

    std::string path() const;
    void        setPath( const std::string& sPath );

    std::string query() const;
    // Provided string should be appropriately url-encoded.
    // Do not prepend a '?'.
    void        setQuery( const std::string& sQuery );

    std::string frag() const;
    void        setFrag( const std::string& sFrag );

    
// State    
private:
    std::string m_sScheme;
    std::string m_sUser;
    std::string m_sPassword;
    std::string m_sHost;
    int         m_nPort;
    std::string m_sPath;    // contains any path segments and params
    std::string m_sQuery;
    std::string m_sFrag;
};


} // namespace url
} // namespace bp


//////////////////////////////////////////////////////////////////////
// Get the implementations.
#include "impl/bpurlimpl.h"


#endif

