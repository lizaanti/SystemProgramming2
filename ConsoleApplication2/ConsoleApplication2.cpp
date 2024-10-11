#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

void SetConsoleTextColor(HANDLE hConsole, WORD color) {
    SetConsoleTextAttribute(hConsole, color);
}

void WriteToConsole(const std::string& text, HANDLE hConsole) {
    DWORD written;
    WriteConsole(hConsole, text.c_str(), text.size(), &written, nullptr);
}

void CopyFilesToConsole(const std::vector<std::string>& files, bool suppressErrors) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    // Получаем оригинальные атрибуты консоли
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalAttributes = consoleInfo.wAttributes;

    for (const auto& file : files) {
        std::ifstream inFile(file);
        if (!inFile) {
            if (!suppressErrors) {
                std::cerr << "Error: Could not open file " << file << std::endl;
            }
            continue;
        }

        // Читаем содержимое файла и выводим в консоль
        std::string line;
        while (std::getline(inFile, line)) {
            WriteToConsole(line + "\n", hConsole);
        }
        inFile.close();
    }

    // Восстанавливаем оригинальные атрибуты консоли
    SetConsoleTextAttribute(hConsole, originalAttributes);
}

int main(int argc, char* argv[]) {
    bool suppressErrors = false;
    std::vector<std::string> files;

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-s") {
            suppressErrors = true;
        }
        else {
            files.push_back(arg);
        }
    }

    // Копируем файлы на стандартное устройство вывода
    CopyFilesToConsole(files, suppressErrors);

    return 0;
}