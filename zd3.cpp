#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>

const std::string FILENAME = "dice_rolls.log";
const size_t MAX_FILE_SIZE = 1024; // 1 Кб

int roll_dice() {
    return rand() % 6 + 1; 
}

size_t get_file_size(const std::string& filename) {
    struct stat stat_buf;
    if (stat(filename.c_str(), &stat_buf) == 0) {
        return stat_buf.st_size;
    }
    return 0;
}

int main() {
    std::srand(std::time(nullptr));

    std::ofstream file(FILENAME, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Помилка відкриття файлу для запису." << std::endl;
        return 1;
    }

    while (true) {
        size_t current_size = get_file_size(FILENAME);
        if (current_size >= MAX_FILE_SIZE) {
            std::cout << "Досягнуто ліміту розміру файлу: " << MAX_FILE_SIZE << " байт. Завершення." << std::endl;
            break;
        }

        int result = roll_dice();
        file << result << "\n";
    }

    file.close();
    return 0;
}
