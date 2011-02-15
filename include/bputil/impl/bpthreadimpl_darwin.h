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
 *  bpthreadimpl_darwin.h
 * 
 *  Inline implementation file for bpthread.h (darwin version).
 *
 *  Note: This file is included by bpthread.h.
 *        It is not intended for direct inclusion by client code.
 *       
 *  Written by Lloyd Hilaiel, 2005 & 2007
 */
#ifndef BPTHREADIMPLDARWIN_H_
#define BPTHREADIMPLDARWIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <assert.h>


namespace bplus {
namespace thread {


inline static void blockSignal(int cig)
{
  sigset_t ss;

  sigemptyset(&ss);
  sigaddset(&ss, cig);
  pthread_sigmask(SIG_BLOCK, &ss, NULL);
}

inline Thread::Thread()
{
    m_osSpecific = (void *) calloc(1, sizeof(pthread_t));
}

inline Thread::~Thread()
{
    free(m_osSpecific);
    m_osSpecific = NULL;
}

struct barDat_t {
  StartRoutine runme;
  void *arg;
};

inline static void *blockAndRun(void *arg1)
{
  struct barDat_t *bdptr = (struct barDat_t *) arg1;
  StartRoutine runme = bdptr->runme;
  void *arg = bdptr->arg;

  blockSignal(SIGINT);
  free(bdptr);

  return runme(arg);
}

inline bool
Thread::run(StartRoutine startFunc, void * cookie)
{
    int rsz = -1;
    struct barDat_t *bdptr;

    bdptr = (barDat_t *) malloc(sizeof(struct barDat_t));
    assert(bdptr != NULL);

    bdptr->runme = startFunc;
    bdptr->arg = cookie;
    rsz = pthread_create((pthread_t *) m_osSpecific, NULL,
                         blockAndRun, bdptr);

    if (rsz != 0) free(bdptr);

    return (rsz == 0);
}

inline void
Thread::detach()
{
    pthread_detach(*((pthread_t *) m_osSpecific));
}

inline void
Thread::join()
{
    pthread_join(*((pthread_t *) m_osSpecific), NULL);
}

inline unsigned int
Thread::ID()
{
    return (unsigned int) *((pthread_t *) m_osSpecific);
}

inline unsigned int
Thread::currentThreadID()
{
    return (unsigned int) pthread_self();
}


} // namespace thread
} // namespace bplus


#endif // BPTHREADIMPLDARWIN_H_

