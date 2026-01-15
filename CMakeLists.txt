cmake_minimum_required(VERSION 3.14)
project(CalibrationCodeGen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python and pybind11
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)


add_custom_target(generate_calib_code ALL
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/generate.py
        ${CMAKE_CURRENT_SOURCE_DIR}/definitions/*.yaml
        ${CMAKE_CURRENT_SOURCE_DIR}/templates/group.h.j2
        ${CMAKE_CURRENT_SOURCE_DIR}/templates/set.h.j2
        ${CMAKE_CURRENT_SOURCE_DIR}/templates/pybind.cpp.j2
    COMMENT "Generating calibration code from YAML"
)

# Glob all generated sources/headers
file(GLOB GENERATED_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB GENERATED_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

# Main library

add_library(calib_codegen STATIC ${GENERATED_SRC} ${GENERATED_HEADERS})
add_dependencies(calib_codegen generate_calib_code)
target_include_directories(calib_codegen PUBLIC include)
target_link_libraries(calib_codegen PUBLIC pybind11::pybind11)


# Python module: main entry + all *_binding.cpp
file(GLOB BINDING_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*_binding.cpp)
set(PYBIND_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/src/calib_pybind.cpp)
pybind11_add_module(calib_pybind ${PYBIND_MAIN} ${BINDING_SRC})
add_dependencies(calib_pybind generate_calib_code)
target_link_libraries(calib_pybind PRIVATE calib_codegen pybind11::pybind11)
target_include_directories(calib_pybind PRIVATE include)

# Optionally, install targets
# install(TARGETS calib_codegen calib_pybind DESTINATION lib)
# install(DIRECTORY include/ DESTINATION include)
