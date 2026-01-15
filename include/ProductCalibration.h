
#pragma once
#include "CalibrationGroup.hpp"
#include "SystemVersion.hpp"
#include "FrownCal.hpp"

#pragma pack(push, 1)
struct ProductCalibration : public CalibrationGroup {
    SystemVersion systemVersion;
    FrownCal frownCal;
};
#pragma pack(pop)