#include <string>
#include <omnetpp.h>

class Txc1: public cSimpleModule {
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *mmsg);
}

Define_Module(Txc1);

void Txc1::initialize() {

}