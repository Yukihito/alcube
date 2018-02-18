import yaml
import re
from typing import Dict
import sys
from . import errors
from . import utils


class DTOInstance:
    def __init__(self, name, type_name, length, is_host_ptr):
        self.name = name
        self.type_name = type_name.replace('unsigned ', 'u')
        self.length = length
        self.is_host_ptr = is_host_ptr

    def __str__(self):
        result = '{} {}'.format(self.type_name, self.name)
        if self.length is not None:
            result += '[{}]'.format(self.length)
        if self.is_host_ptr:
            result += ' (host ptr)'
        return result


class TypeDefinition:
    def __init__(self, name, cpp_name, size, is_struct=True):
        self.name = name
        self.cpp_name = cpp_name
        self.size = size
        self.is_struct = is_struct

    def get_cpp_dto_type_name_with_namespace(self):
        if self.is_struct:
            return 'dtos::{}'.format(self.name)
        else:
            return self.cpp_name

    def get_cpp_memory_type_name(self):
        if self.is_struct:
            return self.name
        else:
            return '{}Memory'.format(self.name.capitalize())

    def get_cpp_memory_type_name_with_namespace(self):
        return 'memories::{}'.format(self.get_cpp_memory_type_name())


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
        self.type_definition = TypeDefinition(self.name, self.name, self.calculate_size())
        defined_type_definitions_map[self.name] = self.type_definition
        for field in self.fields:
            field.type_definition = defined_type_definitions_map[field.type_name]

    def calculate_size(self):
        return sum(field.size for field in self.fields)


class Definition:
    """
    :type dto_instances: list[DTOInstance]
    :type definition: None|dict
    :type dto_types: dict[str, TypeDefinition]
    :type dto_structs: list[StructDefinition]
    :type variables: list[str]
    """
    def __init__(self):
        self.kernels_directory_paths = []
        self.definition = None
        self.dto_instances = []
        self.dto_types = {}
        self.dto_structs = []
        self.variables = []

    def load_dto_instances(self):
        raw_dto_instances = self.definition['dto-instances']
        self.dto_instances = []
        for raw_dto_instance in raw_dto_instances:
            length = None if 'length' not in raw_dto_instance else raw_dto_instance['length']
            is_host_ptr = 'host-ptr' in raw_dto_instance and raw_dto_instance['host-ptr']
            self.dto_instances.append(
                DTOInstance(raw_dto_instance['name'], raw_dto_instance['type'], length, is_host_ptr))

    def load_dto_structs(self):
        builtin_type_definitions = {
            TypeDefinition('char', 'char', 1, is_struct=False),
            TypeDefinition('uchar', 'unsigned char', 1, is_struct=False),
            TypeDefinition('short', 'short', 2, is_struct=False),
            TypeDefinition('ushort', 'unsigned short', 2, is_struct=False),
            TypeDefinition('int', 'int', 4, is_struct=False),
            TypeDefinition('uint', 'unsigned int', 4, is_struct=False),
            TypeDefinition('long', 'long', 8, is_struct=False),
            TypeDefinition('ulong', 'unsigned long', 8, is_struct=False),
            TypeDefinition('float', 'float', 4, is_struct=False),
            TypeDefinition('double', 'double', 8, is_struct=False),
            TypeDefinition('float3', 'cl_float3', 4 * 3, is_struct=False),
            TypeDefinition('float4', 'cl_float4', 4 * 4, is_struct=False)
        }

        defined_type_definitions_map = dict()
        for type_definition in builtin_type_definitions:
            defined_type_definitions_map[type_definition.name] = type_definition
        if self.definition is None:
            raise errors.InputNotSpecified()
        raw_struct_definitions = self.definition['dto-structs']

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
        self.dto_structs = sorted_struct_definitions
        self.dto_types = defined_type_definitions_map

    def load(self):
        if utils.input_file_name:
            self.definition = yaml.load(utils.load_text((utils.input_file_name)))
        else:
            self.definition = yaml.load(sys.stdin.read())
        self.kernels_directory_paths = self.definition['kernels-directories']
        self.variables = self.definition['variables']
        self.load_dto_instances()
        self.load_dto_structs()
