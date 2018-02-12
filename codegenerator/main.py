from os import path
import yaml
import re
from typing import Dict
import sys
from string import Template
import argparse
from . import errors


def load_template(template_path):
    text = ''
    with open(template_path, encoding='utf-8') as file:
        for line in file:
            text += line
    return Template(text)


class TypeDefinition:
    def __init__(self, name, cpp_name, size):
        self.name = name
        self.cpp_name = cpp_name
        self.size = size


class FieldDefinition:
    """
    :type owner: StructDefinition
    :type type_definition: TypeDefinition
    """
    def __init__(self, owner, type_name, name, length):
        self.type_name = type_name
        self.name = name
        self.length = 1 if length is None else length
        self.size = 0
        self.owner = owner
        self.type_definition = None

    def __str__(self):
        result = self.type_name
        if self.length > 1:
            result += '[{}]'.format(str(self.length))
        result += '\t{}\t({})'.format(self.name, self.size)
        return result

    def calculate_size(self, defined_definitions_map: Dict[str, TypeDefinition]):
        self.size = defined_definitions_map[self.type_name].size * self.length


class StructDefinition:
    """
    :type fields: list[FieldDefinition]
    :type type_definition: None|TypeDefinition
    """
    def __init__(self, name, raw_field_definitions):
        self.name = name
        self.fields = []

        for raw_field_definition in raw_field_definitions:
            regex = r'^\s*(?P<type_name>\w+)(?P<length>(\[\d+\])?)\s+(?P<field_name>\w+)\s*$'
            result = re.match(regex, raw_field_definition)
            if result is None:
                raise errors.InvalidFieldFormatDefinition(name, raw_field_definition)
            type_name = result.group('type_name')
            length = None
            field_name = result.group('field_name')
            if result.group('length') != '':
                length = int(result.group('length')[1:-1])
            self.fields.append(FieldDefinition(self, type_name, field_name, length))
        self.type_definition = None

    def __str__(self):
        result = '{}\t({})\n'.format(self.name, 0 if self.type_definition is None else self.type_definition.size)
        for field in self.fields:
            result += '\t{}\n'.format(str(field))
        return result

    def can_define(self, defined_type_definitions_map):
        return all(field.type_name in defined_type_definitions_map for field in self.fields)

    def pad(self, defined_type_definitions_map):
        fields = []
        padding_index = 0
        for field in self.fields:
            field.calculate_size(defined_type_definitions_map)
            fields.append(field)
            padding_size = field.size % 4
            if padding_size != 0:
                padding = FieldDefinition(self, 'char', '_padding{}'.format(str(padding_index)), padding_size)
                padding.calculate_size(defined_type_definitions_map)
                fields.append(padding)
                padding_index += 1
        self.fields = fields

    def define(self, defined_type_definitions_map):
        self.pad(defined_type_definitions_map)
        self.type_definition = TypeDefinition(self.name, self.name, sum(field.size for field in self.fields))
        defined_type_definitions_map[self.name] = self.type_definition
        for field in self.fields:
            field.type_definition = defined_type_definitions_map[field.type_name]


def create_cpp_text(struct_definitions):
    """
    :type struct_definitions: list[StructDefinition]
    """
    source_dir_path = path.dirname(path.abspath(__file__))
    template_dir_path = '{}/src/templates/cpp/physics/opencl'.format(source_dir_path[:-len('/codegenerator')])
    root_template = load_template('{}/dtos.h.template'.format(template_dir_path))
    class_template = load_template('{}/dtos.h-class.template'.format(template_dir_path))
    field_template = load_template('{}/dtos.h-field.template'.format(template_dir_path))
    field_with_index_template = load_template('{}/dtos.h-field-with-index.template'.format(template_dir_path))
    classes_texts = []
    for struct_definition in struct_definitions:
        fields_texts = []
        for field_definition in struct_definition.fields:
            if field_definition.length > 1:
                fields_texts.append(field_with_index_template.substitute(
                    type_name=field_definition.type_definition.cpp_name,
                    name=field_definition.name,
                    length=field_definition.length
                ))
            else:
                fields_texts.append(field_template.substitute(
                    type_name=field_definition.type_definition.cpp_name,
                    name=field_definition.name
                ))
        classes_texts.append(class_template.substitute(
            class_name=struct_definition.name,
            fields='\n'.join(fields_texts)
        ))
    text = root_template.substitute(classes='\n\n'.join(classes_texts))
    return text


def create_kernel_text(struct_definitions):
    """
    :type struct_definitions: list[StructDefinition]
    """
    source_dir_path = path.dirname(path.abspath(__file__))
    template_dir_path = '{}/src/templates/kernels/physics'.format(source_dir_path[:-len('/codegenerator')])
    root_template = load_template('{}/generatedcode.cl.template'.format(template_dir_path))
    struct_template = load_template('{}/generatedcode.cl-struct.template'.format(template_dir_path))
    field_template = load_template('{}/generatedcode.cl-field.template'.format(template_dir_path))
    field_with_index_template = load_template('{}/generatedcode.cl-field-with-index.template'.format(template_dir_path))
    structs_texts = []
    for struct_definition in struct_definitions:
        fields_texts = []
        for field_definition in struct_definition.fields:
            if field_definition.length > 1:
                fields_texts.append(field_with_index_template.substitute(
                    type_name=field_definition.type_definition.name,
                    name=field_definition.name,
                    length=field_definition.length
                ))
            else:
                fields_texts.append(field_template.substitute(
                    type_name=field_definition.type_definition.name,
                    name=field_definition.name
                ))
        structs_texts.append(struct_template.substitute(
            name=struct_definition.name,
            fields='\n'.join(fields_texts)
        ))
    text = root_template.substitute(structs='\n\n'.join(structs_texts))
    return text


def generate(create_text):
    builtin_type_definitions = {
        TypeDefinition('char', 'char', 1),
        TypeDefinition('uchar', 'unsigned char', 1),
        TypeDefinition('short', 'short', 2),
        TypeDefinition('ushort', 'unsigned short', 2),
        TypeDefinition('int', 'int', 4),
        TypeDefinition('uint', 'unsigned int', 4),
        TypeDefinition('long', 'long', 8),
        TypeDefinition('ulong', 'unsigned long', 8),
        TypeDefinition('float', 'float', 4),
        TypeDefinition('double', 'double', 8),
        TypeDefinition('float3', 'cl_float3', 4 * 3),
        TypeDefinition('float4', 'cl_float4', 4 * 4)
    }

    defined_type_definitions_map = dict()
    for type_definition in builtin_type_definitions:
        defined_type_definitions_map[type_definition.name] = type_definition
    raw_struct_definitions = yaml.load(sys.stdin.read())
    if raw_struct_definitions is None:
        raise errors.InputNotSpecified()
    struct_definitions = []
    items = list(raw_struct_definitions.items())
    items.sort()
    for name, raw_field_definitions in items:
        struct_definition = StructDefinition(name, raw_field_definitions)
        struct_definitions.append(struct_definition)

    sorted_struct_definitions = []
    next_struct_definitions = []
    """:type next_definitions: list[StructDefinition]"""
    current_struct_definitions = struct_definitions[:]
    while True:
        for struct_definition in current_struct_definitions:
            if struct_definition.can_define(defined_type_definitions_map):
                struct_definition.define(defined_type_definitions_map)
                sorted_struct_definitions.append(struct_definition)
            else:
                next_struct_definitions.append(struct_definition)
        if len(current_struct_definitions) == len(next_struct_definitions):
            break
        current_struct_definitions = next_struct_definitions[:]
        next_struct_definitions = []
    if len(next_struct_definitions) > 0:
        undefined_type_field_definitions = []
        for struct_definition in next_struct_definitions:
            for field_definition in struct_definition.fields:
                if field_definition.type_name not in defined_type_definitions_map:
                    undefined_type_field_definitions.append(field_definition)
        raise errors.UndefinedTypeFieldDefinitions(undefined_type_field_definitions)
    print(create_text(sorted_struct_definitions))


def route():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-f', '--format',
        type=str,
        help='File format to generate. e.g. cpp, kernel'
    )
    args = parser.parse_args()
    if args.format == 'cpp':
        generate(create_cpp_text)
    elif args.format == 'kernel':
        generate(create_kernel_text)
    elif args.format is None:
        raise errors.FormatNotSpecified(parser)
    else:
        raise errors.UnsupportedFormat(args.format)


def handle_error(f):
    try:
        f()
    except errors.InvalidFieldFormatDefinition as e:
        print('Invalid field format in definition of {}: {}'.format(
            e.type_name, e.specified_text), file=sys.stderr)
        exit(1)
    except errors.UndefinedTypeFieldDefinitions as e:
        for field_definition in e.field_definitions:
            print('Undefined type used in definition of {}#{}: {}'.format(
                field_definition.owner.name, field_definition.name, field_definition.type_name), file=sys.stderr)
        exit(1)
    except errors.InputNotSpecified:
        print('Input not specified', file=sys.stderr)
        exit(1)
    except errors.UnsupportedFormat as e:
        print('Unsupported format: {} ("cpp" or "kernel" required.)'.format(e.format), file=sys.stderr)
        exit(1)
    except errors.FormatNotSpecified as e:
        print('--format or -f required', file=sys.stderr)
        e.parser.print_help()
        exit(1)


def _main():
    handle_error(route)


if __name__ == '__main__':
    _main()
