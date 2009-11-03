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
 *  Inline implementation file for bpservicedescription.h.
 * 
 *  Note: This file is included by bpservicedescription.h.
 *        It is not intended for direct inclusion by client code.
 *       
 */
#ifndef BPSERVICEDESRIPTIONIMPL_H_
#define BPSERVICEDESRIPTIONIMPL_H_

#include <sstream>
#include <set>


inline bplus::service::Argument::Argument()
    : m_type(None),
      m_required(true)
{
}

inline bplus::service::Argument::Argument(const char * name,
                                   bplus::service::Argument::Type t)
    : m_required(true)
{
    setType(t);
    setName(name);
}


inline bplus::service::Argument::~Argument()
{
}

inline std::string
bplus::service::Argument::name() const
{
    return m_name;
}

inline void
bplus::service::Argument::setName(const char * name)
{
    m_name = name;
}

inline bplus::service::Argument::Type
bplus::service::Argument::type() const
{
    return m_type;
}

inline void
bplus::service::Argument::setType(Type type)
{
    m_type = type;
}

inline bool
bplus::service::Argument::required() const
{
    return m_required;
}

inline void
bplus::service::Argument::setRequired(bool required)
{
    m_required = required;
}

inline std::string
bplus::service::Argument::docString() const
{
    return m_docString;
}

inline void
bplus::service::Argument::setDocString(const char * docString)
{
    m_docString = docString;
}

inline const char *
bplus::service::Argument::typeAsString(Type type)
{
    switch (type) {
        case None: return "none";
        case Null: return "null";
        case Boolean: return "boolean";
        case Integer: return "integer";
        case Double: return "double";
        case String: return "string";
        case Map: return "map";
        case List: return "list";
        case CallBack: return "callback";
        case Path: return "path";
        case Any: return "any";
    }
    return "unknown";
}

inline bplus::service::Argument::Type
bplus::service::Argument::stringAsType(const char * str)
{
    Type t = None;
    if (str != NULL) {
        if (!strcmp(str, "null")) t = Null;
        else if (!strcmp(str, "boolean")) t = Boolean;
        else if (!strcmp(str, "integer")) t = Integer;
        else if (!strcmp(str, "double")) t = Double;
        else if (!strcmp(str, "string")) t = String;
        else if (!strcmp(str, "map")) t = Map;
        else if (!strcmp(str, "list")) t = List;
        else if (!strcmp(str, "callback")) t = CallBack;
        else if (!strcmp(str, "path")) t = Path;
        else if (!strcmp(str, "any")) t = Any;
    }
    return t;
}

inline void 
bplus::service::Argument::clear()
{
    m_name.clear();
    m_docString.clear();
    m_type = None;
    m_required = true;
}

inline bool 
bplus::service::Argument::fromBPArgumentDefinition(const BPArgumentDefinition * def)
{
    clear();

    if (def->name) m_name.append(def->name);
    if (def->docString) m_docString.append(def->docString);
    switch (def->type) {
        case BPTNull: m_type = Null; break;
        case BPTBoolean: m_type = Boolean; break;
        case BPTInteger: m_type = Integer; break;
        case BPTDouble: m_type = Double; break; 
        case BPTString: m_type = String; break;
        case BPTMap: m_type = Map; break;
        case BPTList: m_type = List; break;
        case BPTCallBack: m_type = CallBack; break;
        case BPTPath: m_type = Path; break;
        case BPTAny: m_type = Any; break;
        default: m_type = None; break;
    }
    m_required = def->required != 0;
    
    return true;
}

inline void
bplus::service::Argument::toBPArgumentDefinition(BPArgumentDefinition * argDef)
{
    
    argDef->name = (char *) m_name.c_str();
    argDef->docString = (char *) m_docString.c_str();
    switch (m_type) {
        case None: // collapse None -> Null
        case Null: argDef->type = BPTNull; break;
        case Boolean: argDef->type = BPTBoolean; break;
        case Integer: argDef->type = BPTInteger; break;
        case Double: argDef->type = BPTDouble; break; 
        case String: argDef->type = BPTString; break;
        case Map: argDef->type = BPTMap; break;
        case List: argDef->type = BPTList; break;
        case CallBack: argDef->type = BPTCallBack; break;
        case Path: argDef->type = BPTPath; break;
        case Any: argDef->type = BPTAny; break;
    }
    argDef->required = m_required;
}

inline bplus::service::Function::Function()
    : m_adefs(NULL)
{
}

inline bplus::service::Function::Function(const bplus::service::Function & f)
    : m_name(f.m_name),
      m_docString(f.m_docString),
      m_arguments(f.m_arguments),
      m_adefs(NULL) // generated on demand, don't copy
{
}

inline bplus::service::Function &
bplus::service::Function::operator=(const bplus::service::Function & f)
{
    m_name = f.m_name;
    m_docString = f.m_docString;
    m_arguments = f.m_arguments;
    
    if (m_adefs) {
        free(m_adefs);
    }
    m_adefs = NULL;
    return *this;
}

inline bplus::service::Function::~Function()
{
    
    if (m_adefs) {
        free(m_adefs);
        m_adefs = NULL;
    }
}

inline std::string
bplus::service::Function::name() const
{
    return m_name;
}

inline void
bplus::service::Function::setName(const char * name)
{
    m_name = name;
}

inline std::list<bplus::service::Argument>
bplus::service::Function::arguments() const
{
    return m_arguments;
}

inline bool
bplus::service::Function::getArgument(const char * name, Argument &oArg) const
{
    std::list<Argument>::const_iterator it;
    
    for (it = m_arguments.begin(); it != m_arguments.end(); it++)
    {
        if (!it->name().compare(name)) 
        {
            oArg = *it;
            return true;
        }
    }
    
    return false;
}

inline void
bplus::service::Function::setArguments(
    const std::list<bplus::service::Argument> & arguments) 
{
    m_arguments = arguments;
}

inline void
bplus::service::Function::addArgument(const Argument& argument)
{
    m_arguments.push_back( argument );
}
                                  
inline std::string
bplus::service::Function::docString() const
{
    return m_docString;
}

inline void
bplus::service::Function::setDocString(const char * docString)
{
    m_docString = docString;
}

inline void 
bplus::service::Function::clear()
{
    m_name.clear();
    m_docString.clear();
    m_arguments.clear();
}

inline bool 
bplus::service::Function::fromBPFunctionDefinition(const BPFunctionDefinition * def)
{
    clear();
    if (!def) return false;
    if (def->functionName) m_name.append(def->functionName);
    if (def->docString) m_docString.append(def->docString);

    // now arguments
    for (unsigned int i = 0; i < def->numArguments; i++) {
        Argument a;
        if (a.fromBPArgumentDefinition(def->arguments + i)) {
            m_arguments.push_back(a);
        } else {
            clear();
            return false;
        }
    }

    return true;
}

inline void
bplus::service::Function::toBPFunctionDefinition(BPFunctionDefinition * func)
{
    
    if (m_adefs) {
        free(m_adefs);
        m_adefs = NULL;
    }
    func->functionName = (char *) m_name.c_str();
    func->docString = (char *) m_docString.c_str();

    // now arguments
    if (m_arguments.size()) {
        func->numArguments = m_arguments.size();
        func->arguments = m_adefs = (BPArgumentDefinition *)
            calloc(m_arguments.size(), sizeof(BPArgumentDefinition));    

        std::list<Argument>::iterator i;

        unsigned int x;
        for (x = 0, i = m_arguments.begin(); i != m_arguments.end(); x++, i++)
        {
            i->toBPArgumentDefinition(m_adefs + x);
        }
    }
}

inline bplus::service::Description::Description()
    : m_majorVersion(0),
      m_minorVersion(0),
      m_microVersion(0),
      m_builtIn(false),
      m_def(NULL)
{
}

inline bplus::service::Description::Description(const bplus::service::Description & d)
    : m_name(d.m_name), 
    m_majorVersion(d.m_majorVersion),    
    m_minorVersion(d.m_minorVersion),    
    m_microVersion(d.m_microVersion),
    m_docString(d.m_docString), 
    m_functions(d.m_functions),
    m_builtIn(d.m_builtIn),
    m_def(NULL)  // generated on demand, don't copy
{
}

inline bplus::service::Description &
bplus::service::Description::operator=(const bplus::service::Description & d)
{
    m_name = d.m_name;
    m_majorVersion = d.m_majorVersion;
    m_minorVersion = d.m_minorVersion;
    m_microVersion = d.m_microVersion;
    m_docString= d.m_docString;
    m_functions = d.m_functions;
    m_builtIn = d.m_builtIn;

    if (m_def) free(m_def); // m_def is demand generated!
    m_def = NULL;

    return *this;
}

inline bplus::service::Description::~Description()
{
    freeDef();
}

inline bool
bplus::service::Description::isBuiltIn() const
{
    return m_builtIn;
}

inline void
bplus::service::Description::setIsBuiltIn(bool x)
{
    m_builtIn = x;
}

inline std::string
bplus::service::Description::name() const
{
    return m_name;
}

inline void
bplus::service::Description::setName(const char * name)
{
    m_name = name;
}

inline std::string
bplus::service::Description::docString() const
{
    return m_docString;
}

inline void
bplus::service::Description::setDocString(const char * docString)
{
    m_docString = docString;
}

inline std::list<bplus::service::Function>
bplus::service::Description::functions() const
{
    return m_functions;
}

inline void
bplus::service::Description::setFunctions(
    const std::list<bplus::service::Function> & functions)
{
    m_functions = functions;
}

inline void
bplus::service::Description::addFunction( const Function& function )
{
    m_functions.push_back( function );
}

inline bool
bplus::service::Description::getFunction(const char * funcName,
                                  bplus::service::Function & oFunc) const
{
    std::list<Function>::const_iterator it;
    
    for (it = m_functions.begin(); it != m_functions.end(); it++)
    {
        if (!it->name().compare(funcName)) 
        {
            oFunc = *it;
            return true;
        }
    }
    
    return false;
}

inline bplus::service::Function*
bplus::service::Description::getFunction(const char * funcName) const
{
	std::list<bplus::service::Function>::const_iterator it;

    for (it = m_functions.begin(); it != m_functions.end(); it++) {
        if (!it->name().compare(funcName)) {
			return const_cast<bplus::service::Function*>(&(*it));
        }
    }

    return NULL;
}

inline bool
bplus::service::Description::hasFunction(const char * funcName) const
{
    Function f;
    return getFunction(funcName, f);
}

inline std::string
bplus::service::Description::versionString() const
{
    std::stringstream ss;
    ss << m_majorVersion << "." << m_minorVersion << "." << m_microVersion;
    return ss.str();
}

inline bplus::service::Version
bplus::service::Description::version() const
{
    bplus::service::Version v;
    v.setMajor(m_majorVersion);
    v.setMinor(m_minorVersion);    
    v.setMicro(m_microVersion);    

    return v;
}

inline void
bplus::service::Description::setVersion(const bplus::service::Version& version)
{
    // Note: service::Version uses -1 as a "not set" indicator.
    if (version.majorVer() >= 0) {
        m_majorVersion = version.majorVer();
    }

    if (version.minorVer() >= 0) {
        m_minorVersion = version.minorVer();
    }

    if (version.microVer() >= 0) {
        m_microVersion = version.microVer();
    }
}

inline unsigned int
bplus::service::Description::majorVersion() const
{
    return m_majorVersion;    
}

inline void
bplus::service::Description::setMajorVersion(unsigned int majorVersion)
{
    m_majorVersion = majorVersion;    
}

inline unsigned int
bplus::service::Description::minorVersion() const
{
    return m_minorVersion;
}

inline void
bplus::service::Description::setMinorVersion(unsigned int minorVersion)
{
    m_minorVersion = minorVersion;    
}

inline unsigned int
bplus::service::Description::microVersion() const
{
    return m_microVersion;
}

inline void
bplus::service::Description::setMicroVersion(unsigned int microVersion)
{
    m_microVersion = microVersion;
}


inline bplus::Object*
bplus::service::Description::toBPObject() const
{
    // This builds a description which will be sent to clients.
    // Hence, the JSON includes bplus::Object type info.
    using namespace bplus;
    
    Map* m = new Map;
    m->add("name", new String(name()));

    // include the version as a string
    m->add("versionString", new String(versionString()));

    // include the version as integers so that clients interested
    // in them broken up need not parse the version string
    Map* verMap = new Map;
    verMap->add("major", new Integer(majorVersion()));
    verMap->add("minor", new Integer(minorVersion()));
    verMap->add("micro", new Integer(microVersion()));
    m->add("version", verMap);

    if (!m_docString.empty()) {
        m->add("documentation", new String(m_docString));
    }
        
    List* l = new List;

    std::list<Function>::const_iterator fit;
    for (fit = m_functions.begin(); fit != m_functions.end(); fit++)
    {
        Map* funcDesc = new Map;
        funcDesc->add("name", new String(fit->name()));
        if (!fit->docString().empty())
        {
            funcDesc->add("documentation", new String(fit->docString()));
        }
            
        List* params = new List;
        std::list<Argument> args = fit->arguments();
        std::list<Argument>::const_iterator ait;
        for (ait = args.begin(); ait != args.end(); ait++)
        {
            Map* param = new Map;
            param->add("name", new String(ait->name()));
            param->add("required", new Bool(ait->required()));
            if (!ait->docString().empty()) {
                param->add("documentation", new String(ait->docString()));
            }
                
            const char * type = Argument::typeAsString(ait->type());
            param->add("type", new String(type));;
                
            params->append(param);
        }
        funcDesc->add("parameters", params);
        l->append(funcDesc);
    }

    m->add("functions", l);

    return m;
}

inline bool
bplus::service::Description::fromBPObject(const Object* bp)
{
    // This parses JSON that comes from the server.  Hence, it
    // is "regular" json without all of the bplus::Object type info.
    
    const Map* m = dynamic_cast<const Map*>(bp);
    if (m == NULL) return false;
    
    if (!m->has("name", BPTString)) return false;
    setName(std::string(*(m->get("name"))).c_str());

    if (!m->has("version/major", BPTInteger) ||
        !m->has("version/minor", BPTInteger) ||
        !m->has("version/micro", BPTInteger))
    {
        return false;
    }

    setMajorVersion((unsigned int) (dynamic_cast<const Integer*>
                                    (m->get("version/major")))->value());
    setMinorVersion((unsigned int) (dynamic_cast<const Integer*>
                                    (m->get("version/minor")))->value());
    setMicroVersion((unsigned int) (dynamic_cast<const Integer*>
                                    (m->get("version/micro")))->value());
    
    if (m->has("documentation", BPTString)) {
        setDocString(std::string(*(m->get("documentation"))).c_str());
    }
    
    if (m->has("functions", BPTList)) {
        const List* l = dynamic_cast<const List*>(m->get("functions"));
        assert(l != NULL);

        unsigned int i;
        
        for (i=0; i < l->size(); i++) {
            const Map* fm = dynamic_cast<const Map*>(l->value(i));
            if (fm == NULL) continue;
            Function func;
            if (fm->has("name", BPTString)) {
                func.setName(std::string(*(fm->get("name"))).c_str());
            }
            if (fm->has("documentation", BPTString)) {
                func.setDocString(
                    std::string(*(fm->get("documentation"))).c_str());
            }
            /* now we gotta do params (sheesh) */
            std::list<Argument> params;
            
            if (fm->has("parameters", BPTList)) {          
                const List* pl =
                    dynamic_cast<const List*>(fm->get("parameters"));

                unsigned int j;
                for (j=0; j<pl->size();j++) {
                    const Map* am =
                        dynamic_cast<const Map*>(pl->value(j));

                    if (am == NULL) continue;

                    Argument cap;
                    if (am->has("name", BPTString)) {
                        cap.setName(std::string(*(am->get("name"))).c_str());
                    }
                    if (am->has("required", BPTBoolean)) {
                        cap.setRequired(*(am->get("required")));
                    }
                    if (am->has("documentation", BPTString)) {
                        cap.setDocString(
                            std::string(*(am->get("documentation"))).c_str());
                    }
                    if (am->has("type", BPTString)) {
                        std::string typeStr = std::string(*(am->get("type")));
                        Argument::Type t =
                            Argument::stringAsType(typeStr.c_str());
                        cap.setType(t);
                    }
                    params.push_back(cap);
                }
            }
            func.setArguments(params);
            m_functions.push_back(func);
        }
    }

    return true;
}

inline void 
bplus::service::Description::clear()
{
    m_name.clear();
    m_majorVersion = m_minorVersion = m_microVersion = 0;
    m_docString.clear();
    m_functions.clear();
    m_builtIn = false;
}

inline bool 
bplus::service::Description::fromBPCoreletDefinition(const BPCoreletDefinition * def)
{
    clear();
    if (!def) return false;

    if (def->coreletName) m_name.append(def->coreletName);
    m_majorVersion = def->majorVersion;    
    m_minorVersion = def->minorVersion;
    m_microVersion = def->microVersion;
    if (def->docString) m_docString.append(def->docString);    

    // now functions
    for (unsigned int i = 0; i < def->numFunctions; i++) {
        Function f;
        if (f.fromBPFunctionDefinition(def->functions + i)) {
            m_functions.push_back(f);
        } else {
            clear();
            return false;
        }
    }
    
    return true;
}


inline std::string
bplus::service::validateArguments(const bplus::service::Function & desc,
                               bplus::Map* arguments)
{
    // We do this verification in two, expensive iterations.  suspect
    // n is sufficiently small that it doesn't matter.  let the profiler
    // be the judge.

    // a set that we'll build up for more efficient checking of presence
    // of required arguments
    std::set<std::string> haveArgs;
    
    // a map of arguments which must be modified.  
    // Needed since Safari will often send integer arguments
    // as doubles
    std::map<std::string, bplus::Object*> mods;
    
    // _first_ verify all arguments that are present are of the
    // correct type
    if (arguments != NULL) 
    {
        bplus::Map::Iterator iter(*arguments);
        const char* name = NULL;
        while ((name = iter.nextKey()) != NULL) 
        {
            bplus::service::Argument adesc;

            // unknown argument
            if (!desc.getArgument(name, adesc))
            {
                std::stringstream ss;
                ss << "argument '" << name << "' not supported by function '"
                   << desc.name() << "'";
                return ss.str();
            }

            // argument is known, now check type
            const char * gottype = NULL;
            if (adesc.type() != bplus::service::Argument::Any) {
                switch (arguments->value(name)->type())
                {
                    case BPTBoolean:
                        if (adesc.type() != bplus::service::Argument::Boolean)
                        {
                            gottype = "boolean";
                        }
                        break;
                    case BPTDouble:
                        if (adesc.type() != bplus::service::Argument::Double)
                        {
                            gottype = "double";
                        }
                        break;
                    case BPTCallBack:
                        if (adesc.type() != bplus::service::Argument::CallBack)
                        {
                            gottype = "callback";
                        }
                        break;
                    case BPTInteger:
                        if (adesc.type() != bplus::service::Argument::Integer)
                        {
                            gottype = "integer";
                        }
                        break;
                    case BPTList:
                        if (adesc.type() != bplus::service::Argument::List)
                        {
                            gottype = "list";
                        }
                        break;
                    case BPTMap:
                        if (adesc.type() != bplus::service::Argument::Map)
                        {
                            gottype = "map";
                        }
                        break;
                    case BPTString:
                        if (adesc.type() != bplus::service::Argument::String)
                        {
                            gottype = "string";
                        }
                        break;
                    case BPTPath:
                        if (adesc.type() != bplus::service::Argument::Path)
                        {
                            gottype = "path";
                        }
                        break;
                    case BPTNull:
                        if (adesc.type() != bplus::service::Argument::Null)
                        {
                            gottype = "null";
                        }
                        break;
                    default:
                        gottype = "unknown";
                        break;
                };
            }
            
            // if gottype is non-null, we have a type mismatch
            if (gottype != NULL)
            {
                // Allow conversion between int and double.  Needed
                // since Safari often sends integers as doubles
                std::string expected(bplus::service::Argument::typeAsString(adesc.type()));
                std::string got(gottype);
                if (!expected.compare("integer") && !got.compare("double")) {
                    const bplus::Double* oldVal = 
                        dynamic_cast<const bplus::Double*>(arguments->value(name));
                    double dval = oldVal->value() + 0.5;  // round
                    mods[name] = new bplus::Integer((BPInteger)dval);
                } else if (!expected.compare("double") 
                           && !got.compare("integer")) {
                    const bplus::Integer* oldVal = 
                        dynamic_cast<const bplus::Integer*>(arguments->value(name));
                    mods[name] = new bplus::Double((BPDouble)oldVal->value());
                } else {
                    std::stringstream ss;
                    ss << "argument '" << name
                       << "' should be of type "
                       << bplus::service::Argument::typeAsString(adesc.type())
                       << ", but is of type " << gottype;
                    return ss.str();
                }
            }

            // we're cool.  add this to the set of arguments we have
            haveArgs.insert(name);
        }
    }
        
    // modify args as needed
    std::map<std::string, bplus::Object*>::iterator miter;
    for (miter = mods.begin(); miter != mods.end(); ++miter) {
        arguments->add(miter->first.c_str(), miter->second);
    }
    
    // verify all required arguments are present
    std::list<bplus::service::Argument> args = desc.arguments();
    std::list<bplus::service::Argument>::iterator ait;    
    for (ait = args.begin(); ait != args.end(); ait++)
    {
        if (ait->required())
        {
            std::set<std::string>::iterator findArg;
            findArg = haveArgs.find(ait->name());
            if (findArg == haveArgs.end())
            {
                std::stringstream ss;
                ss << "call to '" << desc.name() << "' requires a '"
                   << ait->name() << "' argument";
                return ss.str();
            }
        }
    }
    return std::string();
}

// try to format a doc string, breaking lines at 60 chars
#define DEFAULTBREAKAT 70
static std::string
formatDocString(const char * str, const char * pad = NULL,
                unsigned int breakat = DEFAULTBREAKAT)
{
    std::string out;
    
    unsigned int ix;
    unsigned int space = 0;
    unsigned int lastBreak = 0;
    for (ix = 0; ix < strlen(str); ix++)
    {
        switch (str[ix]) {
            case ' ':             space = ix; break;
            case '\n': case '\r': lastBreak = ix; break;
        }

        if (ix - lastBreak >= breakat && space != 0)
        {
            out.append(str + lastBreak, space - lastBreak);
            out += "\n";
            if (pad != NULL) out += pad;
            ix = lastBreak = space+1;
            space = 0;
        }
    }
    out.append(str + lastBreak, ix - lastBreak);

    return out;
}

#define MYSPACE "   "
inline std::string
bplus::service::Description::toHumanReadableString() const
{
    std::stringstream ss;
    
    ss << "Describing corelet '"
       << (name().empty() ? "unknown" : name())
       << "', version: "
       << versionString() << std::endl;

    if (!docString().empty())
    {
        ss << MYSPACE
           << formatDocString(docString().c_str(), MYSPACE)
           << std::endl;
    }

    std::list<bplus::service::Function> functions = this->functions();
    std::list<bplus::service::Function>::iterator fit;

    ss << std::endl;        
    ss << functions.size() << " function(s) supported:" << std::endl;

    for (fit = functions.begin(); fit != functions.end(); fit++) 
    {
        ss << MYSPACE << fit->name() << ":" << std::endl;

        if (!fit->docString().empty()) {
            ss << MYSPACE MYSPACE
               << formatDocString(fit->docString().c_str(),
                                  MYSPACE MYSPACE)
               << std::endl;
        }
        
        // now for arguments
        std::list<bplus::service::Argument> arguments = fit->arguments();
        std::list<bplus::service::Argument>::iterator ait;

        if (arguments.size() > 0) {
            ss << std::endl;
            for (ait = arguments.begin(); ait != arguments.end(); ait++) 
                ss << MYSPACE MYSPACE MYSPACE
                   << (ait->required() ? "" : "[") 
                   << Argument::typeAsString(ait->type())<< " "
                   << ait->name()
                   << (ait->required() ? "" : "]") 
                   << std::endl
                   << MYSPACE MYSPACE MYSPACE MYSPACE 
                   << formatDocString(ait->docString().c_str(),
                                      MYSPACE MYSPACE
                                      MYSPACE MYSPACE, 50)
                   << std::endl;
        }
    }
    ss << std::endl;

    return ss.str();
}


inline void
bplus::service::Description::freeDef()
{
    if (m_def) {
        if (m_def->functions) free(m_def->functions);
        free(m_def);
        m_def = NULL;
    }
}

inline const BPCoreletDefinition *
bplus::service::Description::toBPCoreletDefinition(void)
{
    freeDef();
    
    m_def = (BPCoreletDefinition *) calloc(1, sizeof(BPCoreletDefinition));
    assert(m_def != NULL);

    m_def->coreletName = (char *) m_name.c_str();
    m_def->majorVersion = m_majorVersion;
    m_def->minorVersion = m_minorVersion;    
    m_def->microVersion = m_microVersion;    
    m_def->docString = (char *) m_docString.c_str();
    
    // functions
    if (m_functions.size()) {
        
        m_def->numFunctions = m_functions.size();
        
        m_def->functions = (BPFunctionDefinition *)
            calloc(m_functions.size(), sizeof(BPFunctionDefinition));    

        std::list<Function>::iterator i;

        unsigned int x;
        for (x = 0, i = m_functions.begin(); i != m_functions.end(); x++, i++) {
            i->toBPFunctionDefinition(m_def->functions + x);
        }
    }
    
    return m_def;
}


#endif // BPSERVICEDESRIPTIONIMPL_H_
