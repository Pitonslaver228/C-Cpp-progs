#include "calc_lib.h"
#include <cmath>
#include <cstring>

const char* get_function_name() {
    return "cos^3(x)";
}

const char* get_author_surname() {
    return "Гайбатов Р. Р.";
}

Color get_table_header_color() {
    return { "white", "blue" };
}

Color get_table_footer_color() {
    return { "white", "green" };
}

// Точная функция: cos^3(x) = (3cos(x) + cos(3x)) / 4
double exact_value(double x) {
    return (3 * std::cos(x) + std::cos(3 * x)) / 4.0;
}

// разложение по Тейлору для cos^3(x)
// cos^3(x) = (1/4)(3 cos(x) + cos(3x)) — используем разложение cos(x) и cos(3x)
double cos_series(double x, double eps) {
    double term = 1.0;
    double sum = 0.0;
    int n = 0;

    while (std::abs(term) > eps) {
        term = std::pow(-1, n) * std::pow(x, 2 * n) / tgamma(2 * n + 1);
        sum += term;
        if (n++ > 1000) break;
    }

    return sum;
}

double series_value(double x, double eps) {
    // разложеНИЕ
    double cos1 = 0.0;
    double cos3 = 0.0;
    double term;
    int n = 0;

    do {
        term = std::pow(-1, n) * std::pow(x, 2 * n) / tgamma(2 * n + 1);
        cos1 += term;
        ++n;
    } while (std::abs(term) > eps && n < 1000);

    n = 0;
    do {
        term = std::pow(-1, n) * std::pow(3 * x, 2 * n) / tgamma(2 * n + 1);
        cos3 += term;
        ++n;
    } while (std::abs(term) > eps && n < 1000);

    return (3 * cos1 + cos3) / 4.0;
}

double compute_delta(double f_series, double f_exact) {
    return std::sqrt(std::abs(f_series * f_series - f_exact * f_exact));
}

int compute_values(double start, double end, double step,
                   double epsilon,
                   ColorCallback cb,
                   Result* results, int maxCount) {
    int count = 0;
    int callCount = 0;

    for (double x = start; x <= end + 1e-9 && count < maxCount; x += step) {
        double sVal = series_value(x, epsilon);
        double eVal = exact_value(x);
        double delta = compute_delta(sVal, eVal);

        results[count] = { x, eVal, sVal, delta, true };

        if (cb) cb(++callCount);
        count++;
    }

    return count;
}
