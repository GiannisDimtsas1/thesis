//
// Generated file, do not edit! Created by nedtool 4.6 from src/node/communication/routing/myRoutingProtocol/MyRoutingProtocolPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "MyRoutingProtocolPacket_m.h"

USING_NAMESPACE


// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




// Template rule for outputting std::vector<T> types
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

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

EXECUTE_ON_STARTUP(
    cEnum *e = cEnum::find("MyRoutingProtocolPacketDef");
    if (!e) enums.getInstance()->add(e = new cEnum("MyRoutingProtocolPacketDef"));
    e->insert(MY_ROUTING_ADV_PACKET, "MY_ROUTING_ADV_PACKET");
    e->insert(MY_ROUTING_JOIN_PACKET, "MY_ROUTING_JOIN_PACKET");
    e->insert(MY_ROUTING_TDMA_PACKET, "MY_ROUTING_TDMA_PACKET");
    e->insert(MY_ROUTING_DATA_PACKET, "MY_ROUTING_DATA_PACKET");
);

Register_Class(MyRoutingProtocolPacket);

MyRoutingProtocolPacket::MyRoutingProtocolPacket(const char *name, int kind) : ::RoutingPacket(name,kind)
{
    this->MyRoutingProtocolPacketKind_var = 0;
    schedule_arraysize = 0;
    this->schedule_var = 0;
}

MyRoutingProtocolPacket::MyRoutingProtocolPacket(const MyRoutingProtocolPacket& other) : ::RoutingPacket(other)
{
    schedule_arraysize = 0;
    this->schedule_var = 0;
    copy(other);
}

MyRoutingProtocolPacket::~MyRoutingProtocolPacket()
{
    delete [] schedule_var;
}

MyRoutingProtocolPacket& MyRoutingProtocolPacket::operator=(const MyRoutingProtocolPacket& other)
{
    if (this==&other) return *this;
    ::RoutingPacket::operator=(other);
    copy(other);
    return *this;
}

void MyRoutingProtocolPacket::copy(const MyRoutingProtocolPacket& other)
{
    this->MyRoutingProtocolPacketKind_var = other.MyRoutingProtocolPacketKind_var;
    delete [] this->schedule_var;
    this->schedule_var = (other.schedule_arraysize==0) ? NULL : new int[other.schedule_arraysize];
    schedule_arraysize = other.schedule_arraysize;
    for (unsigned int i=0; i<schedule_arraysize; i++)
        this->schedule_var[i] = other.schedule_var[i];
}

void MyRoutingProtocolPacket::parsimPack(cCommBuffer *b)
{
    ::RoutingPacket::parsimPack(b);
    doPacking(b,this->MyRoutingProtocolPacketKind_var);
    b->pack(schedule_arraysize);
    doPacking(b,this->schedule_var,schedule_arraysize);
}

void MyRoutingProtocolPacket::parsimUnpack(cCommBuffer *b)
{
    ::RoutingPacket::parsimUnpack(b);
    doUnpacking(b,this->MyRoutingProtocolPacketKind_var);
    delete [] this->schedule_var;
    b->unpack(schedule_arraysize);
    if (schedule_arraysize==0) {
        this->schedule_var = 0;
    } else {
        this->schedule_var = new int[schedule_arraysize];
        doUnpacking(b,this->schedule_var,schedule_arraysize);
    }
}

int MyRoutingProtocolPacket::getMyRoutingProtocolPacketKind() const
{
    return MyRoutingProtocolPacketKind_var;
}

void MyRoutingProtocolPacket::setMyRoutingProtocolPacketKind(int MyRoutingProtocolPacketKind)
{
    this->MyRoutingProtocolPacketKind_var = MyRoutingProtocolPacketKind;
}

void MyRoutingProtocolPacket::setScheduleArraySize(unsigned int size)
{
    int *schedule_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = schedule_arraysize < size ? schedule_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        schedule_var2[i] = this->schedule_var[i];
    for (unsigned int i=sz; i<size; i++)
        schedule_var2[i] = 0;
    schedule_arraysize = size;
    delete [] this->schedule_var;
    this->schedule_var = schedule_var2;
}

unsigned int MyRoutingProtocolPacket::getScheduleArraySize() const
{
    return schedule_arraysize;
}

int MyRoutingProtocolPacket::getSchedule(unsigned int k) const
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    return schedule_var[k];
}

void MyRoutingProtocolPacket::setSchedule(unsigned int k, int schedule)
{
    if (k>=schedule_arraysize) throw cRuntimeError("Array of size %d indexed by %d", schedule_arraysize, k);
    this->schedule_var[k] = schedule;
}

class MyRoutingProtocolPacketDescriptor : public cClassDescriptor
{
  public:
    MyRoutingProtocolPacketDescriptor();
    virtual ~MyRoutingProtocolPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(MyRoutingProtocolPacketDescriptor);

MyRoutingProtocolPacketDescriptor::MyRoutingProtocolPacketDescriptor() : cClassDescriptor("MyRoutingProtocolPacket", "RoutingPacket")
{
}

MyRoutingProtocolPacketDescriptor::~MyRoutingProtocolPacketDescriptor()
{
}

bool MyRoutingProtocolPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<MyRoutingProtocolPacket *>(obj)!=NULL;
}

const char *MyRoutingProtocolPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int MyRoutingProtocolPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int MyRoutingProtocolPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *MyRoutingProtocolPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "MyRoutingProtocolPacketKind",
        "schedule",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int MyRoutingProtocolPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='M' && strcmp(fieldName, "MyRoutingProtocolPacketKind")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "schedule")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *MyRoutingProtocolPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *MyRoutingProtocolPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "MyRoutingProtocolPacketDef";
            return NULL;
        default: return NULL;
    }
}

int MyRoutingProtocolPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    MyRoutingProtocolPacket *pp = (MyRoutingProtocolPacket *)object; (void)pp;
    switch (field) {
        case 1: return pp->getScheduleArraySize();
        default: return 0;
    }
}

std::string MyRoutingProtocolPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    MyRoutingProtocolPacket *pp = (MyRoutingProtocolPacket *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getMyRoutingProtocolPacketKind());
        case 1: return long2string(pp->getSchedule(i));
        default: return "";
    }
}

bool MyRoutingProtocolPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    MyRoutingProtocolPacket *pp = (MyRoutingProtocolPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setMyRoutingProtocolPacketKind(string2long(value)); return true;
        case 1: pp->setSchedule(i,string2long(value)); return true;
        default: return false;
    }
}

const char *MyRoutingProtocolPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    };
}

void *MyRoutingProtocolPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    MyRoutingProtocolPacket *pp = (MyRoutingProtocolPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


