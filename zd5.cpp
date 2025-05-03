#include <iostream>
#include <fstream>
#include <sys/resource.h>
#include <csignal>
#include <unistd.h>

using namespace std;

void handle_file_size_limit(int) {
    cerr << "File size limit exceeded!" << endl;
    exit(1);
}

void set_file_size_limit(size_t max_bytes) {
    struct rlimit rl;
    rl.rlim_cur = max_bytes;
    rl.rlim_max = max_bytes;
    if (setrlimit(RLIMIT_FSIZE, &rl) != 0) {
        perror("setrlimit");
        exit(1);
    }
    signal(SIGXFSZ, handle_file_size_limit);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Program need two arguments" << endl;
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];

    ifstream in(input_file, ios::binary);
    if (!in.is_open()) {
        cerr << "Cannot open file " << input_file << " for reading" << endl;
        return 1;
    }

    ofstream out(output_file, ios::binary);
    if (!out.is_open()) {
        cerr << "Cannot open file " << output_file << " for writing" << endl;
        return 1;
    }

    set_file_size_limit(44);

    char buffer[4096];
    while (in.read(buffer, sizeof(buffer))) {
        out.write(buffer, in.gcount());
    }

    if (in.gcount() > 0) {
        out.write(buffer, in.gcount());
    }

    in.close();
    out.close();

    return 0;
}
