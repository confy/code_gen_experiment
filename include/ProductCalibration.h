#pragma once
#include "CalibrationGroup.h"
#include <json.hpp>
#include "SystemVersion.h"
#include "FrownCal.h"
#include "ProductSpecificGroup.h"

using json = nlohmann::json;

#pragma pack(push, 1)
struct ProductCalibration : public CalibrationGroup {
    SystemVersion systemVersion;
    FrownCal frownCal;
    ProductSpecificGroup productSpecificGroup;
};
#pragma pack(pop)

inline void to_json(json& j, const ProductCalibration& p) {
    j = json{
        {"SystemVersion", p.systemVersion},        {"FrownCal", p.frownCal},        {"ProductSpecificGroup", p.productSpecificGroup}    };
}

inline void from_json(const json& j, ProductCalibration& p) {
    j.at("SystemVersion").get_to(p.systemVersion);
    j.at("FrownCal").get_to(p.frownCal);
    j.at("ProductSpecificGroup").get_to(p.productSpecificGroup);
}