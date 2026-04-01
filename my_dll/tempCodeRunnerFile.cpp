#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime> // Для rand()

// СТРУКТУРЫ

typedef struct {
    double x;
    double valueExact;
    double valueSeries;
    double delta;
    bool valid;
} Result;

typedef struct {
    const char* textColor;
    const char* bgColor;
} Color;

typedef Color (*ColorCallback)(int);

typedef const char* (*GetStringFunc)();
typedef Color (*GetColorFunc)();
typedef int (*ComputeValuesFunc)(double, double, double, double, ColorCallback, Result*, int);

// === ФУНКЦИИ ===

int callbackCounter = 0;

// Преобразование из текста цвета в код консоли
int color_name_to_code(const std::string& name, bool isBg = false) {
    if (name == "black") return 0;
    if (name == "blue") return isBg ? BACKGROUND_BLUE : FOREGROUND_BLUE;
    if (name == "green") return isBg ? BACKGROUND_GREEN : FOREGROUND_GREEN;
    if (name == "red") return isBg ? BACKGROUND_RED : FOREGROUND_RED;
    if (name == "white") return isBg ? (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
                                     : (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return 0;
}

// Установка цвета текста и фона консоли
void SetConsoleTextColor(const std::string& text, const std::string& bg) {
    int tColor = color_name_to_code(text, false);
    int bColor = color_name_to_code(bg, true);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tColor | bColor);
}

// Callback возвращает цвет шапки
Color color_callback(int callCount) {
    callbackCounter = callCount;
    return { "red", "blue" }; // Имя: Ренат, Отчество: Рашидович
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(nullptr)); // Инициализация генерации случайных чисел

    HMODULE hLib = LoadLibraryA("calc_lib.dll");
    if (!hLib) {
        std::cerr << "Ошибка загрузки библиотеки." << std::endl;
        return 1;
    }

    auto get_function_name = (GetStringFunc)GetProcAddress(hLib, "get_function_name");
    auto get_author_surname = (GetStringFunc)GetProcAddress(hLib, "get_author_surname");
    auto get_table_header_color = (GetColorFunc)GetProcAddress(hLib, "get_table_header_color");
    auto get_table_footer_color = (GetColorFunc)GetProcAddress(hLib, "get_table_footer_color");
    auto compute_values = (ComputeValuesFunc)GetProcAddress(hLib, "compute_values");

    if (!get_function_name || !get_author_surname || !get_table_header_color ||
        !get_table_footer_color || !compute_values) {
        std::cerr << "Ошибка: не найдены все необходимые функции в DLL." << std::endl;
        FreeLibrary(hLib);
        return 1;
    }

    double x_start, x_end, step, epsilon;
    std::cout << "Введите начальное значение x: "; std::cin >> x_start;
    std::cout << "Введите конечное значение x: "; std::cin >> x_end;
    std::cout << "Введите шаг: "; std::cin >> step;
    std::cout << "Введите точность (epsilon): "; std::cin >> epsilon;

    const int maxCount = 1000;
    std::vector<Result> results(maxCount);

    // === ОТРИСОВКА ШАПКИ ===
    Color headerColor = color_callback(0);
    SetConsoleTextColor(headerColor.textColor, headerColor.bgColor);

    std::cout << "\nТаблица значений функции: " << get_function_name() << "\n";
    std::cout << "Автор библиотеки: " << get_author_surname() << "\n";
    std::cout << "Цвет шапки: Текст = " << headerColor.textColor
              << ", Фон = " << headerColor.bgColor << "\n";

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "--------------------------------------------------------------\n";
    std::cout << "|    X    | Точное значение | Рядовое значение |   Δ (delta)  |\n";
    std::cout << "--------------------------------------------------------------\n";

    // === РАСЧЕТ ДАННЫХ ===
    int computed = compute_values(x_start, x_end, step, epsilon, color_callback, results.data(), maxCount);

    // === СЛУЧАЙНЫЙ ЦВЕТ ДЛЯ ТЕЛА ТАБЛИЦЫ ===
    std::vector<std::string> textColors = {"white", "green", "blue", "red"};
    std::vector<std::string> bgColors = {"black", "blue", "green"};

    std::string rowTextColor = textColors[rand() % textColors.size()];
    std::string rowBgColor = bgColors[rand() % bgColors.size()];
    SetConsoleTextColor(rowTextColor, rowBgColor);

    // === ТЕЛО ТАБЛИЦЫ ===
    for (int i = 0; i < computed; ++i) {
        const Result& r = results[i];
        std::cout << "| " << std::setw(7) << r.x
                  << " | " << std::setw(16) << r.valueExact
                  << " | " << std::setw(16) << r.valueSeries
                  << " | " << std::setw(11) << r.delta << " |\n";
    }

    // === ПОДВАЛ ===
    Color footerColor = get_table_footer_color();
    SetConsoleTextColor(footerColor.textColor, footerColor.bgColor);
    std::cout << "--------------------------------------------------------------\n";
    std::cout << "Цвет подвала: Текст = " << footerColor.textColor
              << ", Фон = " << footerColor.bgColor << "\n";
    std::cout << "Всего строк: " << computed << ", Callback вызван: " << callbackCounter << " раз(а).\n";

    // Сброс цвета
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    FreeLibrary(hLib);
    return 0;
}
