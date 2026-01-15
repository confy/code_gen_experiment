#pragma once

#include "CalibrationGroup.h"
#include <json.hpp>
#include <vector>
#include <algorithm>
#include <stdexcept>

using json = nlohmann::json;

struct ProductSpecificGroup : public CalibrationGroup {
    float testField = 0;
    float testArray[10];

    ProductSpecificGroup() = default;
    ~ProductSpecificGroup() = default;
};

inline void to_json(json& j, const ProductSpecificGroup& p) {
    j = json{
        {"test_field", p.testField},        {"test_array", p.testArray}    };
}

inline void from_json(const json& j, ProductSpecificGroup& p) {
    j.at("test_field").get_to(p.testField);
    const auto& test_array_json = j.at("test_array");
    if (test_array_json.size() != 10) {
        throw std::runtime_error("Incorrect array size for test_array");
    }
    std::copy(test_array_json.begin(), test_array_json.end(), p.testArray);
}
