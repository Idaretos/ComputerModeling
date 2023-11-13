//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2005 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// Single-server queue
//
simple FFJobFifo
{
    parameters:
        volatile double serviceTime @unit(s);
        string name;
    gates:
        input in;
        output out;
}
