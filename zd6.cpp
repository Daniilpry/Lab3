#include <iostream>
#include <sys/resource.h>
#include <csignal>
#include <unistd.h>

using namespace std;

void handle_stack_overflow(int) {
    cerr << "Stack size limit exceeded!" << endl;
    exit(1);
}

void set_stack_limit(size_t bytes) {
    struct rlimit rl;
    rl.rlim_cur = bytes;
    rl.rlim_max = bytes;
    if (setrlimit(RLIMIT_STACK, &rl) != 0) {
        perror("setrlimit");
        exit(1);
    }
}

void recurse(int depth) {
    char buffer[1024]; // забирає 1 КБ
    for (int i = 0; i < 1024; i++) buffer[i] = i;
    cout << "Depth: " << depth << endl;
    recurse(depth + 1);
}

int main() {
    signal(SIGSEGV, handle_stack_overflow);
    set_stack_limit(16 * 1024); // 16 КБ

    recurse(1);
    return 0;
}
