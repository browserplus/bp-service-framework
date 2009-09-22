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
 *  bpstrutil.h
 *
 *  Created by Gordon Durand on 7/20/07.
 *  Copyright 2007 Yahoo! Inc. All rights reserved.
 *
 */

#ifndef BPSTRUTIL_H_
#define BPSTRUTIL_H_

#include <iomanip>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace bplus {
namespace strutil {

/**
 * Converts from UTF-8 to unicode.
 *
 * \returns converted string if successful, empty string otherwise. 
 */ 
std::wstring utf8ToWide(const std::string& sIn);

/**
 * Converts from unicode to UTF-8.
 *
 * \returns converted string if successful, empty string otherwise. 
 */ 
std::string wideToUtf8(const std::wstring& wsIn);

/**
 * return a copy of sIn with all sOld replaced by sNew.
 */
void replace(std::string& sTarget,
             const std::string& sOld,
             const std::string& sNew);

/**
 * split a string given the provided delimiter, returning a
 * vector of resultant strings
 * Notes:
 *   if 'str' is empty, return vector will contain one empty string.
 *   if 'str' starts with 'delim', first vector member will be empty.
 *   if 'str' ends with 'delim', last vector member will be empty.
 *   delim is treated as a substring to match.
 */
std::vector<std::string> split(const std::string& str,
                               const std::string& delim);

/**
 * split a string given the provided delimiter, returning a
 * vector of resultant strings.  The resultant strings are
 * trimmed of any leading and trailing whitespace.
 * Note: delim is treated as a set of delimiter characters.
 */
std::vector<std::string>
splitAndTrim(const std::string& str, const std::string& delim);

/**
 * join string vector elements into a string using the provided
 * separator between each element.
 */
std::string join( const std::vector<std::string>& vsIn,
                  const std::string& sSep );

/**
 * return an uppercase copy of sIn using default codepage.
 */
std::string toUpper(const std::string& sIn);

/**
 * Trims whitespace from the beginning and end of a string.
 */
std::string trim( const std::string& sSource );

/**
 * Trims whitespace from the beginning of a string.
 */
std::string trimLeft( const std::string& sSource );

/**
 * Trims whitespace from the end of a string.
 */
std::string trimRight( const std::string& sSource );

/**
 * return whether the provided strings are equal, ignoring case.
 */
bool isEqualNoCase( const std::string& s1,
                    const std::string& s2);

/**
 * return whether a string completely matches a wildcard
 * pattern containing '*' as the wildcard character
 */
// Disabled for the moment: requires boost on gcc.
//bool matchesWildcard(const std::string& s,
//                     const std::string& pattern);

/**
 * return a string corresponding to cszIn.
 * if cszIn is 0, return an empty string.
 */
inline std::string safeStr(const char* cszIn)
{
    return cszIn ? cszIn : "";
}

/** 
 * quote a JSON string
 */
std::string quoteJsonString(const std::string& str);

/**
 * Loads a string with the contents of the specified file.
 * Any previous contents of the string are discarded.
 * \param  sPath Full path to file
 * \param  sOut String to load
 * \return true iff successful.
 */
// Disable for the moment.  Coupling with file.
//bool loadFromFile(const std::string& sPath,
//                  std::string& sOut);

/**
 * Writes a string to the specified file.
 * If a file already exists at the path it will be deleted. 
 * \param  sPath Full path to file
 * \param  sIn String to store
 * \return true iff successful.
 */
// Disable for the moment.  Coupling with file.
//bool storeToFile(const std::string& sPath,
//                 const std::string& sIn);


} // namespace strutil
} // namespace bplus


// This is a manipulator you can apply to a stream so that numeric values
// will be represented in the form "0x1234abcd".
#define BP_HEX_MANIP std::hex << std::showbase


//////////////////////////////////////////////////////////////////////
// Get the implementations.
#include "impl/bpstrutilimpl.h"


#endif
