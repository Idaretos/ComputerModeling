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

class Txc6 : public cSimpleModule {
  private:
    cMessage *event; // pointer to the event object which we'll use for timing
    cMessage *tictocMsg; // variable to remember the message until we send it back

  public:
    Txc6();
    virtual ~Txc6();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Txc6);

Txc6::Txc6() {
    // Set the pointer to NULL, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.
    event = tictocMsg = NULL;
}

Txc6::~Txc6() {
    // Dispose of dynamically allocated the objects
    cancelAndDelete(event);
    delete tictocMsg;
}

void Txc6::initialize() {
    // Create the event object we'll use for timing -- just any ordinary message.
    event = new cMessage("event");

    // No tictoc message yet.
    tictocMsg = NULL;

    if (getName() == "tic") {
        // We don't start right away, but instead send an message to ourselves
        // (a "self-message") -- we'll do the first sending when it arrives
        // back to us, at t=5.0s simulated time.
        EV << "Scheduling first send to t=5.0s\n";
        tictocMsg = new cMessage("tictocMsg");
        scheduleAt(5.0, event);
    }
}

void Txc6::handleMessage(cMessage *msg) {
    // There are several ways of distinguishing messages, for example by message
    // kind (an int attribute of cMessage) or by class using dynamic_cast
    // (provided you subclass from cMessage). In this code we just check if we
    // recognize the pointer, which (if feasible) is the easiest and fastest
    // method.
    if (msg==event) {
        // The self-message arrived, so we can send out tictocMsg and NULL out
        // its pointer so that it doesn't confuse us later.
        EV << "Wait period is over, sending back message\n";
        send(tictocMsg, "out");
        tictocMsg = NULL;
    } else {
        // If the message we received is not our self-message, then it must
        // be the tic-toc message arriving from our partner. We remember its
        // pointer in the tictocMsg variable, then schedule our self-message
        // to come back to us in 1s simulated time.
        if (uniform(0,1) < 0.1) {
            EV << "\"Losing\" message\n";
            delete msg;
        } else {
            simtime_t delay = par("delayTime");
            EV << "Message arrived, starting to wait " << delay << "secs...\n";
            tictocMsg = msg;
            scheduleAt(simTime()+delay, event);
        }
    }
}
