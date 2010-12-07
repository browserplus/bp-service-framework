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
 *  bpservice.h
 *
 *  Created by David Grigsby on 9/1/09.
 *  Copyright 2009 Yahoo! Inc. All rights reserved.
 *
 */
#ifndef BPSERVICE_H_
#define BPSERVICE_H_

#include "bpserviceapi/bppfunctions.h"
#include "bpservicedescription.h"
#include "bptransaction.h"
#include "bputil/bppathstring.h"

namespace bplus {
namespace service {


// Design Notes
// B+ differentiates between a *service* and *instances* on the service.
// The service corresponds to the service dll which is loaded by a B+
// "harness" process.  Only one copy of a particular service dll is
// loaded at a time, regardless of the number of clients.
// Instances are allocated for each client of the service, e.g. for
// each browser page or iframe that uses the service.
// For maximum ease of development for service authors, this framework blurs the
// service/instance distinction somewhat.  There is no "Instance" class.
// Static members of this class represent operations and state for the
// *service* and non-static members represent operations and state for
// an *instance*.

class Service
{
// Construction/Destruction    
public:
    // ctor
    Service() {}

    // Additional initialization.
    // This method is called immediately after the instance has been
    // default constructed and initialized.
    // Derived services may override this method if desired.
    virtual void finalConstruct() {}

    // dtor
    virtual ~Service() {}

// Class-scope facilities    
public:
    // Log to the bplus logging system.
    // Note: values for level are BP_[DEBUG|INFO|WARN|ERROR|FATAL],
    // declared in bpcfunctions.h.
    static void         log( unsigned int level,
                             const std::string& sLog );

    // Returns service name in the form: "name version".
    static std::string  fullName();

// Methods to access intance-specific attributes
// Note: Do not call these from constructor of classes derived from
//       this class - use finalConstruct for that purpose.
public:
    const std::string&  clientUri();
    const tPathString&  serviceDir();
    const tPathString&  dataDir();
    const tPathString&  tempDir();
    const std::string&  locale();
    const std::string&  userAgent();
    int                 clientPid();

// Overridable Methods
protected:
    // Called when the service's dynamic library has been loaded, prior
    // to allocation of any instances.
    // Return: bool success code.
    // Derived services may override this method if desired.
    static bool     onInitialize();
    
    // Called when the service's dynamic library is about to be unloaded,
    // after destruction of all instances.
    // Return: bool success code.
    // Derived services may override this method if desired.
    static bool     onShutdown();

    static int      onInstall(const BPPath serviceDir, const BPPath dataDir);

    static int      onUninstall(const BPPath serviceDir, const BPPath dataDir);

// Class-scope Internal Methods    
private:    
    static const BPServiceDefinition*
                    bppInitialize( const BPCFunctionTable* pCoreFuncs,
                                   const BPPath serviceDir,
                                   const BPPath dependentDir,
                                   const BPElement* pDependentParams );

    static void     bppShutdown();

    static int      bppAllocate( void** instance,
                                 const BPString uri, const BPPath serviceDir,
                                 const BPPath dataDir, const BPPath tempDir,
                                 const BPString locale,
                                 const BPString userAgent, int clientPid );

    static void     bppDestroy( void* pInstance );

    static void     bppInvoke( void* pInstance,
                               const char* cszFuncName,
                               unsigned int tid,
                               const BPElement* pArgs );

    static void     bppCancel(void* instance, unsigned int tid);

    static int      bppInstall(const BPPath serviceDir, const BPPath dataDir);

    static int      bppUninstall(const BPPath serviceDir, const BPPath dataDir);
    
    static void     setupDescription();
    
    static Service* createInstance();

    static bool     callInitializeHook();

    static bool     callShutdownHook();

    static int      callInstallHook(const BPPath serviceDir,
                                    const BPPath dataDir);

    static int      callUninstallHook(const BPPath serviceDir,
                                      const BPPath dataDir);
    
// Had to be public, but only for internal use.    
public:    
    static const BPPFunctionTable*  getEntryPoints();
    
// Class-wide State
protected:
    static const BPCFunctionTable*  s_pCoreFuncs;
    static tPathString              s_serviceDir;
    static tPathString              s_dependentDir;
    static bplus::Object*           s_pDependentParams;
    static Description              s_description;

// Internal Methods
private:
    virtual void    invoke( const char* cszFuncName,
                            const Transaction& tran,
                            const bplus::Map& args ) = 0;

// Instance-specific State    
private:    
    std::string     m_clientUri;
    tPathString     m_serviceDir;
    tPathString     m_dataDir;
    tPathString     m_tempDir;
    std::string     m_locale;
    std::string     m_userAgent;
    int             m_clientPid;
    
// Prevent copying
private:
    Service( const Service& );
    Service& operator=( const Service& );
};


} // service
} // bplus


////////////////////////////////////////////////////////////////////////////////
// Get the implementations.
#include "impl/bpserviceimpl.h"


#endif // BPSERVICE_H_
