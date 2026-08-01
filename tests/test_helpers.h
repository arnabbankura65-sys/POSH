#pragma once
#include <iostream>
#include <string>
#include <vector>

inline int tests_run = 0;
inline int tests_failed = 0;
inline std::string vec_to_string(const std::vector<std::string>& v) {
    std::string result = "{";
    for (size_t i = 0; i < v.size(); ++i) {
        result += "\"" + v[i] + "\"";
        if (i + 1 < v.size()) result += ", ";
    }
    result += "}";
    return result;
}

#define EXPECT(cond) do { \
    ++tests_run; \
    if (!(cond)) { \
        ++tests_failed; \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " EXPECT(" #cond ")\n"; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    ++tests_run; \
    auto _a = (a); auto _b = (b); \
    if (!(_a == _b)) { \
        ++tests_failed; \
        std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ \
                  << " EXPECT_EQ failed\n"; \
    } \
} while (0)

