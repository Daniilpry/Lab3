#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <unistd.h>

using namespace std;

int main() {
    key_t key = ftok("shmfile", 65);

    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    char* data = (char*) shmat(shmid, nullptr, 0);
    if ((intptr_t)data == -1) {
        perror("shmat");
        return 1;
    }

    strcpy(data, "Привіт зі спільної пам’яті!");

    cout << "Дані записано в спільну пам’ять.\n";
    cout << "Зміст: " << data << endl;

    shmdt(data);

    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
