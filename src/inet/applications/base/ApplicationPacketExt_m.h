//
// Generated file, do not edit! Created by nedtool 5.0 from inet/applications/base/ApplicationPacketExt.msg.
//

#ifndef __INET_APPLICATIONPACKETEXT_M_H
#define __INET_APPLICATIONPACKETEXT_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0500
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// cplusplus {{
#include "inet/networklayer/common/L3Address.h"
#include "inet/common/geometry/common/Coord.h"
#include <list>
// }}


namespace inet {

/**
 * Struct generated from inet/applications/base/ApplicationPacketExt.msg:29 by nedtool.
 */
struct nodeinfo
{
    nodeinfo();
    L3Address localAddr;
    Coord pos;
    Coord velocity;
    int nodeDegree;
    double meanSnrNeighbourood;
    double meanPowNeighbourood;
};

// helpers for local use
void __doPacking(omnetpp::cCommBuffer *b, const nodeinfo& a);
void __doUnpacking(omnetpp::cCommBuffer *b, nodeinfo& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const nodeinfo& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, nodeinfo& obj) { __doUnpacking(b, obj); }

/**
 * Class generated from <tt>inet/applications/base/ApplicationPacketExt.msg:39</tt> by nedtool.
 * <pre>
 * packet ApplicationPacketExt
 * {
 *     long sequenceNumber;
 * 
 *     nodeinfo myInfo;
 * 
 *     //int nodeDegree;
 *     //double meanSnrNeighbourood;
 * 	//double meanPowNeighbourood;	
 * 
 *     nodeinfo neighbours[];
 * }
 * </pre>
 */
class ApplicationPacketExt : public ::omnetpp::cPacket
{
  protected:
    long sequenceNumber;
    nodeinfo myInfo;
    nodeinfo *neighbours; // array ptr
    unsigned int neighbours_arraysize;

  private:
    void copy(const ApplicationPacketExt& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const ApplicationPacketExt&);

  public:
    ApplicationPacketExt(const char *name=nullptr, int kind=0);
    ApplicationPacketExt(const ApplicationPacketExt& other);
    virtual ~ApplicationPacketExt();
    ApplicationPacketExt& operator=(const ApplicationPacketExt& other);
    virtual ApplicationPacketExt *dup() const {return new ApplicationPacketExt(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual long getSequenceNumber() const;
    virtual void setSequenceNumber(long sequenceNumber);
    virtual nodeinfo& getMyInfo();
    virtual const nodeinfo& getMyInfo() const {return const_cast<ApplicationPacketExt*>(this)->getMyInfo();}
    virtual void setMyInfo(const nodeinfo& myInfo);
    virtual void setNeighboursArraySize(unsigned int size);
    virtual unsigned int getNeighboursArraySize() const;
    virtual nodeinfo& getNeighbours(unsigned int k);
    virtual const nodeinfo& getNeighbours(unsigned int k) const {return const_cast<ApplicationPacketExt*>(this)->getNeighbours(k);}
    virtual void setNeighbours(unsigned int k, const nodeinfo& neighbours);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const ApplicationPacketExt& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, ApplicationPacketExt& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_APPLICATIONPACKETEXT_M_H

