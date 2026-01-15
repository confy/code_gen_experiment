# Code gen experiment

generate.py will generate C++ and pybind bindings from YAML definitions in the definitions/ directory.

The generation can be run alone or as part of the CMake build.

## Usage

```bash
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

```bash
cmake -S . -B build
cmake --build build
```