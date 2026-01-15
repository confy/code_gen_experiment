import yaml
import os
from jinja2 import Environment, FileSystemLoader

def snake_to_camel(name):
    parts = name.split('_')
    return parts[0] + ''.join(word.capitalize() for word in parts[1:])

def process_fields(fields):
    result = []
    for field in fields:
        field_snake = field['name']
        field_camel = snake_to_camel(field_snake)
        field_copy = field.copy()
        field_copy['name_snake'] = field_snake
        field_copy['name_camel'] = field_camel
        result.append(field_copy)
    return result

def generate_group_files(env, group, is_set):
    name_snake = group['name']
    name_camel = snake_to_camel(name_snake)
    group['name_camel'] = name_camel
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

    with open('calibration.yaml', 'r') as f:
        data = yaml.safe_load(f)

    print('Groups:')
    for group in data['groups']:
        print(f"  - {group['name']}: {group}")

    os.makedirs('include', exist_ok=True)
    os.makedirs('src', exist_ok=True)

    # Generate code for each group
    for item in data['groups']:
        is_set = item.get('is_set', False)
        generate_group_files(env, item, is_set)

    # Generate code for each top-level calibration group except 'groups'
    for key, value in data.items():
        if key == 'groups':
            continue
        for entry in value:
            # treat these as sets
            entry['fields'] = entry.get('members', [])
            generate_group_files(env, entry, True)

    print(f"Generated {len(data['groups'])} group files and {sum(len(v) for k,v in data.items() if k != 'groups')} set files.")

if __name__ == "__main__":
    generate()