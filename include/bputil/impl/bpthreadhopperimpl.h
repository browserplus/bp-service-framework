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
 *  Inline implementation file for bpthreadhopper.h.
 * 
 *  Note: This file is included by bpthreadhopper.h.
 *        It is not intended for direct inclusion by client code.
 *       
 * Written by Lloyd Hilaiel, Fri Jul  6 13:58:30 MDT 2007
 * (c) 2007, Yahoo! Inc, all rights reserved.
 */

#ifndef BPTHREADHOPPERIMPL_H_
#define BPTHREADHOPPERIMPL_H_



inline
bplus::thread::Hopper::Hopper()
    : m_osSpecific(NULL)
{
}


inline
bplus::thread::HoppingClass::HoppingClass() : m_hopper()
{
    m_hopper.initializeOnCurrentThread();
}

inline
bplus::thread::HoppingClass::~HoppingClass()
{
}

inline void
bplus::thread::HoppingClass::hoppingClassRelayFunc(void * ctx)
{
    HoppingClass * t = (HoppingClass *) (((void **)ctx)[0]);
    void * arg = ((void **)ctx)[1];
    t->onHop(arg);
    free(ctx);
}

inline void
bplus::thread::HoppingClass::hop(void * context)
{
    void ** args = (void **) calloc(2, sizeof(void *));
    args[0] = this;
    args[1] = context;
    m_hopper.invokeOnThread(hoppingClassRelayFunc, (void *) args);
}


#endif // BPTHREADHOPPERIMPL_H_
