#pragma once

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

struct Result {
    double x;
    double valueExact;
    double valueSeries;
    double delta;
    bool valid;
};

struct Color {
    const char* textColor;
    const char* bgColor;
};

typedef Color (*ColorCallback)(int callCount);

// Внешние функции
EXPORT const char* get_function_name();
EXPORT const char* get_author_surname();
EXPORT Color get_table_header_color();
EXPORT Color get_table_footer_color();

EXPORT int compute_values(
    double start, double end, double step,
    double epsilon,
    ColorCallback cb,
    Result* results, int maxCount
);

}
  
