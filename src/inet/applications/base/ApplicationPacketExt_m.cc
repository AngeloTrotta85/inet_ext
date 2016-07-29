//
// Generated file, do not edit! Created by nedtool 5.0 from inet/applications/base/ApplicationPacketExt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ApplicationPacketExt_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

nodeinfo::nodeinfo()
{
    this->nodeDegree = 0;
    this->meanSnrNeighbourood = 0;
    this->meanPowNeighbourood = 0;
}

void __doPacking(omnetpp::cCommBuffer *b, const nodeinfo& a)
{
    doParsimPacking(b,a.localAddr);
    doParsimPacking(b,a.pos);
    doParsimPacking(b,a.velocity);
    doParsimPacking(b,a.nodeDegree);
    doParsimPacking(b,a.meanSnrNeighbourood);
    doParsimPacking(b,a.meanPowNeighbourood);
}

void __doUnpacking(omnetpp::cCommBuffer *b, nodeinfo& a)
{
    doParsimUnpacking(b,a.localAddr);
    doParsimUnpacking(b,a.pos);
    doParsimUnpacking(b,a.velocity);
    doParsimUnpacking(b,a.nodeDegree);
    doParsimUnpacking(b,a.meanSnrNeighbourood);
    doParsimUnpacking(b,a.meanPowNeighbourood);
}

class nodeinfoDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    nodeinfoDescriptor();
    virtual ~nodeinfoDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(nodeinfoDescriptor);

nodeinfoDescriptor::nodeinfoDescriptor() : omnetpp::cClassDescriptor("inet::nodeinfo", "")
{
    propertynames = nullptr;
}

nodeinfoDescriptor::~nodeinfoDescriptor()
{
    delete[] propertynames;
}

bool nodeinfoDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<nodeinfo *>(obj)!=nullptr;
}

const char **nodeinfoDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *nodeinfoDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int nodeinfoDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 6+basedesc->getFieldCount() : 6;
}

unsigned int nodeinfoDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<6) ? fieldTypeFlags[field] : 0;
}

const char *nodeinfoDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "localAddr",
        "pos",
        "velocity",
        "nodeDegree",
        "meanSnrNeighbourood",
        "meanPowNeighbourood",
    };
    return (field>=0 && field<6) ? fieldNames[field] : nullptr;
}

int nodeinfoDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='l' && strcmp(fieldName, "localAddr")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pos")==0) return base+1;
    if (fieldName[0]=='v' && strcmp(fieldName, "velocity")==0) return base+2;
    if (fieldName[0]=='n' && strcmp(fieldName, "nodeDegree")==0) return base+3;
    if (fieldName[0]=='m' && strcmp(fieldName, "meanSnrNeighbourood")==0) return base+4;
    if (fieldName[0]=='m' && strcmp(fieldName, "meanPowNeighbourood")==0) return base+5;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *nodeinfoDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "L3Address",
        "Coord",
        "Coord",
        "int",
        "double",
        "double",
    };
    return (field>=0 && field<6) ? fieldTypeStrings[field] : nullptr;
}

const char **nodeinfoDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *nodeinfoDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int nodeinfoDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    nodeinfo *pp = (nodeinfo *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string nodeinfoDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    nodeinfo *pp = (nodeinfo *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->localAddr; return out.str();}
        case 1: {std::stringstream out; out << pp->pos; return out.str();}
        case 2: {std::stringstream out; out << pp->velocity; return out.str();}
        case 3: return long2string(pp->nodeDegree);
        case 4: return double2string(pp->meanSnrNeighbourood);
        case 5: return double2string(pp->meanPowNeighbourood);
        default: return "";
    }
}

bool nodeinfoDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    nodeinfo *pp = (nodeinfo *)object; (void)pp;
    switch (field) {
        case 3: pp->nodeDegree = string2long(value); return true;
        case 4: pp->meanSnrNeighbourood = string2double(value); return true;
        case 5: pp->meanPowNeighbourood = string2double(value); return true;
        default: return false;
    }
}

const char *nodeinfoDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(L3Address));
        case 1: return omnetpp::opp_typename(typeid(Coord));
        case 2: return omnetpp::opp_typename(typeid(Coord));
        default: return nullptr;
    };
}

void *nodeinfoDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    nodeinfo *pp = (nodeinfo *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->localAddr); break;
        case 1: return (void *)(&pp->pos); break;
        case 2: return (void *)(&pp->velocity); break;
        default: return nullptr;
    }
}

Register_Class(ApplicationPacketExt);

ApplicationPacketExt::ApplicationPacketExt(const char *name, int kind) : ::omnetpp::cPacket(name,kind)
{
    this->sequenceNumber = 0;
    neighbours_arraysize = 0;
    this->neighbours = 0;
}

ApplicationPacketExt::ApplicationPacketExt(const ApplicationPacketExt& other) : ::omnetpp::cPacket(other)
{
    neighbours_arraysize = 0;
    this->neighbours = 0;
    copy(other);
}

ApplicationPacketExt::~ApplicationPacketExt()
{
    delete [] this->neighbours;
}

ApplicationPacketExt& ApplicationPacketExt::operator=(const ApplicationPacketExt& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ApplicationPacketExt::copy(const ApplicationPacketExt& other)
{
    this->sequenceNumber = other.sequenceNumber;
    this->myInfo = other.myInfo;
    delete [] this->neighbours;
    this->neighbours = (other.neighbours_arraysize==0) ? nullptr : new nodeinfo[other.neighbours_arraysize];
    neighbours_arraysize = other.neighbours_arraysize;
    for (unsigned int i=0; i<neighbours_arraysize; i++)
        this->neighbours[i] = other.neighbours[i];
}

void ApplicationPacketExt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->myInfo);
    b->pack(neighbours_arraysize);
    doParsimArrayPacking(b,this->neighbours,neighbours_arraysize);
}

void ApplicationPacketExt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->myInfo);
    delete [] this->neighbours;
    b->unpack(neighbours_arraysize);
    if (neighbours_arraysize==0) {
        this->neighbours = 0;
    } else {
        this->neighbours = new nodeinfo[neighbours_arraysize];
        doParsimArrayUnpacking(b,this->neighbours,neighbours_arraysize);
    }
}

long ApplicationPacketExt::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void ApplicationPacketExt::setSequenceNumber(long sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

nodeinfo& ApplicationPacketExt::getMyInfo()
{
    return this->myInfo;
}

void ApplicationPacketExt::setMyInfo(const nodeinfo& myInfo)
{
    this->myInfo = myInfo;
}

void ApplicationPacketExt::setNeighboursArraySize(unsigned int size)
{
    nodeinfo *neighbours2 = (size==0) ? nullptr : new nodeinfo[size];
    unsigned int sz = neighbours_arraysize < size ? neighbours_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        neighbours2[i] = this->neighbours[i];
    neighbours_arraysize = size;
    delete [] this->neighbours;
    this->neighbours = neighbours2;
}

unsigned int ApplicationPacketExt::getNeighboursArraySize() const
{
    return neighbours_arraysize;
}

nodeinfo& ApplicationPacketExt::getNeighbours(unsigned int k)
{
    if (k>=neighbours_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", neighbours_arraysize, k);
    return this->neighbours[k];
}

void ApplicationPacketExt::setNeighbours(unsigned int k, const nodeinfo& neighbours)
{
    if (k>=neighbours_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", neighbours_arraysize, k);
    this->neighbours[k] = neighbours;
}

class ApplicationPacketExtDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    ApplicationPacketExtDescriptor();
    virtual ~ApplicationPacketExtDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(ApplicationPacketExtDescriptor);

ApplicationPacketExtDescriptor::ApplicationPacketExtDescriptor() : omnetpp::cClassDescriptor("inet::ApplicationPacketExt", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

ApplicationPacketExtDescriptor::~ApplicationPacketExtDescriptor()
{
    delete[] propertynames;
}

bool ApplicationPacketExtDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ApplicationPacketExt *>(obj)!=nullptr;
}

const char **ApplicationPacketExtDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *ApplicationPacketExtDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int ApplicationPacketExtDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int ApplicationPacketExtDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *ApplicationPacketExtDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sequenceNumber",
        "myInfo",
        "neighbours",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int ApplicationPacketExtDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sequenceNumber")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "myInfo")==0) return base+1;
    if (fieldName[0]=='n' && strcmp(fieldName, "neighbours")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *ApplicationPacketExtDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "long",
        "nodeinfo",
        "nodeinfo",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **ApplicationPacketExtDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ApplicationPacketExtDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ApplicationPacketExtDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    ApplicationPacketExt *pp = (ApplicationPacketExt *)object; (void)pp;
    switch (field) {
        case 2: return pp->getNeighboursArraySize();
        default: return 0;
    }
}

std::string ApplicationPacketExtDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ApplicationPacketExt *pp = (ApplicationPacketExt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSequenceNumber());
        case 1: {std::stringstream out; out << pp->getMyInfo(); return out.str();}
        case 2: {std::stringstream out; out << pp->getNeighbours(i); return out.str();}
        default: return "";
    }
}

bool ApplicationPacketExtDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    ApplicationPacketExt *pp = (ApplicationPacketExt *)object; (void)pp;
    switch (field) {
        case 0: pp->setSequenceNumber(string2long(value)); return true;
        default: return false;
    }
}

const char *ApplicationPacketExtDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 1: return omnetpp::opp_typename(typeid(nodeinfo));
        case 2: return omnetpp::opp_typename(typeid(nodeinfo));
        default: return nullptr;
    };
}

void *ApplicationPacketExtDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    ApplicationPacketExt *pp = (ApplicationPacketExt *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getMyInfo()); break;
        case 2: return (void *)(&pp->getNeighbours(i)); break;
        default: return nullptr;
    }
}

} // namespace inet

