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
 * Portions created by Yahoo! are Copyright (c) 2010 Yahoo! Inc.
 * All rights reserved.
 * 
 * Contributor(s): 
 * ***** END LICENSE BLOCK *****
 */

/**
 *  bptypeutilimpl.h
 * 
 *  Inline implementation file for bptypeutil.h.
 *
 *  Note: This file is included by bptypeutil.h.
 *        It is not intended for direct inclusion by client code.
 */

#include <assert.h>
#include "bputil/bpstrutil.h"


#ifdef WIN32
#pragma warning(disable:4100)
#endif


namespace bplus {


const char *
typeAsString(BPType t)
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
        case BPTNativePath: return "path";
        case BPTWritableNativePath: return "writablePath";
        case BPTAny: return "any";
    }
    return "unknown";
}


inline
Object::Object(BPType t)
{
    e.type = t;
}

inline
Object::~Object()
{
}


inline BPType
Object::type() const
{
    return e.type;
}

inline bool
Object::has(const char * path, BPType type) const
{
    const Object * obj = get(path);
    return ((obj != NULL) && obj->type() == type);
}

inline bool
Object::has(const char * path) const
{
    return (get(path) != NULL);
}

inline const Object *
Object::get(const char * path) const
{
    const Object * obj = NULL;

    if (path == NULL) return obj;
    
    std::vector<std::string> paths =
        strutil::split(std::string(path), "/");

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
                obj = static_cast<const Map *>(oldobj)->values[j];
                break;
            }
        }
        if (obj == NULL) break;
    }
    
    return obj;
}

inline const BPElement *
Object::elemPtr() const
{
    return &e;
}

inline Object *
Object::build(const BPElement * elem)
{
    Object * obj = NULL;

    if (elem != NULL)
    {
        switch (elem->type)
        {
            case BPTNull:
                obj = new Null;
                break;
            case BPTBoolean:
                obj = new Bool(elem->value.booleanVal);
                break;
            case BPTInteger:
                obj = new Integer(elem->value.integerVal);
                break;
            case BPTCallBack:
                obj = new CallBack(elem->value.callbackVal);
                break;
            case BPTDouble:
                obj = new Double(elem->value.doubleVal);
                break;
            case BPTString:
                obj = new String(elem->value.stringVal);
                break;
            case BPTNativePath: 
            {
//              obj = new Path(file::Path(elem->value.pathVal));
                obj = new Path(tPathString(elem->value.pathVal));
                break;
            }
            case BPTWritableNativePath: 
            {
                obj = new WritablePath(tPathString(elem->value.pathVal));
                break;
            }
            case BPTMap:
            {
                Map * m = new Map;
                
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
                List * l = new List;
                
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
Object::getStringNodeValue( const char * cszPath )
{
    const Object* pNode = get(cszPath);
    if (!pNode || pNode->type() != BPTString)
    {
        return NULL;
    }
    
    return static_cast<const String *>(pNode)->value();
}

inline 
Object::operator bool() const 
{
    throw ConversionException("cannot convert to bool");
}

inline 
Object::operator std::string() const 
{
    throw ConversionException("cannot convert to string");
}

#if defined(WIN32) || defined(WINDOWS) || defined(_WINDOWS)
inline
//Object::operator file::Path() const 
Object::operator tPathString() const 
{
    throw ConversionException("cannot convert to path");
}
#endif

inline
Object::operator long long() const 
{
    throw ConversionException("cannot convert to long");
}

inline 
Object::operator double() const 
{
    throw ConversionException("cannot convert to double");
}

inline 
Object::operator std::map<std::string, const Object *>() const
{
    throw ConversionException("cannot convert to map<string, Object*>");
}
    
inline 
Object::operator std::vector<const Object *>() const
{
    throw ConversionException("cannot convert to vector<Object*>");
}

inline const Object &
Object::operator[](const char *) const
{
    throw ConversionException("cannot apply operator[const char*]");
}

inline const Object &
Object::operator[](unsigned int) const
{
    throw ConversionException("cannot apply operator[int]");
}

inline 
Null::Null()
    : Object(BPTNull) 
{
}

inline 
Null::~Null()
{
}

inline Object * 
Null::clone() const
{
    return new Null();
}

inline 
Bool::Bool(bool val)
    : Object(BPTBoolean) 
{
    e.value.booleanVal = val;
}

inline 
Bool::~Bool()
{
}

inline BPBool
Bool::value() const
{
    return e.value.booleanVal;
}

inline Object * 
Bool::clone() const
{
    return new Bool(value());
}

inline 
Bool::operator bool() const 
{
    return value();
}

inline 
String::String(const char * str)
    : Object(BPTString) 
{
    if (!str) str = "";
    this->str.append(str);
    e.value.stringVal = (char *) this->str.c_str();
}

inline 
String::String(const char * str, unsigned int len)
    : Object(BPTString) 
{
    this->str.append(str, len);
    e.value.stringVal = (char *) this->str.c_str();
}

inline 
String::String(const std::string & str)
    : Object(BPTString) 
{
    this->str = str;
    e.value.stringVal = (char *) this->str.c_str();
}

inline 
String::String(const String & other)
    : Object(BPTString)
{
    str = other.str;
    e.value.stringVal = (char *) this->str.c_str();
}

inline String &
String::operator= (const String & other)
{
    str = other.str;
    e.value.stringVal = (char *) this->str.c_str();
    return *this;
}

inline 
String::~String()
{
}

inline const BPString
String::value() const
{
    return e.value.stringVal;
}

inline Object * 
String::clone() const
{
    return new String(*this);
}

inline 
String::operator std::string() const 
{
    return std::string(value());
}

inline 
//WritablePath::WritablePath(const file::Path & path)
WritablePath::WritablePath(const tPathString& path)
    : Path(path)
{
    e.type = BPTWritableNativePath;
}

inline 
WritablePath::WritablePath(const WritablePath & other)
    :  Path(other)
{
    e.type = BPTWritableNativePath;
}

inline WritablePath &
WritablePath::operator= (const WritablePath & other)
{
    m_path = other.m_path;
    e.value.pathVal = (BPPath) m_path.c_str();
    return *this;
}

inline Object * 
WritablePath::clone() const
{
    return new WritablePath(*this);
}

/*
inline 
Path::Path(const file::Path & path)
    : Object(BPTNativePath), m_path(path.external_file_string())
{
    e.value.pathVal = (BPPath) m_path.c_str();
}
*/
inline 
Path::Path(const tPathString& path)
   : Object(BPTNativePath), m_path(path)
{
    e.value.pathVal = (BPPath) m_path.c_str();
}

inline 
Path::Path(const Path & other)
    :  Object(BPTNativePath), m_path(other.m_path)
{
    e.type = BPTNativePath;
    e.value.pathVal = (BPPath) m_path.c_str();
}

inline const BPPath
Path::value() const
{
    return e.value.pathVal;
}

inline Path &
Path::operator= (const Path & other)
{
    m_path = other.m_path;
    e.value.pathVal = (BPPath) m_path.c_str();
    return *this;
}

/*
inline 
Path::operator file::Path() const 
{
	return file::Path(m_path);
}
*/
inline 
Path::operator tPathString() const 
{
    return m_path;
}


inline 
Path::~Path()
{
}

inline Object * 
Path::clone() const
{
// 2009apr29 dg
//  return new Path(this->str);
    return new Path(*this);
}

inline 
Map::Map() : Object(BPTMap) 
{
    e.value.mapVal.size = 0;
    e.value.mapVal.elements = NULL;
}

inline 
Map::Map(const Map & o) : Object(BPTMap) 
{
    e.value.mapVal.size = 0;
    e.value.mapVal.elements = NULL;

    Iterator i(o);
    const char * k;
    while (NULL != (k = i.nextKey())) {
        add(k, o.value(k)->clone());
    }
}

inline Map &
Map::operator= (const Map & o)
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

inline Object *
Map::clone() const
{
    return new Map(*this);
}

inline 
Map::~Map()
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
Map::size() const
{
    return e.value.mapVal.size;
}


inline const Object *
Map::value(const char * key) const
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

inline const Object &
Map::operator[](const char * key) const
{
    const Object * v = value(key);
    if (v == NULL) {
        throw bplus::ConversionException("no such element in map");
    }
    return *v;
}

inline bool
Map::kill(const char * key)
{
    bool rval = false;
	if (key == NULL) return rval;
    std::vector<std::string>::iterator it;
    std::vector<Object*>::iterator vit;
    for (it = keys.begin(), vit = values.begin();
         it != keys.end() && vit != values.end();
         ++it, ++vit) {
        if (!strcmp(key, (*it).c_str())) {
            keys.erase(it);
            delete *vit;
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
Map::add(const char * key, Object * value)
{
//  BPASSERT(value != NULL);
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
Map::add(const std::string& key, Object* value)
{
    add(key.c_str(), value);
}

inline bool
Map::getBool(const std::string& sPath, bool& bValue) const
{
    if (has(sPath.c_str(), BPTBoolean)) {
        bValue = dynamic_cast<const Bool*>(get(sPath.c_str()))->value();
        return true;
    }

    return false;
}

inline bool
Map::getInteger(const std::string& sPath, int& nValue) const
{
    if (has(sPath.c_str(), BPTInteger)) {
        long long int lVal = dynamic_cast<const Integer*>(get(sPath.c_str()))->value();
        nValue = static_cast<int>(lVal);
        return true;
    }

    return false;
}
   
inline bool
Map::getList(const std::string& sPath, const List*& pList) const
{
    if (has(sPath.c_str(), BPTList)) {
        pList = dynamic_cast<const List*>(get(sPath.c_str()));
        return true;
    }

    return false;
}

inline bool
Map::getLong(const std::string& sPath, long long int& lValue) const
{
    if (has(sPath.c_str(), BPTInteger)) {
        lValue = dynamic_cast<const Integer*>(get(sPath.c_str()))->value();
        return true;
    }

    return false;
}

inline bool
Map::getMap(const std::string& sPath, const Map*& pMap) const
{
    if (has(sPath.c_str(), BPTMap)) {
        pMap = dynamic_cast<const Map*>(get(sPath.c_str()));
        return true;
    }

    return false;
}

inline bool
Map::getString(const std::string& sPath, std::string& sValue) const
{
    if (has(sPath.c_str(), BPTString)) {
        sValue = dynamic_cast<const String*>(get(sPath.c_str()))->value();
        return true;
    }

    return false;
}

inline
//Map::Iterator::Iterator(const class Map& m) {
Map::Iterator::Iterator(const Map& m) {
    m_it = m.keys.begin();
    m_m = &m;
}

inline const char *
Map::Iterator::nextKey()
{
    if (m_it == m_m->keys.end()) return NULL;
    const char * key = (*m_it).c_str();
    m_it++;
    return key;
}

inline 
Map::operator std::map<std::string, const Object *>() const
{
    std::map<std::string, const Object *> m;
    Iterator i(*this);
    const char * k;
    while (NULL != (k = i.nextKey())) m[k] = value(k);
    return m;
    
}

inline 
Integer::Integer(BPInteger num)
    : Object(BPTInteger)
{
    e.value.integerVal = num;
}

inline 
Integer::~Integer()
{
}

inline BPInteger
Integer::value() const
{
    return e.value.integerVal;
}

inline Object * 
Integer::clone() const
{
    return new Integer(*this);
}

inline 
Integer::operator long long() const 
{
    return value();
}

inline 
CallBack::CallBack(BPCallBack cb) : Integer(cb)
{
    e.type = BPTCallBack;
}

inline Object * 
CallBack::clone() const
{
    return new CallBack(*this);
}

inline
CallBack::~CallBack()
{
}

inline 
Double::Double(BPDouble num)
    : Object(BPTDouble)
{
    e.value.doubleVal = num;
}

inline 
Double::~Double()
{
}

inline BPDouble
Double::value() const
{
    return e.value.doubleVal;
}

inline Object *
Double::clone() const
{
    return new Double(*this);
}

inline 
Double::operator double() const 
{
    return value();
}

inline 
List::List() : Object(BPTList)
{
    e.value.listVal.size = 0;
    e.value.listVal.elements = NULL;
}

inline 
List::List(const List & other) : Object(BPTList)
{
    e.value.listVal.size = 0;
    e.value.listVal.elements = NULL;

    for (unsigned int i = 0; i < other.size(); i++) {
        append(other.value(i)->clone());
    }
}

inline List &
List::operator= (const List & other)
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

inline
List::~List()
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
List::size() const
{
    return e.value.listVal.size;
}

inline const Object *
List::value(unsigned int i) const
{
//  BPASSERT(e.value.listVal.size == values.size());
    assert(e.value.listVal.size == values.size());
    if (i >= e.value.listVal.size) return NULL;
    return values[i];
}

inline const Object &
List::operator[](unsigned int index) const
{
    const Object * v = value(index);
    if (v == NULL) {
        throw bplus::ConversionException("no such element in list, range error");
    }
    
    return *v;
}

inline void
List::append(Object * object)
{
//  BPASSERT(object != NULL);
    assert(object != NULL);
    values.push_back(object);
    e.value.listVal.size++;
    e.value.listVal.elements = 
        (BPElement **) realloc(e.value.listVal.elements,
                               sizeof(BPElement *) * e.value.listVal.size);
    e.value.listVal.elements[e.value.listVal.size - 1] =
        (BPElement *) object->elemPtr();
}

inline Object *
List::clone() const
{
    return new List(*this);
}

inline 
List::operator std::vector<const Object *>() const
{
    std::vector<const Object *> v;
    for (unsigned int i = 0; i < size(); i++) v.push_back(value(i));
    return v;
}


} // namespace bplus
