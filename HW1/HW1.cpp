#include <iostream>
#include <string>
#include <stdlib.h>
#include "mm1simulation.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    const int SEED = 1120;
    
    DEFAULT_MU = 1.0;
    double until = 1.0e7;

    filesystem::path curr_path = filesystem::current_path();
    filesystem::path fname("output/data.txt");
    
    double traffic_load = 1.5;
    if (argc > 1)
        traffic_load = atof(argv[1]);

    ofstream file(curr_path / fname, ios::app);
    mm1Simulator mm1Queue(until, traffic_load, SEED);
    
    cout << "sim_time:\t" + to_string(until) << endl 
         << "traffic_load:\t" + to_string(traffic_load) << endl;
    mm1Queue.simulate();
    
    if (argc > 1)
        file << to_string(traffic_load) + " " + mm1Queue.get_results(" ") << endl;

    file.close();
}
