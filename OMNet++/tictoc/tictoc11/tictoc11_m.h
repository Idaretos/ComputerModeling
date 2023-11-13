//
// Generated file, do not edit! Created by opp_msgtool 6.0 from tictoc11.msg.
//

#ifndef __TICTOC11_M_H
#define __TICTOC11_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class TicTocMsg11;
/**
 * Class generated from <tt>tictoc11.msg:12</tt> by opp_msgtool.
 * <pre>
 * message TicTocMsg11
 * {
 *     int source;
 *     int destination;
 *     int hopCount = 0;
 * }
 * </pre>
 */
class TicTocMsg11 : public ::omnetpp::cMessage
{
  protected:
    int source = 0;
    int destination = 0;
    int hopCount = 0;

  private:
    void copy(const TicTocMsg11& other);

  protected:
    bool operator==(const TicTocMsg11&) = delete;

  public:
    TicTocMsg11(const char *name=nullptr, short kind=0);
    TicTocMsg11(const TicTocMsg11& other);
    virtual ~TicTocMsg11();
    TicTocMsg11& operator=(const TicTocMsg11& other);
    virtual TicTocMsg11 *dup() const override {return new TicTocMsg11(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual int getSource() const;
    virtual void setSource(int source);

    virtual int getDestination() const;
    virtual void setDestination(int destination);

    virtual int getHopCount() const;
    virtual void setHopCount(int hopCount);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const TicTocMsg11& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, TicTocMsg11& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline TicTocMsg11 *fromAnyPtr(any_ptr ptr) { return check_and_cast<TicTocMsg11*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __TICTOC11_M_H
