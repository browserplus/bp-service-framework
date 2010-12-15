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
 *  Inline implementation file for bpservice.h.
 * 
 *  Note: This file is included by bpservice.h.
 *        It is not intended for direct inclusion by client code.
 *       
 */
#ifndef BPSERVICEIMPL_H_
#define BPSERVICEIMPL_H_

namespace bplus {
namespace service {


inline void
Service::log( unsigned int level,
              const std::string& sIn )
{
    if (s_pCoreFuncs->log) {
        s_pCoreFuncs->log( level, "%s", sIn.c_str() );
    }
}


inline std::string Service::fullName()
{
    return s_description.name() + " " + s_description.versionString();
}


inline const std::string&
Service::clientUri()
{
    return m_clientUri;
}
   
inline const bplus::tPathString&
Service::serviceDir()
{
    return m_serviceDir;
}


inline const bplus::tPathString&
Service::dataDir()
{
    return m_dataDir;
}


inline const bplus::tPathString&
Service::tempDir()
{
    return m_tempDir;
}


inline const std::string&
Service::locale()
{
    return m_locale;
}


inline const std::string&
Service::userAgent()
{
    return m_userAgent;
}


inline int
Service::clientPid()
{
    return m_clientPid;
}





// Default no-op implementation
inline bool
Service::onInitialize()
{
    return true;
}


// Default no-op implementation
inline bool
Service::onShutdown()
{
    return true;
}


// Default no-op implementation
inline int
Service::onInstall(const BPPath serviceDir, const BPPath dataDir)
{
    return 0;
}


// Default no-op implementation
inline int
Service::onUninstall(const BPPath serviceDir, const BPPath dataDir)
{
    return 0;
}


inline const BPPFunctionTable*
Service::getEntryPoints()
{
    static BPPFunctionTable s_functionTable =
    {
        BPP_SERVICE_API_VERSION,
        bppInitialize,
        bppShutdown,
        bppAllocate,
        bppDestroy,
        bppInvoke,
        bppCancel,
        bppInstall,
        bppUninstall
    };

    return &s_functionTable;
}


inline const BPServiceDefinition*
Service::bppInitialize( const BPCFunctionTable* pCoreFuncs,
                        const BPPath serviceDir,
                        const BPPath dependentDir,
                        const BPElement* pDependentParams )
{
    s_pCoreFuncs        = pCoreFuncs;
    s_serviceDir        = serviceDir;
    s_dependentDir      = bplus::strutil::safeStr(dependentDir);
    s_pDependentParams  = bplus::Object::build(pDependentParams);

    setupDescription();
    
    // Call our preprocessor-generated func that knows derived service name.
    // Note: At the moment we allow service initialization to proceed,
    //       even if this call fails.
    //       We could change to return NULL on failure.
    // Note: we prepend fullName because sometimes it isn't available
    //       to higher logging levels at this point.
    if (!callInitializeHook()) {
        std::string sLog = "(" + fullName() + ") onInitialize() failed. ";
        log( BP_WARN, sLog );
    }
    
    return s_description.toBPServiceDefinition();
}


inline void
Service::bppShutdown()
{
    // Call our preprocessor-generated func that knows derived service name.
    if (!callShutdownHook()) {
        log( BP_WARN, "onShutdown() failed." );
    }
}


inline int
Service::bppAllocate( void** instance,
                      const BPString uri, const BPPath serviceDir,
                      const BPPath dataDir, const BPPath tempDir,
                      const BPString locale,
                      const BPString userAgent, int clientPid )
{
    Service* pInst = createInstance();

    // Our class factory uses Service default constructor.
    // So we have to set instance attributes manually.
    pInst->m_clientUri  = uri;
    pInst->m_serviceDir = serviceDir;
    pInst->m_dataDir    = dataDir;
    pInst->m_tempDir    = tempDir;
    pInst->m_locale     = locale;
    pInst->m_userAgent  = userAgent;
    pInst->m_clientPid  = clientPid;
    
    // Let derived service do any needed work now that members are setup.
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
Service::bppCancel( void* pInstance, unsigned int tid )
{
    // TODO: implement
}


inline int
Service::bppInstall(const BPPath serviceDir, const BPPath dataDir)
{
    return callInstallHook(serviceDir, dataDir);
}

inline int
Service::bppUninstall(const BPPath serviceDir, const BPPath dataDir)
{
    return callUninstallHook(serviceDir, dataDir);
}


} // service
} // bplus



////////////////////////////////////////////////////////////////////////////////
// Macro Magic
//
// Some of the functions performed:
// * instantiate derived service
// * Implement the required browserplus Service API C functions
// * route method invokations to proper service method
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
bplus::tPathString bplus::service::Service::s_serviceDir; \
bplus::tPathString bplus::service::Service::s_dependentDir; \
bplus::Object* bplus::service::Service::s_pDependentParams = NULL; \
bplus::service::Description bplus::service::Service::s_description; \
\
bplus::service::Service* bplus::service::Service::createInstance() \
{ \
    return new className(); \
} \
\
typedef void (className::* tInvokableFunc)( const bplus::service::Transaction& tran, \
                                            const bplus::Map& args ); \
std::map<std::string, tInvokableFunc> className::s_mapFuncs; \
\
inline bool bplus::service::Service::callInitializeHook() \
{ \
    return className::onInitialize(); \
} \
\
inline bool bplus::service::Service::callShutdownHook() \
{ \
    return className::onShutdown(); \
} \
\
inline int bplus::service::Service::callInstallHook(const BPPath serviceDir, \
                                                    const BPPath dataDir) \
{ \
   return className::onInstall(serviceDir, dataDir); \
} \
\
inline int bplus::service::Service::callUninstallHook(const BPPath serviceDir, \
                                                      const BPPath dataDir) \
{ \
   return className::onUninstall(serviceDir, dataDir); \
}



//////////////////////////////
// BP_SERVICE_DESC
// Note: this macro does double duty.
// To a client it appears they're just describing their API.
// But we piggyback on their macro invokation to call BP_SERVICE_DEFNS
// to define other needed items.
// We do it this way to minimize the macro use required of clients.
//

#define BP_SERVICE_DESC( className, serviceName, version, docString ) \
BP_SERVICE_DEFNS( className ); \
\
void bplus::service::Service::setupDescription() \
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
}



//////////////////////////////
// BP_SERVICE
//
// All the definitions here happen within a class declaration, since
// that is where the macro is invoked.
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


#endif // BPSERVICEIMPL_H_
