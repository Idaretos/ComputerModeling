#include <iostream>
#include <cmath>
#include <ctime>

constexpr double SIM_TIME = 1.0e7;
constexpr double SERV_TIME = 1.00;

double rand_val(int seed) {
    const long a = 16807;
    const long m = 2147483647;
    const long q = 127773;
    const long r = 2836;
    static long x;

    if (seed != 0) x = seed;

    long x_div_q = x / q;
    long x_mod_q = x % q;
    long x_new = (a * x_mod_q) - (r * x_div_q);
    if (x_new > 0)
        x = x_new;
    else
        x = x_new + m;

    return static_cast<double>(x) / m;
}

double exponential(double x) {
    double z;
    do {
        z = rand_val(0);
    } while ((z == 0) || (z == 1));

    return -x * std::log(z);
}

int main() {
    double end_time = SIM_TIME;
    double Ts = SERV_TIME;
    double traffic_load = 0.01;
    double Ta = SERV_TIME / traffic_load;
    double time = 0.0;
    double t1 = 0.0;
    double t2 = SIM_TIME;
    unsigned int n = 0;
    unsigned int c = 0;
    double b = 0.0;
    double s = 0.0;
    double tn = time;
    double tb, x, u, l, w;

    rand_val(1120);

    while (time < end_time) {
        if (t1 < t2) {
            time = t1;
            s = s + n * (time - tn);
            n++;
            tn = time;
            t1 = time + exponential(Ta);
            if (n == 1) {
                tb = time;
                t2 = time + exponential(Ts);
            }
        } else {
            time = t2;
            s = s + n * (time - tn);
            n--;
            tn = time;
            c++;
            if (n > 0)
                t2 = time + exponential(Ts);
            else {
                t2 = SIM_TIME;
                b = b + time - tb;
            }
        }
    }

    b = b + time - tb;
    x = c / time;
    u = b / time;
    l = s / time;
    w = l / x;

    std::cout << "=============================================================== \n";
    std::cout << "=            *** Results from M/M/1 simulation ***            = \n";
    std::cout << "=============================================================== \n";
    std::cout << "=  Total simulated time         = " << time << " sec     \n";
    std::cout << "=============================================================== \n";
    std::cout << "=  INPUTS:                                    \n";
    std::cout << "=    Mean time between arrivals = " << Ta << " sec      \n";
    std::cout << "=    Mean service time          = " << Ts << " sec      \n";
    std::cout << "=============================================================== \n";
    std::cout << "=  OUTPUTS:                                   \n";
    std::cout << "=    Number of completions      = " << c << " cust     \n";
    std::cout << "=    Throughput rate            = " << x << " cust/sec \n";
    std::cout << "=    Server utilization         = " << 100.0 * u << " %       \n";
    std::cout << "=    Mean number in system      = " << l << " cust     \n";
    std::cout << "=    Mean queueing delay        = " << w-Ts << "sec    \n"; 
    std::cout << "=    Mean residence time        = " << w << " sec      \n";
    std::cout << "=============================================================== \n";

    return 0;
}
