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
 * Portions created by Yahoo! are Copyright (C) 2006-2008 Yahoo!.
 * All Rights Reserved.
 * 
 * Contributor(s): 
 * ***** END LICENSE BLOCK ***** */

/**
 *  bptypeutilimpl.h
 * 
 *  Inline implementation file for bptypeutil.h.
 *
 *  Note: This file is included by bptypeutil.h.
 *        It is not intended for direct inclusion by client code.
 */
#ifndef BPTYPEUTILIMPL_H_
#define BPTYPEUTILIMPL_H_

#include <assert.h>

#ifdef WIN32
#pragma warning(disable:4100)
#endif


#ifdef WIN32
#define PATH_SEPARATOR "\\"
#else 
#define PATH_SEPARATOR "/"
#endif

#define FILE_URL_PREFIX "file://"


inline std::vector<std::string> 
split(const std::string& str, 
      const std::string& delim)
{
    std::vector<std::string> vsRet;
    
    unsigned int offset = 0;
    unsigned int delimIndex = 0;
    delimIndex = str.find(delim, offset);
    while (delimIndex != std::string::npos) {
        vsRet.push_back(str.substr(offset, delimIndex - offset));
        offset += delimIndex - offset + delim.length();
        delimIndex = str.find(delim, offset);
    }
    vsRet.push_back(str.substr(offset));

    return vsRet;
}

inline std::string 
urlEncode(const std::string& s)
{
    std::string out;
    
    char hex[4];

    static const char noencode[] = "!'()*-._";
    static const char hexvals[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
        'A', 'B', 'C', 'D', 'E', 'F'
    };
    
    for (unsigned int i = 0; i < s.length(); i++) {
        if (isalnum((unsigned char)s[i]) || strchr(noencode, s[i]) != NULL) {
            out.append(&s[i], 1);
        } else {
            hex[0] = '%';
            hex[1] = hexvals[(s[i] >> 4) & 0x0F];
            hex[2] = hexvals[s[i] & 0xF];
            hex[3] = 0;
            out.append(hex, strlen(hex));
        }
    }
    return out;
}

inline std::string 
urlFromPath(const std::string& s)
{
    // is this already a url?
    if (s.substr(0, strlen(FILE_URL_PREFIX)) == FILE_URL_PREFIX) {
        return s;
    }
    
    std::string delim(PATH_SEPARATOR);
    std::vector<std::string> edges = split(s, delim);
    
    std::string rval(FILE_URL_PREFIX);
    for (unsigned int i = 0; i < edges.size(); i++) {
#ifdef WIN32
        // leave DOS volumes alone
        if (i == 0 && edges[i].length() == 2 && edges[i][1] == ':') {
            rval.append("/");
            rval.append(edges[i]);
            continue;
        }
#endif
        if (edges[i].length() > 0) {
            rval.append("/");
            rval.append(urlEncode(edges[i]));
        }
    }
    return rval;
}


inline const char *
bplus::typeAsString(BPType t)
{
    switch (t) {
        case BPTNull: return "null";
        case BPTBoolean: return "boolean";
        case BPTInteger: return "integer";
        case BPTDouble: return "double";
        case BPTString: return "string";
        case BPTMap: return "map";
        case BPTList: return "list";
        case BPTCallBack: return "callback";
        case BPTPath: return "path";
        case BPTAny: return "any";
    }
    return "unknown";
}


inline bplus::Object::Object(BPType t)
{
    e.type = t;
}

inline bplus::Object::~Object()
{
}

inline BPType
bplus::Object::type() const
{
    return e.type;
}

inline bool
bplus::Object::has(const char * path, BPType type) const
{
    const bplus::Object * obj = get(path);
    return ((obj != NULL) && obj->type() == type);
}

inline bool
bplus::Object::has(const char * path) const
{
    return (get(path) != NULL);
}

inline void
bplus::Object::attachNode(const char * path,
                       Object * node)
{
    // XXX
}

inline const bplus::Object *
bplus::Object::get(const char * path) const
{
    const Object * obj = NULL;

    if (path == NULL) return obj;
    
    std::vector<std::string> paths = split(std::string(path), "/");

    obj = this;

    for (unsigned int i = 0; i < paths.size(); i++) {
        if (obj->type() != BPTMap) {
            obj = NULL;
            break;
        }
        const Object * oldobj = obj;        
        obj = NULL;
        for (unsigned int j = 0; j < oldobj->e.value.mapVal.size; j++)
        {
            if (!paths[i].compare(oldobj->e.value.mapVal.elements[j].key))
            {
                obj = static_cast<const bplus::Map *>(oldobj)->values[j];
                break;
            }
        }
        if (obj == NULL) break;
    }
    
    return obj;
}

inline const BPElement *
bplus::Object::elemPtr() const
{
    return &e;
}

inline bplus::Object *
bplus::Object::build(const BPElement * elem)
{
    Object * obj = NULL;

    if (elem != NULL)
    {
        switch (elem->type)
        {
            case BPTNull:
                obj = new bplus::Null;
                break;
            case BPTBoolean:
                obj = new bplus::Bool(elem->value.booleanVal);
                break;
            case BPTInteger:
                obj = new bplus::Integer(elem->value.integerVal);
                break;
            case BPTCallBack:
                obj = new bplus::CallBack(elem->value.callbackVal);
                break;
            case BPTDouble:
                obj = new bplus::Double(elem->value.doubleVal);
                break;
            case BPTString:
                obj = new bplus::String(elem->value.stringVal);
                break;
            case BPTPath:
                obj = new bplus::Path(elem->value.pathVal);
                break;
            case BPTMap:
            {
                bplus::Map * m = new bplus::Map;
                
                for (unsigned int i = 0; i < elem->value.mapVal.size; i++)
                {
                    m->add(elem->value.mapVal.elements[i].key,
                           build(elem->value.mapVal.elements[i].value));
                }

                obj = m;
                break;
            }
            case BPTList:
            {
                bplus::List * l = new bplus::List;
                
                for (unsigned int i = 0; i < elem->value.listVal.size; i++)
                {
                    l->append(build(elem->value.listVal.elements[i]));
                }

                obj = l;
                break;
            }
            case BPTAny: {
                // invalid
                break;
            }
        }
    }
    
    return obj;
}

inline const char *
bplus::Object::getStringNodeValue( const char * cszPath )
{
    const Object* pNode = get(cszPath);
    if (!pNode || pNode->type() != BPTString)
    {
        return NULL;
    }
    
    return static_cast<const bplus::String *>(pNode)->value();
}

inline bplus::Object::operator bool() const 
{
    throw ConversionException("cannot convert to bool");
}

inline bplus::Object::operator std::string() const 
{
    throw ConversionException("cannot convert to string");
}

inline bplus::Object::operator long long() const 
{
    throw ConversionException("cannot convert to long");
}

inline bplus::Object::operator double() const 
{
    throw ConversionException("cannot convert to double");
}

inline bplus::Object::operator std::map<std::string, const bplus::Object *>() const
{
    throw ConversionException("cannot convert to map<string, Object*>");
}
    

inline bplus::Object::operator std::vector<const bplus::Object *>() const
{
    throw ConversionException("cannot convert to vector<Object*>");
}

inline const bplus::Object &
bplus::Object::operator[](const char * key) const
{
    throw ConversionException("cannot apply operator[const char*]");
}

inline const bplus::Object &
bplus::Object::operator[](unsigned int index) const
{
    throw ConversionException("cannot apply operator[int]");
}

inline bplus::Null::Null()
    : bplus::Object(BPTNull) 
{
}

inline bplus::Null::~Null()
{
}

inline bplus::Object * 
bplus::Null::clone() const
{
    return new bplus::Null();
}
    
inline bplus::Bool::Bool(bool val)
    : bplus::Object(BPTBoolean) 
{
    e.value.booleanVal = val;
}

inline bplus::Bool::~Bool()
{
}

inline BPBool
bplus::Bool::value() const
{
    return e.value.booleanVal;
}

inline bplus::Object * 
bplus::Bool::clone() const
{
    return new bplus::Bool(value());
}

inline bplus::Bool::operator bool() const 
{
    return value();
}

inline bplus::String::String(const char * str)
    : bplus::Object(BPTString) 
{
    if (!str) str = "";
    this->str.append(str);
    e.value.stringVal = (char *) this->str.c_str();
}

inline bplus::String::String(const char * str, unsigned int len)
    : bplus::Object(BPTString) 
{
    this->str.append(str, len);
    e.value.stringVal = (char *) this->str.c_str();
}

inline bplus::String::String(const std::string & str)
    : bplus::Object(BPTString) 
{
    this->str = str;
    e.value.stringVal = (char *) this->str.c_str();
}

inline bplus::String::String(const String & other)
    : bplus::Object(BPTString)
{
    str = other.str;
    e.value.stringVal = (char *) this->str.c_str();
}

inline bplus::String &
bplus::String::operator= (const String & other)
{
    str = other.str;
    e.value.stringVal = (char *) this->str.c_str();
    return *this;
}

inline bplus::String::~String()
{
}

inline const BPString
bplus::String::value() const
{
    return e.value.stringVal;
}

inline bplus::Object * 
bplus::String::clone() const
{
    return new String(*this);
}

inline bplus::String::operator std::string() const 
{
    return std::string(value());
}

inline bplus::Path::Path(const char * str)
    : bplus::String(str) 
{
    e.type = BPTPath;
    this->str = urlFromPath(this->str);
    e.value.pathVal = (char *)this->str.c_str();
}

inline bplus::Path::Path(const char * str, unsigned int len)
    : bplus::String(str, len)
{
    e.type = BPTPath;
    this->str.clear();
    this->str.append(str, len);
    this->str = urlFromPath(this->str);
    e.value.pathVal = (char *)this->str.c_str();
}

inline bplus::Path::Path(const std::string & str)
    : bplus::String(str) 
{
    e.type = BPTPath;
    this->str = urlFromPath(this->str);
    e.value.pathVal = (char *)this->str.c_str();
}

inline bplus::Path::Path(const Path & other)
    : bplus::String(other.str)
{
    e.type = BPTPath;
}

inline bplus::Path &
bplus::Path::operator= (const Path & other)
{
    str = other.str;
    e.value.pathVal = (char *) str.c_str();
    return *this;
}

inline bplus::Path::~Path()
{
}

inline bplus::Object * 
bplus::Path::clone() const
{
    return new Path(this->str);
}

inline bplus::Map::Map() : bplus::Object(BPTMap) 
{
    e.value.mapVal.size = 0;
    e.value.mapVal.elements = NULL;
}

inline bplus::Map::Map(const Map & o) : bplus::Object(BPTMap) 
{
    e.value.mapVal.size = 0;
    e.value.mapVal.elements = NULL;

    Iterator i(o);
    const char * k;
    while (NULL != (k = i.nextKey())) {
        add(k, o.value(k)->clone());
    }
}

inline bplus::Map &
bplus::Map::operator= (const bplus::Map & o)
{
    for (unsigned int i = 0; i < values.size(); i++) delete values[i];
    if (e.value.mapVal.elements != NULL) free(e.value.mapVal.elements);
    memset((void *) &e, 0, sizeof(e));
    values.clear(); keys.clear();

    e.type = BPTMap;    
    Iterator i(o);
    const char * k;
    while (NULL != (k = i.nextKey())) {
        add(k, o.value(k)->clone());
    }

    return *this;
}

inline bplus::Object *
bplus::Map::clone() const
{
    return new Map(*this);
}

inline bplus::Map::~Map()
{
    for (unsigned int i = 0; i < values.size(); i++)
    {
        delete values[i];
    }
    
    if (e.value.mapVal.elements != NULL) 
    {
        free(e.value.mapVal.elements);
    }
}


inline unsigned int
bplus::Map::size() const
{
    return e.value.mapVal.size;
}


inline const bplus::Object *
bplus::Map::value(const char * key) const
{
	if (key == NULL) return NULL;
    unsigned int i;
    std::vector<std::string>::const_iterator it;
    for (i = 0, it = keys.begin(); it != keys.end(); it++, i++) {
        if (!strcmp(key, (*it).c_str()))
            return values[i];
    }
    return NULL;
}

inline const bplus::Object &
bplus::Map::operator[](const char * key) const
{
    const bplus::Object * v = value(key);
    if (v == NULL) {
        throw bplus::ConversionException("no such element in map");
    }
    return *v;
}

inline bool
bplus::Map::kill(const char * key)
{
    bool rval = false;
	if (key == NULL) return rval;
    std::vector<std::string>::iterator it;
    std::vector<bplus::Object*>::iterator vit;
    for (it = keys.begin(), vit = values.begin();
         it != keys.end() && vit != values.end();
         ++it, ++vit) {
        if (!strcmp(key, (*it).c_str())) {
            keys.erase(it);
            values.erase(vit);
            e.value.mapVal.size--;
            rval = true;
            break;
        }
    }
    
    // if we found and removed key, rebuild BPElements
    if (rval) {
        e.value.mapVal.elements =
            (BPMapElem *) realloc(e.value.mapVal.elements,
                                  sizeof(BPMapElem) * e.value.mapVal.size);
        for (unsigned int ix = 0; ix < e.value.mapVal.size; ix++)
        {
            e.value.mapVal.elements[ix].key = (BPString) keys[ix].c_str();
            e.value.mapVal.elements[ix].value = (BPElement *) values[ix]->elemPtr();

        }
    }
    return rval;
}

inline void
bplus::Map::add(const char * key, bplus::Object * value)
{
    assert(value != NULL);
    kill(key);
    unsigned int ix = e.value.mapVal.size;
    e.value.mapVal.size++;
    values.push_back(value);
    e.value.mapVal.elements =
        (BPMapElem *) realloc(e.value.mapVal.elements,
                              sizeof(BPMapElem) * e.value.mapVal.size);
    e.value.mapVal.elements[ix].value = (BPElement *) value->elemPtr();    
    // adding a key may cause some strings to be reallocated (!).  after
    // the addition we must update key ptrs
    keys.push_back(key);
	for (ix = 0; ix < e.value.mapVal.size; ix++)
	{
		e.value.mapVal.elements[ix].key = (BPString) keys[ix].c_str();
	}
}

inline void
bplus::Map::add(const std::string& key, bplus::Object* value)
{
    add(key.c_str(), value);
}

inline bool
bplus::Map::getBool(const std::string& sKey, bool& bValue) const
{
    if (has(sKey.c_str(), BPTBoolean)) {
        bValue = dynamic_cast<const bplus::Bool*>(get(sKey.c_str()))->value();
        return true;
    }

    return false;
}

inline bool
bplus::Map::getInteger(const std::string& sKey, int& nValue) const
{
    if (has(sKey.c_str(), BPTInteger)) {
        long long int lVal = dynamic_cast<const Integer*>(get(sKey.c_str()))->value();
        nValue = static_cast<int>(lVal);
        return true;
    }

    return false;
}
   
inline bool
bplus::Map::getList(const std::string& sKey, const bplus::List*& pList) const
{
    if (has(sKey.c_str(), BPTList)) {
        pList = dynamic_cast<const bplus::List*>(get(sKey.c_str()));
        return true;
    }

    return false;
}

inline bool
bplus::Map::getLong(const std::string& sKey, long long int& lValue) const
{
    if (has(sKey.c_str(), BPTInteger)) {
        lValue = dynamic_cast<const bplus::Integer*>(get(sKey.c_str()))->value();
        return true;
    }

    return false;
}

inline bool
bplus::Map::getMap(const std::string& sKey, const bplus::Map*& pMap) const
{
    if (has(sKey.c_str(), BPTMap)) {
        pMap = dynamic_cast<const bplus::Map*>(get(sKey.c_str()));
        return true;
    }

    return false;
}

inline bool
bplus::Map::getString(const std::string& sKey, std::string& sValue) const
{
    if (has(sKey.c_str(), BPTString)) {
        sValue = dynamic_cast<const bplus::String*>(get(sKey.c_str()))->value();
        return true;
    }

    return false;
}

inline bplus::Map::Iterator::Iterator(const class bplus::Map& m) {
    m_it = m.keys.begin();
    m_m = &m;
}

inline const char *
bplus::Map::Iterator::nextKey()
{
    if (m_it == m_m->keys.end()) return NULL;
    const char * key = (*m_it).c_str();
    m_it++;
    return key;
}

inline bplus::Map::operator std::map<std::string, const bplus::Object *>() const
{
    std::map<std::string, const bplus::Object *> m;
    Iterator i(*this);
    const char * k;
    while (NULL != (k = i.nextKey())) m[k] = value(k);
    return m;
    
}

inline bplus::Integer::Integer(BPInteger num)
    : Object(BPTInteger)
{
    e.value.integerVal = num;
}

inline bplus::Integer::~Integer()
{
}

inline BPInteger
bplus::Integer::value() const
{
    return e.value.integerVal;
}

inline bplus::Object * 
bplus::Integer::clone() const
{
    return new Integer(*this);
}

inline bplus::Integer::operator long long() const 
{
    return value();
}

inline bplus::CallBack::CallBack(BPCallBack cb) : Integer(cb)
{
    e.type = BPTCallBack;
}

inline bplus::Object * 
bplus::CallBack::clone() const
{
    return new CallBack(*this);
}

inline bplus::CallBack::~CallBack()
{
}

inline bplus::Double::Double(BPDouble num)
    : Object(BPTDouble)
{
    e.value.doubleVal = num;
}

inline bplus::Double::~Double()
{
}

inline BPDouble
bplus::Double::value() const
{
    return e.value.doubleVal;
}

inline bplus::Object *
bplus::Double::clone() const
{
    return new Double(*this);
}


inline bplus::Double::operator double() const 
{
    return value();
}

inline bplus::List::List() : Object(BPTList)
{
    e.value.listVal.size = 0;
    e.value.listVal.elements = NULL;
}

inline bplus::List::List(const List & other) : Object(BPTList)
{
    e.value.listVal.size = 0;
    e.value.listVal.elements = NULL;

    for (unsigned int i = 0; i < other.size(); i++) {
        append(other.value(i)->clone());
    }
}

inline bplus::List &
bplus::List::operator= (const List & other)
{
    // release
    for (unsigned int i = 0; i < values.size(); i++) delete values[i];
    if (e.value.listVal.elements != NULL) free(e.value.listVal.elements);

    // reinitialize
    memset((void *) &e, 0, sizeof(e));
    values.clear();
    e.type = BPTList;    

    // populate
    for (unsigned int i = 0; i < other.size(); i++) {
        append(other.value(i)->clone());
    }
    
    return *this;
}

inline bplus::List::~List()
{
    for (unsigned int i = 0; i < values.size(); i++)
    {
        delete values[i];
    }
    
    if (e.value.listVal.elements != NULL) 
    {
        free(e.value.listVal.elements);
    }
}

inline unsigned int
bplus::List::size() const
{
    return e.value.listVal.size;
}

inline const bplus::Object *
bplus::List::value(unsigned int i) const
{
    assert(e.value.listVal.size == values.size());
    if (i >= e.value.listVal.size) return NULL;
    return values[i];
}

inline const bplus::Object &
bplus::List::operator[](unsigned int index) const
{
    const bplus::Object * v = value(index);
    if (v == NULL) {
        throw bplus::ConversionException("no such element in list, range error");
    }
    
    return *v;
}

inline void
bplus::List::append(bplus::Object * object)
{
    assert(object != NULL);
    values.push_back(object);
    e.value.listVal.size++;
    e.value.listVal.elements = 
        (BPElement **) realloc(e.value.listVal.elements,
                               sizeof(BPElement *) * e.value.listVal.size);
    e.value.listVal.elements[e.value.listVal.size - 1] =
        (BPElement *) object->elemPtr();
}

inline bplus::Object *
bplus::List::clone() const
{
    return new bplus::List(*this);
}

inline bplus::List::operator std::vector<const bplus::Object *>() const
{
    std::vector<const Object *> v;
    for (unsigned int i = 0; i < size(); i++) v.push_back(value(i));
    return v;
}

#endif // BPTYPEUTILIMPL_H_
