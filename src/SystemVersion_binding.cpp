
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstring>
#include "SystemVersion.hpp"

namespace py = pybind11;

void bind_(py::module &m) {
    auto cls = py::class_<SystemVersion, CalibrationGroup>(m, "", R"pbdoc(System version information)pbdoc");
    cls.def(py::init<>());

    cls.def_readwrite("major", &SystemVersion::major, "Major version");
    cls.def_readwrite("minor", &SystemVersion::minor, "Minor version");

    cls.def("to_bytes", [](const SystemVersion& self) {
        return py::bytes(reinterpret_cast<const char*>(&self), sizeof(SystemVersion));
    });

    cls.def_static("from_bytes", [](py::bytes data) {
        std::string s = data;
        if (s.size() != sizeof(SystemVersion)) throw py::value_error("Size mismatch");
        SystemVersion obj;
        std::memcpy(&obj, s.data(), sizeof(SystemVersion));
        return obj;
    });
}