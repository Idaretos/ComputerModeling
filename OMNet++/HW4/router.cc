#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class SprayRouter : public cSimpleModule
{
private:
    int numPorts;
    double *portProbs;
public:
    ~SprayRouter();
protected:
    virtual void initialize() override;

    virtual void handleMessage(cMessage *msg) override; 
};

Define_Module(SprayRouter);

SprayRouter::~SprayRouter() {
    delete[] portProbs;
}

void SprayRouter::initialize() {
    // Get the number of output ports
    numPorts = gateSize("out");

    // Get the port probabilities
    portProbs = new double[numPorts];

    std::string probs = par("portProbabilities").stringValue();
    std::stringstream ss(probs);
    std::string token;

    ss >> token;
    portProbs[0] = std::stod(token);

    for (int i = 1; i < numPorts; i++) {
        ss >> token;
        portProbs[i] = std::stod(token) + portProbs[i-1];
    }
}

void SprayRouter::handleMessage(cMessage *msg) {
    // Choose an output port based on the probabilities
    int chosenPort = -1;
    double randomValue = uniform(0, 1);
    for (int i = 0; i < numPorts; i++) {
        if (randomValue < portProbs[i]) {
            chosenPort = i;
            break;
        }
    }

    // Send the message to the chosen port
    EV << "SprayRouter Sending message " << msg->getName() << " to port " << chosenPort << "\n";
    send(msg, "out", chosenPort);
}


class GatherRouter : public cSimpleModule {
/*
 Basically same with SprayRouter, but portProbability is not needed.
 Just have to gather messages from muti input ports to a single output port
*/
private:
    int numInPorts;
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(GatherRouter);

void GatherRouter::initialize() {
    numInPorts = gateSize("in");
}

void GatherRouter::handleMessage(cMessage *msg) {
    // Send the message to the output port
    EV << "GatherRouter Sending message " << msg->getName() << " to port " << 0 << "\n";
    send(msg, "out");
}
