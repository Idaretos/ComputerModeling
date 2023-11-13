//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2005 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#include <omnetpp.h>

using namespace omnetpp;

class FFJobFifo : public cSimpleModule {
  protected:
    cMessage *msgServiced;
    cMessage *endServiceMsg;
    cQueue queue;
    cOutVector qlenVector;

  public:
    FFJobFifo();
    virtual ~FFJobFifo();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    // hook functions to (re)define behaviour
    virtual void arrival(cMessage *msg);
    virtual simtime_t startService(cMessage *msg);
    virtual void endService(cMessage *msg);
    virtual void updateQueueLength();
};


Define_Module(FFJobFifo);

FFJobFifo::FFJobFifo() {
    msgServiced = endServiceMsg = NULL;
}

FFJobFifo::~FFJobFifo() {
    delete msgServiced;
    cancelAndDelete(endServiceMsg);
}

void FFJobFifo::initialize() {
    endServiceMsg = new cMessage("end-service");
    queue.setName("queue");
}

void FFJobFifo::handleMessage(cMessage *msg) {
    if (msg==endServiceMsg) { // service completion event
        endService( msgServiced );
        if (queue.isEmpty()) {
            msgServiced = NULL;
        } else {
            msgServiced = (cMessage *) queue.pop();
            simtime_t serviceTime = startService( msgServiced );
            scheduleAt( simTime()+serviceTime, endServiceMsg );
        }
        updateQueueLength();
    } else if (!msgServiced) { // job arrival when server is idle
        arrival( msg );
        msgServiced = msg;
        simtime_t serviceTime = startService( msgServiced );
        scheduleAt( simTime()+serviceTime, endServiceMsg );
    } else { // job arrival when server is busy
        arrival( msg );
        queue.insert( msg );
        updateQueueLength();
    }
}

simtime_t FFJobFifo::startService(cMessage *msg) {
    EV << "Starting service of " << msg->getName() << endl;
    return msg->getTimestamp();
}

void FFJobFifo::endService(cMessage *msg) {
    EV << "Completed service of " << msg->getName() << endl;
    send( msg, "out" );
}

void FFJobFifo::arrival(cMessage *msg) {
    EV << "Arrival of " << msg->getName() << endl;
    msg->setTimestamp( simTime() );
    updateQueueLength();
}

void FFJobFifo::updateQueueLength() {
    qlenVector.record( queue.getLength() );
}

