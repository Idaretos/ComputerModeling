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

class Tic8 : public cSimpleModule {
private:
    simtime_t timeout;
    cMessage *timeoutEvent; // variable to remember the message until we send it back
    int seq;
    cMessage *message; // original copy of msg
public:
    Tic8();
    virtual ~Tic8();

protected:
    virtual cMessage* generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Tic8);

Tic8::Tic8() {
    // Set the pointer to NULL, so that the destructor won't crash
    // even if initialize() doesn't get called because of a runtime
    // error or user cancellation during the startup process.
    timeoutEvent = NULL;
}

Tic8::~Tic8() {
    // Dispose of dynamically allocated the objects
    cancelAndDelete(timeoutEvent);
}

cMessage* Tic8::generateNewMessage() {
    char msgname[20];
    snprintf(msgname, 20, "tic-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}

void Tic8::sendCopyOf(cMessage *msg) {
    // Duplicate message and send the copy
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, "out");
}

void Tic8::initialize() {
    seq = 0;
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    EV << "Sending initial message\n";
    message = generateNewMessage();
    sendCopyOf(message);
    scheduleAt(simTime()+timeout, timeoutEvent); // schedule the timeout event
}

void Tic8::handleMessage(cMessage *msg) {
    // There are several ways of distinguishing messages, for example by message
    // kind (an int attribute of cMessage) or by class using dynamic_cast
    // (provided you subclass from cMessage). In this code we just check if we
    // recognize the pointer, which (if feasible) is the easiest and fastest
    // method.
    if (msg==timeoutEvent) {
        EV << "Timeout expired, resending message and restarting timer\n";
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);
    } else {// ACK message arrived
        EV << "Received: " << msg->getName() << "\n";
        delete msg; // delete ACK

        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);
        delete message; // delete prev message

        message = generateNewMessage();
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

class Toc8 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc8);

void Toc8::handleMessage(cMessage *msg)
{
    if (uniform(0, 1) < 0.1) {
        EV << "\"Losing\" message " << msg << endl;
        bubble("message lost");  // making animation more informative...
        delete msg; // error-free delivery: delete message
    }
    else {
        EV << "Sending back same message as acknowledgement.\n";
        send(msg, "out"); // send out the message
    }
}