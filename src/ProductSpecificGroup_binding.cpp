#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstring>
#include "ProductSpecificGroup.h"

namespace py = pybind11;

void bind_(py::module &m) {
    auto cls = py::class_<ProductSpecificGroup, CalibrationGroup>(m, "", R"pbdoc(Product specific group)pbdoc");
    cls.def(py::init<>());

    cls.def_readwrite("test_field", &ProductSpecificGroup::testField, "A test field");
        cls.def_property("test_array",
            [](const ProductSpecificGroup &self) {
                return std::vector<float>(self.testArray, self.testArray + 10);
            },
            [](ProductSpecificGroup &self, const std::vector<float> &v) {
                if (v.size() != 10) throw py::value_error("Expected size 10");
                std::memcpy(self.testArray, v.data(), 10 * sizeof(float));
            }, "A test array");

    cls.def("to_bytes", [](const ProductSpecificGroup& self) {
        return py::bytes(reinterpret_cast<const char*>(&self), sizeof(ProductSpecificGroup));
    });

    cls.def_static("from_bytes", [](py::bytes data) {
        std::string s = data;
        if (s.size() != sizeof(ProductSpecificGroup)) throw py::value_error("Size mismatch");
        ProductSpecificGroup obj;
        std::memcpy(&obj, s.data(), sizeof(ProductSpecificGroup));
        return obj;
    });
}