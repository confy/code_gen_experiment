


#pragma once
#include "CalibrationGroup.h"
#include "SystemVersion.h"
#include "FrownCal.h"

#pragma pack(push, 1)
struct ProductCalibration : public CalibrationGroup {
    SystemVersion systemVersion;
    FrownCal frownCal;
};
#pragma pack(pop)