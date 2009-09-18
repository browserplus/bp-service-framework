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
    Service() {}

    // This method is called immediately after the instance has been
    // fully constructed and initialized.
    // Derived services can implement this method.
    // Some base members/methods are not accessible until this is called,
    // e.g. context().
    virtual void finalConstruct() {}
    
    virtual ~Service() {}
    
public:
    // Note: values for level are BP_[DEBUG|INFO|WARN|ERROR|FATAL],
    // declared in bpcfunctions.h.
    static void     log( unsigned int level,
                         const std::string& sLog );
    
    // Get contextual information for this instance.  See bppfunctions.h.
    // Note: do not access this prior to invokation of finalConstruct().
    const std::map<std::string,std::string>& context();

    // Get a particular value from the context map.
    // Returns empty string key not found.
    // Note: do not access this prior to invokation of finalConstruct().
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

    static const BPCoreletDefinition* getDescription();

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



////////////////////////////////////////////////////////////////////////////////
// Service Implementation
//

inline const std::map<std::string,std::string>&
Service::context()
{
    return m_mapContext;
}

   
inline std::string
Service::context( const std::string& key )
{
    std::map<std::string, std::string>::iterator it = m_mapContext.find( key );
    return it == m_mapContext.end() ? "" : it->second;
}


inline void
Service::log( unsigned int level,
              const std::string& sLog )
{
    if (s_pCoreFuncs->log) {
        s_pCoreFuncs->log( level, "%s", sLog.c_str() );
    }
}


inline const BPPFunctionTable*
Service::getEntryPoints()
{
    static BPPFunctionTable s_functionTable =
    {
        BPP_CORELET_API_VERSION,
        bppInitialize,
        bppShutdown,
        bppAllocate,
        bppDestroy,
        bppInvoke,
        0, 0
    };

    return &s_functionTable;
}


inline const BPCoreletDefinition*
Service::bppInitialize( const BPCFunctionTable* pCoreFuncs,
                        const BPElement* pParamMap )
{
    s_pCoreFuncs = pCoreFuncs;
    s_pParamMap = pParamMap;

    return getDescription();
}


inline void
Service::bppShutdown()
{

}


inline int
Service::bppAllocate( void** instance,
                      unsigned int attachID,
                      const BPElement* pContext )
{
    Service* pInst = createInstance();
    pInst->setContext( pContext );
    pInst->finalConstruct();
    
    *instance = (void*) pInst;

    return 0;
}


inline void
Service::bppDestroy( void* pInstance )
{
    delete (Service*) pInstance;
}


inline void
Service::bppInvoke( void* pvInst,
                    const char* cszFuncName,
                    unsigned int tid,
                    const BPElement* pArgs )
{
    try
    {
        Transaction tran( s_pCoreFuncs, tid );

        std::auto_ptr<bplus::Object> poArgs( bplus::Object::build( pArgs ) );
        bplus::Map* pmArgs = dynamic_cast<bplus::Map*>( poArgs.get() );
        
        // Always give invoke() a map.
        bplus::Map mapEmpty;
        const bplus::Map& mapref = pmArgs ? *pmArgs : mapEmpty;
        
        ((Service*) pvInst)->invoke( cszFuncName, tran, mapref );
    }
    catch (bplus::ConversionException& /*exc*/ )
    {
        s_pCoreFuncs->postError( tid, "invalid input", "conversion exception" );
        return;
    }
    // TODO: other catch's could possibly go here
}


inline void
Service::setContext( const BPElement* peCtx )
{
    std::auto_ptr<bplus::Object> ptrCtx( bplus::Object::build( peCtx ) );

    bplus::Map* pmCtx = dynamic_cast<bplus::Map*>( ptrCtx.get() );
    if (!pmCtx) {
        return;
    }

    typedef std::map<std::string, const bplus::Object*> tStrObjMap;
    tStrObjMap mapCtx = *pmCtx;
    for (tStrObjMap::const_iterator it = mapCtx.begin(); it != mapCtx.end();
         ++it)
    {
        // Only add string elements.
        if (it->second->type() == BPTString) {
			m_mapContext.insert( std::make_pair( it->first, std::string(*it->second)) );
        }
    }
}


} // service
} // bplus



////////////////////////////////////////////////////////////////////////////////
// Macros
//

#define BP_SERVICE_DEFNS( className ) \
extern "C" \
{ \
    const BPPFunctionTable* BPPGetEntryPoints() \
    { \
        return bplus::service::Service::getEntryPoints(); \
    } \
} \
\
const BPCFunctionTable* bplus::service::Service::s_pCoreFuncs = NULL; \
const BPElement* bplus::service::Service::s_pParamMap = NULL; \
bplus::service::Description bplus::service::Service::s_description; \
\
bplus::service::Service* bplus::service::Service::createInstance() \
{ \
    return new className(); \
} \
\
typedef void (className::* tInvokableFunc)( const bplus::service::Transaction& tran, \
                                            const bplus::Map& args );\
std::map<std::string, tInvokableFunc> className::s_mapFuncs;



#define BP_SERVICE_DESC( className, serviceName, version, docString ) \
BP_SERVICE_DEFNS( className ); \
const BPCoreletDefinition* bplus::service::Service::getDescription() \
{ \
    s_description.clear(); \
    s_description.setName( serviceName ); \
    s_description.setVersion( version ); \
    s_description.setDocString( docString );


#define ADD_BP_METHOD( className, funcName, docString ) \
{ \
    bplus::service::Function func; \
    func.setName( #funcName ); \
    func.setDocString( docString ); \
    s_description.addFunction( func ); \
    className::s_mapFuncs[#funcName] = &className::funcName; \
}


#define ADD_BP_METHOD_ARG( func, argName, argType, reqd, docString ) \
{ \
    bplus::service::Argument a( argName, bplus::service::Argument::argType ); \
    a.setRequired( reqd ); \
    a.setDocString( docString ); \
    bplus::service::Function* pFunc = s_description.getFunction( #func ); \
    if (pFunc) { \
        pFunc->addArgument( a ); \
    } \
}


#define END_BP_SERVICE_DESC \
    return s_description.toBPCoreletDefinition(); \
}


#define BP_SERVICE( className ) \
typedef void (className::* tInvokableFunc)( const bplus::service::Transaction& tran, \
                                            const bplus::Map& args ); \
static std::map<std::string, tInvokableFunc> s_mapFuncs; \
\
void invoke( const char* cszFuncName, \
             const bplus::service::Transaction& tran, \
             const bplus::Map& args ) \
{ \
    std::map<std::string, tInvokableFunc>::iterator it; \
    it = s_mapFuncs.find( cszFuncName ); \
    if (it == s_mapFuncs.end()) { \
        tran.error( "invalid input", "method does not exist" ); \
        return; \
    } \
    tInvokableFunc func = it->second; \
    (this->*func)( tran, args ); \
}


#endif // BPSERVICE_H_
