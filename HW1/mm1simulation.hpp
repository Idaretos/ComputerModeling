#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <filesystem>
#include "pcg_random.hpp"

using namespace std;
double DEFAULT_MU = 1.0;
pcg32 rng;

double rand_val(int seed) {
    if (seed != 0) 
        rng.seed(seed);
    // Generate a random double in the range [0, 1)
    return (double(rng()) - rng.min()) / (double(rng.max()) - rng.min());
}

double expon(double lambda) {
    if (lambda == 0.0)
        return (double)INFINITY;
    double z;
    do {
        z = rand_val(0);
    } while (z == 0);

    return - std::log(z) / lambda;
}

class mm1Simulator {
private:
    int Queue = 0;
    int server = 0;
    queue<double> arrival_queue;
    queue<double> departure_queue;
    double lambda;
    double mu;
    int created_jobs = 0;
    double until;
    double prev_now = 0.0;
    double now = 0;
    int count = 0;
    int rec = 0;
    double integral_jobs_in_system = 0.0;
    double integral_queue_length = 0.0;
    double integral_system_time = 0.0;
    double integral_server_time = 0.0;
    
    double L, Lq, W, Wq, Ws;

    void run_simulation() {
        arrival_queue.push(expon(lambda));
        count++;
        while (now < until) {
            if (!arrival_queue.empty() || !departure_queue.empty()) {
                if (departure_queue.empty() || arrival_queue.front() <= departure_queue.front()) {
                    now = arrival_queue.front();
                    arrival_queue.pop();
                    handle_arrival();
                } else {
                    now = departure_queue.front();
                    departure_queue.pop();
                    handle_service_finish();
                }
            }
        }
        cal_metrics();
    }

    void cal_metrics() {
        L = integral_jobs_in_system / prev_now;
        Lq = integral_queue_length / prev_now;
        W = L / lambda;
        Ws = 1 / mu;
        Wq = W - Ws;
    }

    void handle_arrival() {
        // Server is idle and no jobs in queue
        if (server == 0 && Queue == 0) {
            record(0.0);
            server = 1;
            departure_queue.push(now+expon(mu));
        }
        // Server is busy or job waiting in queue
        else {
            record();
            Queue++;
        }
        count++;
        arrival_queue.push(now+expon(lambda));
    }

    void handle_service_finish() {
        record();
        server = 0;
        if (Queue > 0) {
            server = 1;
            Queue--;
            departure_queue.push(now+expon(mu));
        }
    }

    void record(double queueing_delay = -1.0) {
        double interval = now - prev_now;
        integral_jobs_in_system += (server+Queue) * interval;
        integral_queue_length += Queue * interval;
        integral_system_time += double(server + Queue)*interval;
        integral_server_time += double(server)*interval;
        prev_now = now;
    }

    void print_results() {
        if (lambda != 0.0)
            cout << "mean jobs in system:\t" << L << endl
                 << "mean queue length:\t"   << Lq << endl
                 << "mean queueing delay:\t" << Wq << endl;
        else
            cout << "mean jobs in system:\t" << 0.0 << endl
                 << "mean queue length:\t"   << 0.0 << endl
                 << "mean queueing delay:\t" << 0.0 << endl;
    }

public:
    mm1Simulator(const int& until, const double& lambda, const double& mu, int seed=1120)
                : until(until), lambda(lambda), mu(mu) {rand_val(seed);}
    mm1Simulator(const int& until, const double& traffic_load, int seed = 1120)
                : until(until), lambda(DEFAULT_MU*traffic_load), mu(DEFAULT_MU) {rand_val(seed);}

    void simulate() {
        run_simulation();
        print_results();
    }

    string get_results(string delimiter= ",") {
        double traffic_load = lambda / mu;
        if (lambda != 0.0)
            return to_string(traffic_load) + delimiter
                 + to_string(integral_jobs_in_system / prev_now) + delimiter
                 + to_string(integral_queue_length / prev_now) + delimiter
                 + to_string(integral_queue_length / (double)count);
        else
            return to_string(traffic_load) + delimiter
                 + to_string(0.0) + delimiter
                 + to_string(0.0) + delimiter
                 + to_string(0.0);
    }
};
