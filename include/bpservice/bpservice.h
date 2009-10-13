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

#include "bpservicedescription.h"
#include "bptransaction.h"
#include "bpserviceapi/bppfunctions.h"


namespace bplus {
namespace service {


class Service
{
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
    
public:
    // Log to the bplus logging system.
    // Note: values for level are BP_[DEBUG|INFO|WARN|ERROR|FATAL],
    // declared in bpcfunctions.h.
    static void     log( unsigned int level,
                         const std::string& sLog );

    // Called when the service's dynamic library has been loaded, prior
    // to allocation of any instances.
    // Return: bool success code.
    // Derived services may override this method if desired.
    static bool     onServiceLoad();
    
    // Called when the service's dynamic library is about to be unloaded,
    // after destruction of all instances.
    // Return: bool success code.
    // Derived services may override this method if desired.
    static bool     onServiceUnload();
    
    // Returns service name in the form: "name version".
    static std::string fullName();
            
    // Get contextual information for this instance.  See bppfunctions.h.
    // Note: Do not call this from derived service's constructor.
    //       May be used from finalConstruct or any other service method.
    const std::map<std::string,std::string>& context();

    // Get a particular value from the context map.
    // Returns empty string if key not found.
    // Note: Do not call this from derived service's constructor.
    //       May be used from finalConstruct or any other service method.
    std::string     context( const std::string& key );
    
private:    
    static const BPCoreletDefinition*
                    bppInitialize( const BPCFunctionTable* pCoreFuncs,
                                   const BPElement* pParamMap );

    static void     bppShutdown();

    static int      bppAllocate( void** instance,
                                 unsigned int attachID,
                                 const BPElement* contextMap );

    static void     bppDestroy( void* pInstance );

    static void     bppInvoke( void* pInstance,
                               const char* cszFuncName,
                               unsigned int tid,
                               const BPElement* pArgs );

    static void     setupDescription();
    
    static Service* createInstance();

    void            setContext( const BPElement* pContext );

    virtual void    invoke( const char* cszFuncName,
                            const Transaction& tran,
                            const bplus::Map& args ) = 0;
    
// Had to be public, but meant for internal use.    
public:    
    static const BPPFunctionTable* getEntryPoints();
    
// Class-wide State
protected:
    static const BPCFunctionTable*  s_pCoreFuncs;
    static const BPElement*         s_pParamMap;
    static Description              s_description;
    
// Instance-specific State    
private:    
    std::map<std::string,std::string>   m_mapContext;
    
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
