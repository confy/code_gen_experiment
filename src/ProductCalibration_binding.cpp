#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstring>
#include "ProductCalibration.h"
#include <json.hpp>
#include <string>

namespace py = pybind11;
using json = nlohmann::json;

void bind_product_calibration(py::module &m) {
    auto cls = py::class_<ProductCalibration, CalibrationGroup>(m, "product_calibration", R"pbdoc(Product specific calibration data)pbdoc");
    cls.def(py::init<>());

    cls.def_readwrite("SystemVersion", &ProductCalibration::systemVersion, "System version information");
    cls.def_readwrite("FrownCal", &ProductCalibration::frownCal, "Frown correction parameters");
    cls.def_readwrite("ProductSpecificGroup", &ProductCalibration::productSpecificGroup, "Product specific group");

    cls.def("to_bytes", [](const ProductCalibration& self) {
        return py::bytes(reinterpret_cast<const char*>(&self), sizeof(ProductCalibration));
    });

    cls.def_static("from_bytes", [](py::bytes data) {
        std::string s = data;
        if (s.size() != sizeof(ProductCalibration)) throw py::value_error("Size mismatch");
        ProductCalibration obj;
        std::memcpy(&obj, s.data(), sizeof(ProductCalibration));
        return obj;
    });

    cls.def("to_json_string", [](const ProductCalibration& self) {
        json j = self;
        return j.dump();
    });

    cls.def_static("from_json_string", [](const std::string& json_string) {
        auto j = json::parse(json_string);
        return j.get<ProductCalibration>();
    });
}