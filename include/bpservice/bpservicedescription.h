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
 * bpservicedescription.h
 * 
 * An abstraction around the in-memory representation of a description
 * of a service's interface.
 *
 * XXX: this c++ representation should be augmented to store information
 *      internally in a format that can be transmitted over the
 *      service API (BPCoreletDefinition).
 */

#ifndef BPSERVICEDESCRIPTION_H_
#define BPSERVICEDESCRIPTION_H_

#include <list>
#include "bpserviceversion.h"
#include "bputil/bptypeutil.h"
#include "bpserviceapi/bpdefinition.h"


namespace bplus {
namespace service {


/**
 * an in memory representation of an argument to a function on a corelet
 */ 
class Argument
{
public:
    enum Type {
        None,
        Null,
        Boolean,
        Integer,
        Double,
        String,
        Map,
        List,
        CallBack,
        Path,
        Any
    };

    Argument();
    Argument(const char * name, Type type);
    ~Argument();

    std::string name() const;
    void setName(const char * name);

    Type type() const;
    void setType(Type type);

    /** get a string representation of the type */
    static const char * typeAsString(Type type);

    /** get a type from a string */
    static Type stringAsType(const char * str);

    bool required() const;
    void setRequired(bool required);

    std::string docString() const;
    void setDocString(const char * docString);    

    /** re-initialize the class */
    void clear();

    bool fromBPArgumentDefinition(const BPArgumentDefinition * bpcd);

    /**
     * generate a BPArgumentDefinition representation of this function.
     * This pointer will point to memory managed by this class,
     * that will not change until toBPCoreletDefinition is called again,
     * or the instance is deleted.
     */
    void toBPArgumentDefinition(BPArgumentDefinition * argDef);

private:
    std::string m_name;    
    std::string m_docString;
    Type m_type;
    bool m_required;
};

/**
 * an in memory representation of a function supported by a corelet
 */ 
class Function
{
public:
    Function();
    Function(const Function & f);
    Function & operator=(const Function & f);
    ~Function();    

    std::string name() const;
    void setName(const char * name);

    std::list<Argument> arguments() const;
    void setArguments(const std::list<Argument> & arguments);    

    /** add an argument to the current list */
    void addArgument( const Argument& argument );
    
    bool getArgument(const char * name, Argument & oArg) const;

    std::string docString() const;
    void setDocString(const char * docString);    

    /** re-initialize the class */
    void clear();

    bool fromBPFunctionDefinition(const BPFunctionDefinition * bpcd);

    /**
     * populate a pointer to a BPFunctionDefinition with pointers into
     * internal memory that will not change until
     * toBPCoreletDefinition is called again, or the instance is
     * deleted.
     */
    void toBPFunctionDefinition(BPFunctionDefinition * func);

private:
    std::string m_name;
    std::string m_docString;    
    std::list<Argument> m_arguments;

    BPArgumentDefinition * m_adefs;
};

/**
 * an in memory description of a corelet's interface
 */ 
class Description
{
public:
    Description();
    Description(const Description & d);
    Description & operator=(const Description & d);
    ~Description();    

    std::string name() const;
    void setName(const char * name);

    /** get a string representation of major, minor, and
     *  micro joined with '.' */
    std::string versionString() const;

    /** get corelet version */
    Version version() const;

    /** set corelet version */
    void setVersion(const Version& version);

    /** get corelet major version */
    unsigned int majorVersion() const;

    /** set corelet major version */
    void setMajorVersion(unsigned int majorVersion);    

    /** get corelet minor version */
    unsigned int minorVersion() const;
    /** set corelet minor version */
    void setMinorVersion(unsigned int minorVersion);    

    /** get corelet micro version */
    unsigned int microVersion() const;
    /** set corelet micro version */
    void setMicroVersion(unsigned int microVersion);    

    std::string docString() const;
    void setDocString(const char * docString);    

    std::list<Function> functions() const;
    void setFunctions(const std::list<Function> & functions);    

    /** add a function to the current list */
    void addFunction(const Function& function);
    
    bool hasFunction(const char * funcName) const;

    /** get the function description */
    bool getFunction(const char * funcName, Function & oFunc) const;

    /** get pointer to requested function or 0 if not found */
    Function* getFunction(const char * funcName ) const;
    
    /** generate a bplus::Object representation of the corelet description.
     *  caller assumes ownership of returned value */
    bplus::Object* toBPObject() const;
    
    /** populate the structure from a bplus::Object representation.
     *  returns false on failure, true on success. */
    bool fromBPObject(const bplus::Object* bp);

    /**
     * populate the structure from a BPCoreletDefinition memory structure
     * returns false on failure, true on success.
     */
    bool fromBPCoreletDefinition(const BPCoreletDefinition * bpcd);

    /**
     * generate a BPCoreletDefinition representation of this service's
     * interface.  This pointer will point to memory managed by this class,
     * that will not change until toBPCoreletDefinition is called again,
     * or the instance is deleted.
     */
    const BPCoreletDefinition * toBPCoreletDefinition(void);

    /** Does this corelet description describe a built in corelet? */
    bool isBuiltIn() const;
    void setIsBuiltIn(bool isBuiltIn);

    /** re-initialize the definition */
    void clear();

    /** generate a human readable buffer of the interface of the service */
    std::string toHumanReadableString() const;

private:
    std::string m_name;
    unsigned int m_majorVersion;    
    unsigned int m_minorVersion;    
    unsigned int m_microVersion;    
    std::string m_docString;
    std::list<Function> m_functions;
    // true for built in corelets, added using the
    // CoreletRegistry::registerCorelet() call
    bool m_builtIn;

    BPCoreletDefinition * m_def;
    void freeDef();
};

/**
 * validate arguments given a bplus::Map containing arguments and a
 * function description describing a function's interface.
 *
 * NOTE:  "arguments" may be modified, for example when converting double
 *                    arguments to integers (thanks, Safari).
 *
 * this function is used by both the daemon (for services) and the
 * plugin (for pluglets)
 *
 * returns non-empty string on failure, containing a verbose error message
 */
std::string validateArguments(const bplus::service::Function & fdesc,
                              bplus::Map* arguments);


std::string
validateArguments(const bplus::service::Function & desc, bplus::Map* arguments);


} // namespace service
} // namespace bplus


//////////////////////////////////////////////////////////////////////
// Get the implementations.
#include "impl/bpservicedescriptionimpl.h"


#endif // BPSERVICEDESCRIPTION_H_
