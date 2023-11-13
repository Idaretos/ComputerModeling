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

/**
 * Packet sink; see NED file for more info.
 */
class FFSink : public cSimpleModule
{
  private:
    cStdDev qstats;
    std::string name;
    // cOutVector qtime;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

Define_Module( FFSink );

void FFSink::initialize()
{
    qstats.setName("queueing time stats");
    name = par("name").stringValue();
    // qtime.setName("queueing time vector");
}

void FFSink::handleMessage(cMessage *msg)
{
    simtime_t d = simTime()-msg->getCreationTime();
    EV << "Sink Received " << msg->getName() << ", queueing time: " << d << "sec" << endl;
    qstats.collect(d);
    // qtime.record(d);
    delete msg;
}

void FFSink::finish()
{
    EV << name << " stats:" << endl;
    EV << "Total jobs processed: " << qstats.getCount() << endl;
    EV << "Avg time in system:   " << qstats.getMean() << endl;
    EV << "Max queueing time:    " << qstats.getMax() << endl;
    EV << "Avg IAT:              " << simTime().dbl() / static_cast<double>(qstats.getCount()) << endl;
    EV << "Standard deviation:   " << qstats.getStddev() << endl;
}

