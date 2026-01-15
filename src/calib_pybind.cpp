#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

/** THIS FILE IS AUTO-GENERATED. DO NOT EDIT. */

void bind_system_version(pybind11::module&);
void bind_frown_cal(pybind11::module&);
void bind_product_specific_group(pybind11::module&);
void bind_product_calibration(pybind11::module&);

PYBIND11_MODULE(calib_pybind, m) {
    bind_system_version(m);
    bind_frown_cal(m);
    bind_product_specific_group(m);
    bind_product_calibration(m);
}