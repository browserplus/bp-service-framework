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
 * bptypeutil.h -- c++ utilities to make building hierarchies of BPElements
 *                 eaiser.  A tool that may be consumed in source form
 *                 by a corelet author to simplify mapping into and out of
 *                 introspectable corelet API types.
 */

#ifndef BPTYPEUTIL_H_
#define BPTYPEUTIL_H_

#include <string>
#include <vector>
#include <map>
#include "bpserviceapi/bptypes.h"


namespace bplus {

    const char * typeAsString(BPType t);

    /**
     * An exception thrown when unsupported type conversions are attempted
     */
    class ConversionException
    {
      public:
        ConversionException(const std::string& desc) : m_desc(desc) { }
        std::string m_desc;
    };

    /**
     * bpu::Object is the common base class for all BPElements
     */
    class Object
    {
    public:
        virtual ~Object();
        
        BPType type() const;
    
        /**
         * check to see if a node has a descendant of a specified type
         * \param path path to node
         * \param type type of node
         */
        bool has(const char * path, BPType type) const;

        /** \overload (don't check type) */
        bool has(const char * path) const;

        /**  attach a supplied node at a specified path, creating as we go */  
        void attachNode(const char * path, Object * node);

        /**
         * Get a descendant node
         */
        const Object * get(const char * path) const;

        /**
         * Get the value from a descendant string node.
         * Returns null if the specified string node not present.
         */
        const char * getStringNodeValue( const char * path );
        
        /**
         * Build a hierarchy of objects from a BPElement pointer.
         * Caller owns returned pointer.
         */
        static Object * build(const BPElement * elem);

        const BPElement * elemPtr() const;

        /**
         * perform a deep copy
         */
        virtual Object * clone() const = 0;

        /**
         * easily access various data stored inside these types.
         * NOTE: all of these operator overloads throw, so you should
         *       perform validation before using these to access data.
         */
        virtual operator bool() const; // throw(ConversionException)
        virtual operator std::string() const; // throw(ConversionException)
        virtual operator long long() const; // throw(ConversionException)
        virtual operator double() const; // throw(ConversionException)
        virtual operator std::map<std::string, const Object *>() const;
            // throw(ConversionException)
        virtual operator std::vector<const Object *>() const;
            // throw(ConversionException)

        virtual const Object & operator[](const char * key) const;
        virtual const Object & operator[](unsigned int index) const;

      protected:
        BPElement e;
        Object(BPType t);
    };

    /** 
     * an object representing a NULL value.
     */
    class Null : public Object 
    {
    public:
        Null();
        // compiler generated copy and assignment operators
        virtual ~Null();
        virtual Object * clone() const;
    };
    
    class Bool : public Object
    {
    public:
        Bool(bool value);
        // compiler generated copy and assignment operators
        virtual ~Bool();
        BPBool value() const;
        operator bool() const; // throw(ConversionException)
        virtual Object * clone() const;
    private:
    };

    class String : public Object
    {
    public:
        String(const char * str);
        String(const char * str, unsigned int len);
        String(const std::string & str);
        String(const String &);
        String & operator= (const String & other);
        virtual ~String();
        const BPString value() const;
        // note: the returned pointer is to internal memory, and is
        // only valid for the lifetime of the object, or the invocation
        operator std::string() const; // throw(ConversionException)
        virtual Object * clone() const;
    protected:
        std::string str;
    };
    
    // Path represents a pathname in URI form.
    class Path : public String
    {
    public:
        Path(const char * str);
        Path(const char * str, unsigned int);
        Path(const std::string & str);
        Path(const Path &);
        Path & operator= (const Path &);

        virtual ~Path();
        virtual Object * clone() const;
    };

    class Integer : public Object
    {
    public:
        Integer(BPInteger num);
        // compiler generated assignment and copy
        virtual ~Integer();
        BPInteger value() const;
        virtual Object * clone() const;
        operator long long() const; // throw(ConversionException)
    };

    class CallBack : public Integer
    {
    public:
        CallBack(BPCallBack cb);
        // compiler generated assignment and copy
        virtual ~CallBack();
        virtual Object * clone() const;
    };

    class Double : public Object
    {
    public:
        Double(BPDouble num);
        // compiler generated assignment and copy
        virtual ~Double();
        BPDouble value() const;
        virtual Object * clone() const;
        operator double() const; // throw(ConversionException)
    };
        
    class List : public Object
    {
    public:
        List();
        List(const List &);
        List & operator= (const List & other);
        virtual ~List();
        unsigned int size() const;
        
        /** access a value by key */
        const Object * value(unsigned int i) const;
        
        void append(Object * object);
        
        virtual Object * clone() const;
        
        // get a vector of const pointers.
        operator std::vector<const Object *>() const; // throw(ConversionException);
        
        virtual const Object & operator[](unsigned int index) const; // throw();
    private:
        std::vector<Object *> values;
    };

    class Map : public Object
    {
    public:
        Map();
        Map(const Map &);
        Map & operator= (const Map & other);
        virtual ~Map();

        unsigned int size() const;

        /** access a value by key */
        const Object * value(const char * key) const;

        /** delete key/value pair from the map
         *  \returns false if the key is not present */
        bool kill(const char * key);

        /** add a key/value pair to the map, if the key already
         *  exists, the value will be overwritten. 
         *  the map retains ownership of the Object and will free
         *  it at the point the map is freed */
        void add(const char * key, Object * value);
        
        /**
         * overload that works with STL strings
         */
        void add(const std::string& key, Object* value);
        
        /**
         * Get a boolean from the map.
         * Returns true iff boolean val with key name present.
         */
        bool getBool(const std::string& key, bool& bValue) const;

        /**
         * Get an integer from the map.
         * Returns true iff integer val with key name present.
         */
        bool getInteger(const std::string& key, int& nValue) const;

        /**
         * Get a List from the map.
         * Returns true iff integer val with key name present.
         */
        bool getList(const std::string& key, const List*& list) const;

        /**
         * Get a native long long integer from the map.
         * Returns true iff integer val with key name present.
         */
        bool getLong(const std::string& key, long long int& lValue) const;

        /**
         * Get a Map from the map.
         * Returns true iff integer val with key name present.
         */
        bool getMap(const std::string& key, const Map*& pMap) const;

        /**
         * Get a string from the map.
         * Returns true iff string val with key name present.
         */
        bool getString(const std::string& key, std::string& sValue) const;

        /** A mechanism to traverse all of the keys present */
        class Iterator {
          public:
            Iterator(const Map& m);
            const char * nextKey();
          private:
            std::vector<std::string>::const_iterator m_it;
            const Map * m_m;
        };

        virtual operator std::map<std::string, const bplus::Object *>() const;

        virtual const Object & operator[](const char * key) const;

        virtual Object * clone() const;
    private:
        std::vector<Object *> values;
        std::vector<std::string> keys;
        friend class bplus::Object;
    };
    
    /* Create a BP object from a Map.
     * Returns NULL if map doesn't describe 
     * a known BP object.
     */
    Object * createBPObject(const Map * map);
};


//////////////////////////////////////////////////////////////////////
// Get the implementations.
#include "impl/bptypeutilimpl.h"


#endif // BPTYPEUTIL_H_
