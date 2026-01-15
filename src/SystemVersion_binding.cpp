#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <cstring>
#include "SystemVersion.h"
#include <json.hpp>
#include <string>

namespace py = pybind11;
using json = nlohmann::json;

void bind_system_version(py::module &m) {
    auto cls = py::class_<SystemVersion, CalibrationGroup>(m, "system_version", R"pbdoc(System version information)pbdoc");
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

    cls.def("to_json_string", [](const SystemVersion& self) {
        json j = self;
        return j.dump();
    });

    cls.def_static("from_json_string", [](const std::string& json_string) {
        auto j = json::parse(json_string);
        return j.get<SystemVersion>();
    });
}