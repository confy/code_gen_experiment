#pragma once
#include "CalibrationGroup.h"

/**
 * @file CalibrationGroups.h
 * @brief All calibration group structs in one file.
 */

/** THIS FILE IS AUTO-GENERATED. DO NOT EDIT. */

#pragma pack(push, 1)
/** System version information */
struct SystemVersion : public CalibrationGroup {
    /** Major version */
    float major;
    /** Minor version */
    float minor;
};

/** Frown correction parameters */
struct FrownCal : public CalibrationGroup {
    /** Frown correction map */
    float mapping[192];
};

/** Product specific group */
struct ProductSpecificGroup : public CalibrationGroup {
    /** A test field */
    float testField;
    /** A test array */
    float testArray[10];
};
#pragma pack(pop)