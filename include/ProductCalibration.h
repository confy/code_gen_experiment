#pragma once
#include "CalibrationGroup.h"
#include "SystemVersion.h"
#include "FrownCal.h"
#include "ProductSpecificGroup.h"

#pragma pack(push, 1)
struct ProductCalibration : public CalibrationGroup {
    SystemVersion systemVersion;
    FrownCal frownCal;
    ProductSpecificGroup productSpecificGroup;
};
#pragma pack(pop)