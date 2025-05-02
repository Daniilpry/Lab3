#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <csignal>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

using namespace std;

void handle_timeout(int) {
    cerr << "⚠️ CPU time limit exceeded!" << endl;
    exit(1);
}

void set_cpu_time_limit(int seconds) {
    struct rlimit rl;
    rl.rlim_cur = seconds;
    rl.rlim_max = seconds;
    if (setrlimit(RLIMIT_CPU, &rl) != 0) {
        perror("setrlimit");
        exit(1);
    }
    signal(SIGXCPU, handle_timeout);
}

vector<int> draw_numbers(int count, int max_value) {
    vector<int> pool(max_value);
    iota(pool.begin(), pool.end(), 1);
    random_device rd;
    mt19937 gen(rd());
    shuffle(pool.begin(), pool.end(), gen);
    return vector<int>(pool.begin(), pool.begin() + count);
}

int main() {
    set_cpu_time_limit(2);
    auto main_numbers = draw_numbers(7, 49);
    auto bonus_numbers = draw_numbers(6, 36);
    cout << "🎰 Лотерея (7 з 49): ";
    for (int n : main_numbers) cout << n << " ";
    cout << "\n🎁 Додаткові (6 з 36): ";
    for (int n : bonus_numbers) cout << n << " ";
    cout << endl;
    return 0;
}
