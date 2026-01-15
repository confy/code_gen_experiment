#pragma once

#include "CalibrationGroup.h"
#include <json.hpp>
#include <vector>
#include <algorithm>
#include <stdexcept>

using json = nlohmann::json;

struct FrownCal : public CalibrationGroup {
    float mapping[192];

    FrownCal() = default;
    ~FrownCal() = default;
};

inline void to_json(json& j, const FrownCal& p) {
    j = json{
        {"mapping", p.mapping}    };
}

inline void from_json(const json& j, FrownCal& p) {
    const auto& mapping_json = j.at("mapping");
    if (mapping_json.size() != 192) {
        throw std::runtime_error("Incorrect array size for mapping");
    }
    std::copy(mapping_json.begin(), mapping_json.end(), p.mapping);
}
