simple SprayRouter
{
    parameters:
        string portProbabilities; // Probability associated with each port [probToSink, probToFIFO1, probToFIFO2, ...]
    gates:
        input in;
        output out[];
}

simple GatherRouter
{
    parameters:
    gates:
        input in[];
        output out;
}
