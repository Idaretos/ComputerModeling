//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2005 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


#include <omnetpp.h>
#include <string>

using namespace omnetpp;

class FFJobFifo : public cSimpleModule {
  protected:
    cMessage *msgServiced;
    cMessage *endServiceMsg;
    cQueue queue;
    std::string name;

    cStdDev qstats;
    cOutVector qlenVector;
    // cOutVector qtime;

  public:
    FFJobFifo();
    virtual ~FFJobFifo();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    // hook functions to (re)define behaviour
    virtual void arrival(cMessage *msg);
    virtual simtime_t startService(cMessage *msg);
    virtual void endService(cMessage *msg);
    virtual void updateLength();
};


Define_Module( FFJobFifo );

FFJobFifo::FFJobFifo() {
    msgServiced = endServiceMsg = NULL;
}

FFJobFifo::~FFJobFifo() {
    delete msgServiced;
    cancelAndDelete(endServiceMsg);
}

void FFJobFifo::initialize() {
    qstats.setName("queueing time stats");
    qlenVector.setName("queue length");
    name = par("name").stringValue();
    // qtime.setName("queueing time vector");

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
            scheduleAt( simTime() + serviceTime, endServiceMsg );
        }
        updateLength();
    } else if (!msgServiced) { // job arrival when server is idle
        arrival( msg );
        msgServiced = msg;
        simtime_t serviceTime = startService( msgServiced );
        scheduleAt( simTime() + serviceTime, endServiceMsg );
        updateLength();
    } else { // job arrival when server is busy
        arrival( msg );
        queue.insert( msg );
        updateLength();
    }
}

simtime_t FFJobFifo::startService(cMessage *msg) {
    EV << "FIFO Starting service of " << msg->getName() << endl;
    return par("serviceTime");
}

void FFJobFifo::endService(cMessage *msg) {
    simtime_t d = simTime()-msg->getTimestamp();
    qstats.collect(d);
    // qtime.record(d);
    EV << "FIFO Completed service of " << msg->getName() << endl;
    send( msg, "out" );
}

void FFJobFifo::finish() {
    EV << name << " stats:" << endl;
    EV << "Total jobs arrived:   " << qstats.getCount() + queue.getLength() << endl;
    EV << "Total jobs processed: " << qstats.getCount() << endl;
    EV << "Avg time in system:   " << qstats.getMean() << endl;
    EV << "Avg IAT:              " << simTime().dbl()/ static_cast<double>(qstats.getCount() + queue.getLength()) << endl;
    EV << "Standard deviation:   " << qstats.getStddev() << endl;
}

void FFJobFifo::arrival(cMessage *msg) {
    EV << "FIFO Arrival of " << msg->getName() << endl;
    msg->setTimestamp( simTime() );
}

void FFJobFifo::updateLength() {
    int slen = msgServiced ? 1 : 0;
    qlenVector.record(queue.getLength()+slen); // Record current queue length
}
