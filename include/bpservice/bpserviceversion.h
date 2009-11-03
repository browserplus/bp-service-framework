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
 *  BPServiceVersion.h
 *
 *  Implements a Service Version abstraction.
 *
 *  Copyright 2008 Yahoo! Inc. All rights reserved.
 *
 */
#ifndef BPSERVICEVERSION_H_
#define BPSERVICEVERSION_H_

#include <string>

#ifdef WIN32
#define snprintf _snprintf_s
#endif


namespace bplus {
namespace service {


/**
 * A Class for representing, comparing and parsing service versions
 */ 
class Version
{
public:
    Version();
    Version(const char * in);  // may throw
    virtual ~Version();    

    Version& operator=(const Version & in);

    bool parse(const char * version);
    bool parse(const std::string & version);

    int majorVer(void) const;
    void setMajor(int majorVersion);

    int minorVer(void) const;
    void setMinor(int minorVersion);

    int microVer(void) const;
    void setMicro(int microVersion);

    /** -1 if this version is less than other version
     *  0 if this version is equal to other version
     *  1 if this version is greater than other version
     */
    int compare(const Version & otherVersion) const;

    /** do the two versions match, considering negative version numbers
     *  to be wildcards */
    bool match(const Version & otherVersion) const;

    std::string asString(void);


    /**
     * A very specific, but common test which occurs all over.
     * When scanning a list of available corelets, how do we tell if
     * the 'current' version both matches the wanted 'version' and
     * 'minversion', AND is newer than the match we've already 'got'
     */
    static bool isNewerMatch(const Version &current,
                             const Version &got,
                             const Version &wantver,
                             const Version &wantminver);
private:
    int m_major;
    int m_minor;
    int m_micro;
};



//////////////////////////////////////////////////////////////////////
// Version Implementation
//

inline Version::Version()
: m_major(-1), m_minor(-1), m_micro(-1)
{
}

inline Version::Version(const char * in)
: m_major(-1), m_minor(-1), m_micro(-1)
{
    if (!parse(in))
    {
        // TODO: perhaps throw a service exception.
        throw( std::string("invalid argument") );
    }
}


inline Version::~Version()
{
}

inline Version&
Version::operator=(const Version& in)
{
    m_major = in.m_major;
    m_minor = in.m_minor;    
    m_micro = in.m_micro;
    return *this;
}

inline int Version::majorVer() const
{
    return m_major;
}

inline void Version::setMajor(int major)
{
    m_major = major;
}

inline int Version::minorVer() const
{
    return m_minor;
}

inline void Version::setMinor(int minor)
{
    m_minor = minor;
}

inline int Version::microVer() const
{
    return m_micro;
}

inline void Version::setMicro(int micro)
{
    m_micro = micro;
}

inline int Version::compare(const Version & other) const
{
    int rv = 0;

    // compare major
    rv = (m_major > other.m_major) ? 1 : ((m_major < other.m_major) ? -1 : 0);

    // compare minor
    if (rv == 0) {
        rv = (m_minor > other.m_minor) ? 1 :
             ((m_minor < other.m_minor) ? -1 : 0);
    }

    // compare micro
    if (rv == 0) {
        rv = (m_micro > other.m_micro) ? 1 :
             ((m_micro < other.m_micro) ? -1 : 0);
    }

    return rv;
}

inline bool Version::match(const Version & other) const
{
    if (m_major >= 0 && other.m_major >= 0 && m_major != other.m_major)
    {
        return false;
    }

    if (m_minor >= 0 && other.m_minor >= 0 && m_minor != other.m_minor)
    {
        return false;
    }

    if (m_micro >= 0 && other.m_micro >= 0 && m_micro != other.m_micro)
    {
        return false;
    }

    return true;
}

#define CV_END 0
#define CV_DOT 1
#define CV_NUMBER 2
#define CV_ERROR 3
inline unsigned int cv_lex(const char * str, int & off)
{
    switch (str[off])
    {
        case 0: return CV_END;
        case '.': off++; return CV_DOT;
        case '0': case '1': case '2': case '3': case '4':  
        case '5': case '6': case '7': case '8': case '9':   
            while (str[off] <= '9' && str[off] >= '0') off++;
            return CV_NUMBER;
    }
    return CV_ERROR;
}

inline bool Version::parse(const char * version)
{
    if (version == NULL || !strlen(version)) return true;

    int off, nxt, rv;
    off = nxt = 0;

    // parse through the string

    // major version
    rv = cv_lex(version, nxt);
    if (rv != CV_NUMBER) return false;
    m_major = atoi(version);

    // dot
    rv = cv_lex(version, nxt);
    if (rv != CV_END && rv != CV_DOT) return false;

    if (rv != CV_END)
    {
        // minor version
        off = nxt;
        rv = cv_lex(version, nxt);
        if (rv != CV_NUMBER) return false;
        m_minor = atoi(version + off);

        // dot
        rv = cv_lex(version, nxt);
        if (rv != CV_END && rv != CV_DOT) return false;

        if (rv != CV_END)
        {
            // micro version
            off = nxt;
            rv = cv_lex(version, nxt);
            if (rv != CV_NUMBER) return false;
            m_micro = atoi(version + off);
        }
    }

    return true;
}

inline bool Version::parse(const std::string & version)
{
    return parse(version.c_str());
}


inline std::string Version::asString(void)
{
    const int bufSize = 31;
    char buf[bufSize];
    std::string str;
    if (m_major >= 0) {
        snprintf(buf, bufSize, "%d", m_major);
        str.append(buf);

        if (m_minor >= 0) {
            str.append(".");
            snprintf(buf, bufSize, "%d", m_minor);
            str.append(buf);    

            if (m_micro >= 0) {
                str.append(".");
                snprintf(buf, bufSize, "%d", m_micro);
                str.append(buf);
            }
        }
    }

    return str;
}

inline bool Version::isNewerMatch(const Version &current,
                                  const Version &got,
                                  const Version &wantver,
                                  const Version &wantminver)
{
    // For 'current' to be a winner it must:
    // 1. be newer than what we've already 'got'
    // 2. match the wantver
    // 3. be greater than the minver

    // (1)
    if (current.compare(got) < 0) return false;

    // (2)
    if (!current.match(wantver)) return false;

    // (3)
    if (current.compare(wantminver) < 0) return false;

    return true;
}



} // service
} // bplus


#endif // BPSERVICEVERSION_H_
