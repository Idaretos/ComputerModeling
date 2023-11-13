//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
using namespace omnetpp;

class Tic7 : public cSimpleModule {
  private:
    simtime_t timeout;
    cMessage *timeoutEvent; // variable to remember the message until we send it back

  public:
    Tic7();
    virtual ~Tic7();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Tic7);

Tic7::Tic7() {
    // Set the pointer to NULL, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.
    timeoutEvent = NULL;
}

Tic7::~Tic7() {
    // Dispose of dynamically allocated the objects
    cancelAndDelete(timeoutEvent);
}

void Tic7::initialize() {
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    EV << "Sending initial message\n";
    cMessage *msg = new cMessage("tictocMsg");
    send(msg, "out");
    scheduleAt(simTime()+timeout, timeoutEvent); // schedule the timeout event
}

void Tic7::handleMessage(cMessage *msg) {
    // There are several ways of distinguishing messages, for example by message
    // kind (an int attribute of cMessage) or by class using dynamic_cast
    // (provided you subclass from cMessage). In this code we just check if we
    // recognize the pointer, which (if feasible) is the easiest and fastest
    // method.
    if (msg==timeoutEvent) {
        EV << "Timeout expired, resending message and restarting timer\n";
        cMessage *msg = new cMessage("tictocMsg"); send(msg, "out"); scheduleAt(simTime()+timeout, timeoutEvent);
    } else {// ACK message arrived
        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);
        delete msg;
        cMessage *msg = new cMessage("tictocMsg"); send(msg,"out"); scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

class Toc7 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc7);

void Toc7::handleMessage(cMessage *msg)
{
    if (uniform(0, 1) < 0.1) {
        EV << "\"Losing\" message.\n";
        bubble("message lost");  // making animation more informative...
        delete msg; // error-free delivery: delete message
    }
    else {
        EV << "Sending back same message as acknowledgement.\n";
        send(msg, "out"); // send out the message
    }
}