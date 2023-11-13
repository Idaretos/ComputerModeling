#include <iostream>
#include <string>
#include <stdlib.h>
#include "mm1simulation.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    const int SEED = 1120;
    
    DEFAULT_MU = 1.0;
    double until = 10.0;
    double traffic_load = 0.99;
    int power = 7;

    filesystem::path curr_path = filesystem::current_path();
    filesystem::path fname("output/time.txt");
    
    if (argc > 1)
        power = atof(argv[1]);
    
    until = pow(10.0, power);

    ofstream file(curr_path / fname, ios::app);
    mm1Simulator mm1Queue(until, traffic_load, SEED);
    
    cout << "sim_time:\t10^" + to_string(power) << endl 
         << "traffic_load:\t" + to_string(traffic_load) << endl;
    mm1Queue.simulate();
    
    if (argc > 1)
        file << to_string(until) + " " + mm1Queue.get_results(" ") << endl;

    file.close();
}
