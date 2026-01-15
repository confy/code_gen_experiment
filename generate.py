import yaml
import os
from jinja2 import Environment, FileSystemLoader

import re
def to_snake_case(name):
    """Convert CamelCase to snake_case."""
    return re.sub(r'(?<!^)(?=[A-Z])', '_', name).lower()

def snake_to_camel(name):
    parts = name.split('_')
    return parts[0] + ''.join(word.capitalize() for word in parts[1:])

def process_fields(fields):
    result = []
    for field in fields:
        field_snake = field['name']
        field_camel = snake_to_camel(field_snake)
        # For C++ member variable: lowerCamelCase (first letter lowercase)
        field_member = field_camel[0].lower() + field_camel[1:] if field_camel else field_camel
        field_copy = field.copy()
        field_copy['name_snake'] = field_snake
        field_copy['name_camel'] = field_camel
        field_copy['name_member'] = field_member
        result.append(field_copy)
    return result

def generate_group_files(env, group, is_set):
    name_snake = to_snake_case(group['name'])
    name_camel = snake_to_camel(group['name'])
    group['name_camel'] = name_camel
    group['name_snake'] = name_snake
    group['fields'] = process_fields(group.get('fields', []))
    tmpl_name = 'set.h.j2' if is_set else 'group.h.j2'
    with open(f"include/{name_camel}.h", 'w') as f:
        f.write(env.get_template(tmpl_name).render(group=group))

    binding_data = group.copy()
    if is_set:
        binding_data['fields'] = process_fields(group.get('members', []))
    with open(f"src/{name_camel}_binding.cpp", 'w') as f:
        f.write(env.get_template('pybind.cpp.j2').render(group=binding_data))

def generate():
    env = Environment(loader=FileSystemLoader('templates'), trim_blocks=True, lstrip_blocks=True)

    # Concatenate all YAMLs in definitions/ for cross-file anchors
    import glob
    yaml_files = glob.glob('definitions/*.yaml')
    # Ensure groups.yaml is first
    yaml_files = sorted(yaml_files, key=lambda x: (not x.endswith('groups.yaml'), x))
    yaml_text = ''
    for yf in yaml_files:
        with open(yf, 'r') as f:
            yaml_text += f.read() + '\n'
    print(f'Loaded YAML from: {yaml_files}')
    print('YAML Content:')
    print(yaml_text)
    try:
        data = yaml.safe_load(yaml_text)
    except Exception as e:
        print(f"YAML load error: {e}")
        data = {}

    # Merge any product-specific group lists (e.g., product_groups) into 'groups', always preserving groups.yaml
    merged_groups = []
    if data and 'groups' in data:
        merged_groups.extend(data['groups'])
    if data:
        for key in list(data.keys()):
            if key.endswith('_groups') and key != 'groups':
                merged_groups.extend(data[key])
                del data[key]
        data['groups'] = merged_groups
    else:
        data = {'groups': []}

    print('Groups:')
    for group in data['groups']:
        print(f"  - {group['name']}: {group}")

    os.makedirs('include', exist_ok=True)
    os.makedirs('src', exist_ok=True)

    all_groups = []
    group_structs = []
    for item in data['groups']:
        is_set = item.get('is_set', False)
        item['name_camel'] = snake_to_camel(item['name'])
        item['fields'] = process_fields(item.get('fields', []))
        struct_code = env.get_template('group_struct.j2').render(group=item)
        group_structs.append(struct_code.strip())
        all_groups.append({
            'name_snake': to_snake_case(item['name']),
            'name_camel': snake_to_camel(item['name'])
        })
        # Explicitly generate group header and binding for every group
        with open(f"include/{item['name_camel']}.h", 'w') as f:
            f.write(env.get_template('group.h.j2').render(group=item))
        # Ensure name_snake is set before using it
        name_snake = to_snake_case(item['name'])
        binding_group = item.copy()
        binding_group['name_snake'] = name_snake
        binding_group['binding_func'] = f"bind_{name_snake}"
        with open(f"src/{item['name_camel']}_binding.cpp", 'w') as f:
            f.write(env.get_template('pybind.cpp.j2').render(group=binding_group))

    # Generate code for each top-level calibration group except 'groups' (sets)
    for key, value in data.items():
        if key == 'groups':
            continue
        for entry in value:
            # treat these as sets
            entry['fields'] = entry.get('members', [])
            generate_group_files(env, entry, True)
            all_groups.append({
                'name_snake': to_snake_case(entry['name']),
                'name_camel': snake_to_camel(entry['name'])
            })

    # Generate CalibrationGroups.h with all group structs
    with open('include/CalibrationGroups.h', 'w') as f:
        f.write(env.get_template('CalibrationGroups.h.j2').render(group_structs=group_structs))

    # Generate main pybind module file
    with open('src/calib_pybind.cpp', 'w') as f:
        f.write(env.get_template('pybind_main.cpp.j2').render(all_groups=all_groups))

    print(f"Generated {len(data['groups'])} group files and {sum(len(v) for k,v in data.items() if k != 'groups')} set files.")

if __name__ == "__main__":
    generate()