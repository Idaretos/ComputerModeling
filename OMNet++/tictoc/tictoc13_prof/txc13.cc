//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "tictoc13_m.h"

using namespace omnetpp;

class Txc13 : public cSimpleModule
{
  private:
    long numSent;
    long numReceived;
    cHistogram hopCountStats;
    cOutVector hopCountVector;
    int rt[6];

  protected:
    virtual TicTocMsg13 *generateMessage();
    virtual void forwardMessage(TicTocMsg13 *msg);
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    // The finish() function is called by OMNeT++ at the end of the simulation:
    virtual void finish();
};

Define_Module(Txc13);

void Txc13::initialize()
{
    // Initialize variables
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);

    // Initialize the routing table
    rt[0] = par("route0");
    rt[1] = par("route1");
    rt[2] = par("route2");
    rt[3] = par("route3");
    rt[4] = par("route4");
    rt[5] = par("route5");

    hopCountStats.setName("hopCountStats");
    hopCountVector.setName("hopCountVector");

    // Module 0 sends the first message
    if (getIndex() == 0)
    {
        // Boot the process scheduling the initial message as a self-message.
        TicTocMsg13 *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Txc13::handleMessage(cMessage *msg)
{
    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);

    if (ttmsg->getDestination() == getIndex())
    {
        // Message arrived
        int hopcount = ttmsg->getHopCount();
        EV << "Message " << ttmsg << " arrived after " << hopcount << " hops.\n";
        bubble("ARRIVED, starting new one!");

        // update statistics.
        numReceived++;
        hopCountVector.record(hopcount);
        hopCountStats.collect(hopcount);

        delete ttmsg;

        // Generate another one.
        EV << "Generating another message: ";
        TicTocMsg13 *newmsg = generateMessage();
        EV << newmsg << endl;
        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        // We need to forward the message.
        forwardMessage(ttmsg);
    }
}

TicTocMsg13 *Txc13::generateMessage()
{
    // Produce source and destination addresses.
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src) dest++;

    char msgname[20];
    snprintf(msgname, 20, "tic-%d-to-%d", src, dest);

    // Create message object and set source and destination field.
    TicTocMsg13 *msg = new TicTocMsg13(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc13::forwardMessage(TicTocMsg13 *msg)
{
    // Increment hop count.
    msg->setHopCount(msg->getHopCount()+1);

    // Gate selection from routing table.
    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);
    int k = rt[ttmsg->getDestination()];

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "out", k);
}

void Txc13::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:     " << numSent << endl;
    EV << "Received: " << numReceived << endl;
    EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
    EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
    EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
    EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;

    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);

    hopCountStats.recordAs("hop count");
}
