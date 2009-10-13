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

// forward decls
bool callLoadHandler();   
bool callUnloadHandler();   


inline void
Service::log( unsigned int level,
              const std::string& sIn )
{
    if (s_pCoreFuncs->log) {
        s_pCoreFuncs->log( level, "%s", sIn.c_str() );
    }
}


inline bool
Service::onServiceLoad()
{
    return true;
}


inline bool
Service::onServiceUnload()
{
    return true;
}


inline std::string Service::fullName()
{
    return s_description.name() + " " + s_description.versionString();
}


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

    setupDescription();
    
    // Call our preprocessor-generated func that knows derived service name.
    // Note: At the moment we allow service initialization to proceed,
    //       even if this call fails.
    //       We could change to return NULL on failure.
    // Note: we prepend fullName because sometimes it isn't available
    //       to higher logging levels at this point.
    if (!callLoadHandler()) {
        std::string sLog = "(" + fullName() + ") onServiceLoad() failed. ";
        log( BP_WARN, sLog );
    }
    
    return s_description.toBPCoreletDefinition();
}


inline void
Service::bppShutdown()
{
    // Call our preprocessor-generated func that knows derived service name.
    if (!callUnloadHandler()) {
        log( BP_WARN, "onServiceUnload() failed." );
    }
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
const BPElement* bplus::service::Service::s_pParamMap = NULL; \
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
inline bool bplus::service::callLoadHandler() \
{ \
    return className::onServiceLoad(); \
} \
\
inline bool bplus::service::callUnloadHandler() \
{ \
    return className::onServiceUnload(); \
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
