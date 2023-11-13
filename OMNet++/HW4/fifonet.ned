import gen;
import fifo;
import sink;
import router;


//
// Simple queueing network: generator + FIFO + sink.
//
module FifoNet
{
    submodules:
        gen: FFGenerator {
            parameters:
                @display("p=89,100;i=block/source");
        }

        fifo0: FFJobFifo {
            parameters:
                @display("p=309,100;i=block/server;q=queue");
                name = "fifo0";
                serviceTime = default(exponential(0.75s));
        }
        fifo1: FFJobFifo {
            parameters:
                @display("p=329,170;i=block/server;q=queue");
                name = "fifo1";
                serviceTime = default(exponential(1s));
        }
        fifo2: FFJobFifo {
            parameters:
                @display("p=409,170;i=block/server;q=queue");
                name = "fifo2";
                serviceTime = default(exponential(2s));
        }
        fifo3: FFJobFifo {
            parameters:
                @display("p=489,170;i=block/server;q=queue");
                name = "fifo3";
                serviceTime = default(exponential(4s));
        }

        sink: FFSink {
            parameters:
                @display("p=409,40;i=block/sink");
                name = "sink";
        }
        
        router0: GatherRouter {
            parameters:
                @display("p=209,100;i=block/queue");
        }
        router1: SprayRouter {
            parameters:
                @display("p=409,100;i=block/queue");
                portProbabilities = default("0.25, 0.25, 0.25, 0.25");
        }
    connections:
        gen.out --> router0.in++;
        fifo0.out --> router1.in;

        router1.out++ --> sink.in;
        router1.out++ --> fifo1.in;
        router1.out++ --> fifo2.in;
        router1.out++ --> fifo3.in;

        fifo1.out --> router0.in++;
        fifo2.out --> router0.in++;
        fifo3.out --> router0.in++;

        router0.out --> fifo0.in;
}

network fifonet
{
    submodules:
        FIFONet: FifoNet;
}
