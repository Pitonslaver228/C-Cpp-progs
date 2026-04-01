#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <algorithm>

class CStringEditor {
private:
    std::string text;
    std::string fileExtension = ".txt";

    // проверка расширения
    bool hasValidExtension(const std::string& filename) {
        size_t position = filename.rfind('.');
        if (position == std::string::npos) return false;
        return filename.substr(position) == fileExtension;
    }

public:
    // д октрытия файла
    bool openFile(const std::string& filename) {
        if (!hasValidExtension(filename)) {
            std::cerr << "Ошибка: Неверное расширение файла. Ожидается " << fileExtension << "\n";
            return false;
        }

        std::ifstream infile(filename);
        if (!infile) {
            std::cerr << "Ошибка: Не удалось открыть файл.\n";
            return false;
        }

        text.clear();
        std::string line; 
        while (std::getline(infile, line)) {
            text += line + '\n';
        }
        infile.close();
        std::cout << "Файл успешно открыт.\n";
        return true;
    }

    // д сохранения файла
    bool saveFile(const std::string& filename) {
        std::ofstream outfile(filename);
        if (!outfile) {
            std::cerr << "Ошибка: Не удалось сохранить файл.\n";
            return false;
        }
        outfile << text;
        outfile.close();
        std::cout << "Файл успешно сохранён.\n";
        return true;
    }

    // нахожддение символа
    void findCharacter(char ch) {
        size_t count = std::count(text.begin(), text.end(), ch);
        std::cout << "Символ '" << ch << "' найден " << count << " раз(а).\n";
    }

    // переводим в верхний регистр
    void toUpperCase() {
        for (char& ch : text) {
            if (ch >= 'а' && ch <= 'я') ch -= 32;
            else if (ch >= 'a' && ch <= 'z') ch -= 32;
        }
        std::cout << "Все буквы преобразованы в верхний регистр.\n";
    }

    // удаление с конца символа(ов0)
    void removeFromEnd(size_t n) {
        if (n > text.length()) n = text.length();
        text.erase(text.length() - n, n);
        std::cout << "Удалено " << n << " символов с конца строки.\n";
    }

    // текст файла
    void printText() const {
        std::cout << "\n--- ТЕКУЩИЙ ТЕКСТ ---\n" << text << "\n----------------------\n";
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    CStringEditor editor;
    std::string filename;

    std::cout << "Введите имя файла для открытия: ";
    std::cin >> filename;

    // удаляем кавычки
    if (!filename.empty() && filename.front() == '"') filename.erase(0, 1);
    if (!filename.empty() && filename.back() == '"') filename.pop_back();

    if (!editor.openFile(filename)) return 1;

    editor.printText();

    // Поиск символа
    char symbol;
    std::cout << "Введите символ для поиска: ";
    std::cin >> symbol;
    editor.findCharacter(symbol);

    // Замена на верхний регистр
    editor.toUpperCase();

    // Удаление символов с конца — пользователь вводит количество
    size_t countToRemove;
    std::cout << "Введите количество символов для удаления с конца текста: ";
    std::cin >> countToRemove;
    editor.removeFromEnd(countToRemove);

    editor.printText();

    std::cout << "Введите имя файла для сохранения: ";
    std::cin >> filename;

    if (!filename.empty() && filename.front() == '"') filename.erase(0, 1);
    if (!filename.empty() && filename.back() == '"') filename.pop_back();

    editor.saveFile(filename);

    return 0;
}
