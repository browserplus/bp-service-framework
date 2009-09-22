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
 *  bpthreadimpl_windows.h
 * 
 *  Inline implementation file for bpthread.h (windows version).
 *
 *  Note: This file is included by bpthread.h.
 *        It is not intended for direct inclusion by client code.
 *       
 *  Written by Lloyd Hilaiel, 2005 & 2007
 */
#ifndef BPTHREADIMPLWINDOWS_H_
#define BPTHREADIMPLWINDOWS_H_

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>


namespace bplus {
namespace thread {


struct WinThreadData {
  HANDLE thrHndl;
  DWORD id;
};


inline Thread::Thread()
{
    m_osSpecific = (void *) calloc(1, sizeof(WinThreadData));
}

inline Thread::~Thread()
{
    free(m_osSpecific);
    m_osSpecific = NULL;
}

inline bool
Thread::run(StartRoutine startFunc, void * cookie)
{
    WinThreadData * td = (WinThreadData *) m_osSpecific;

    td->thrHndl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) startFunc, cookie, 0,
                               &(td->id));

    if (!td->thrHndl) return false;
    
    return true;
}

inline void
Thread::detach()
{
    WinThreadData * td = (WinThreadData *) m_osSpecific;
    CloseHandle(td->thrHndl);
}

inline void
Thread::join()
{
    WinThreadData * td = (WinThreadData *) m_osSpecific;
    WaitForSingleObject(td->thrHndl, INFINITE);
    CloseHandle(td->thrHndl);
}

inline unsigned int
Thread::ID()
{
    WinThreadData * td = (WinThreadData *) m_osSpecific;
    return td->id;
}

inline unsigned int
Thread::currentThreadID()
{
    return (unsigned int) GetCurrentThreadId();
}


} // namespace thread
} // namespace bplus


#endif // BPTHREADIMPL_WINDOWS_H_
