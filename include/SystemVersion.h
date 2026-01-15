#pragma once

#include "CalibrationGroup.h"
#include <json.hpp>
#include <vector>
#include <algorithm>
#include <stdexcept>

using json = nlohmann::json;

struct SystemVersion : public CalibrationGroup {
    float major = 0;
    float minor = 0;

    SystemVersion() = default;
    ~SystemVersion() = default;
};

inline void to_json(json& j, const SystemVersion& p) {
    j = json{
        {"major", p.major},        {"minor", p.minor}    };
}

inline void from_json(const json& j, SystemVersion& p) {
    j.at("major").get_to(p.major);
    j.at("minor").get_to(p.minor);
}
