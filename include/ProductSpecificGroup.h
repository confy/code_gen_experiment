#pragma once
#include "CalibrationGroup.h"

/**
    * @file ProductSpecificGroup.h
    * @brief Product specific group
    */


/** THIS FILE IS AUTO-GENERATED. DO NOT EDIT. */

#pragma pack(push, 1)
struct ProductSpecificGroup : public CalibrationGroup {
    /** A test field */
    float testField;
    /** A test array */
    float testArray[10];
};
#pragma pack(pop)
