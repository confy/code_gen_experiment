#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstring>
#include "FrownCal.h"

namespace py = pybind11;

void bind_frown_cal(py::module &m) {
    auto cls = py::class_<FrownCal, CalibrationGroup>(m, "frown_cal", R"pbdoc(Frown correction parameters)pbdoc");
    cls.def(py::init<>());

        cls.def_property("mapping",
            [](const FrownCal &self) {
                return std::vector<float>(self.mapping, self.mapping + 192);
            },
            [](FrownCal &self, const std::vector<float> &v) {
                if (v.size() != 192) throw py::value_error("Expected size 192");
                std::memcpy(self.mapping, v.data(), 192 * sizeof(float));
            }, "Frown correction map");

    cls.def("to_bytes", [](const FrownCal& self) {
        return py::bytes(reinterpret_cast<const char*>(&self), sizeof(FrownCal));
    });

    cls.def_static("from_bytes", [](py::bytes data) {
        std::string s = data;
        if (s.size() != sizeof(FrownCal)) throw py::value_error("Size mismatch");
        FrownCal obj;
        std::memcpy(&obj, s.data(), sizeof(FrownCal));
        return obj;
    });
}