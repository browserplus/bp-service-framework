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
 *  bpthread.h -- POSIX like abstractions around OS threads.
 *
 *  Written by Lloyd Hilaiel, 2005 & 2007
 */

#ifndef BPTHREAD_H_
#define BPTHREAD_H_

namespace bplus {
namespace thread {


/** A pointer to the routine that the spawned thread will begin
 *  executing */
typedef void *(*StartRoutine) (void * cookie);

class Thread
{
  public:
    Thread();
    ~Thread();    

    bool run(StartRoutine startFunc, void * cookie);

    /** detach from the running thread, making it unjoinable */
    void detach();

    /** block until a thread completes */
    void join();

    /** get a numeric ID for the spawned thread */
    unsigned int ID();

    /** get a numeric ID for the current thread */
    static unsigned int currentThreadID();

  private:
    void * m_osSpecific;
};


} // namespace thread
} // namespace bplus


// #include the inline implementations
#ifdef WIN32
#include "impl/bpthreadimpl_windows.h"
#else
#include "impl/bpthreadimpl_darwin.h"
#endif


#endif // BPTHREAD_H_

