from os import path
import yaml
import re
from typing import Dict


class TypeDefinition:
    def __init__(self, name, size):
        self.name = name
        self.size = size


class InvalidFieldFormatDefinition(Exception):
    def __init__(self, type_name, specified_text):
        self.type_name = type_name
        self.specified_text = specified_text


class UndefinedTypeFieldDefinitions(Exception):
    """
    :type field_definitions: list[FieldDefinition]
    """
    def __init__(self, field_definitions):
        self.field_definitions = field_definitions


class FieldDefinition:
    def __init__(self, owner, type_name, name, length):
        self.type_name = type_name
        self.name = name
        self.length = 1 if length is None else length
        self.size = 0
        self.owner = owner

    def __str__(self):
        result = self.type_name
        if self.length is not None:
            result += '({})'.format(str(self.length))
        result += '\t{}'.format(self.name)
        return result

    def calculate_size(self, defined_definitions_map: Dict[str, TypeDefinition]):
        self.size = defined_definitions_map[self.type_name].size * self.length


class Definition:
    """
    :type fields: list[FieldDefinition]
    """
    def __init__(self, name, raw_field_definitions):
        self.name = name
        self.fields = []

        for raw_field_definition in raw_field_definitions:
            regex = r'(?P<type_name>\w+)(?P<length>(\[\d+\])?)\s+(?P<field_name>\w+)\s*'
            result = re.match(regex, raw_field_definition)
            if result is None:
                raise InvalidFieldFormatDefinition(name, raw_field_definition)
            type_name = result.group('type_name')
            length = None
            field_name = result.group('field_name')
            if result.group('length') != '':
                length = int(result.group('length')[1:-1])
            self.fields.append(FieldDefinition(self, type_name, field_name, length))
        self.type_definition = None

    def __str__(self):
        result = self.name + '\n'
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
                fields.append(FieldDefinition(self, 'char', '_padding{}'.format(str(padding_index)), padding_size))
                padding_index += 1
        self.fields = fields

    def define(self, defined_type_definitions_map):
        self.pad(defined_type_definitions_map)
        self.type_definition = TypeDefinition(self.name, sum(field.size for field in self.fields))
        defined_type_definitions_map[self.name] = self.type_definition


def generate():
    builtin_type_definitions = {
        TypeDefinition('char', 1),
        TypeDefinition('uchar', 1),
        TypeDefinition('short', 2),
        TypeDefinition('ushort', 2),
        TypeDefinition('int', 4),
        TypeDefinition('uint', 4),
        TypeDefinition('long', 8),
        TypeDefinition('ulong', 8),
        TypeDefinition('float', 4),
        TypeDefinition('double', 8),
        TypeDefinition('float3', 4 * 3),
        TypeDefinition('float4', 4 * 4)
    }

    defined_type_definitions_map = dict()
    for definition in builtin_type_definitions:
        defined_type_definitions_map[definition.name] = definition

    source_dir_path = path.dirname(path.abspath(__file__))
    dtos_definition_file_path = '{}/dtos.yml'.format(source_dir_path)
    with open(dtos_definition_file_path, encoding='utf-8') as file:
        raw_definitions = yaml.load(file)

    definitions = []
    for name, raw_field_definitions in raw_definitions.items():
        definition = Definition(name, raw_field_definitions)
        definitions.append(definition)

    sorted_definitions = []
    next_definitions = []
    ''':type next_definitions: list[Definition]'''
    current_definitions = definitions[:]
    while True:
        for definition in current_definitions:
            if definition.can_define(defined_type_definitions_map):
                definition.define(defined_type_definitions_map)
                sorted_definitions.append(definition)
            else:
                next_definitions.append(definition)
        if len(current_definitions) == len(next_definitions):
            break
        current_definitions = next_definitions[:]
        next_definitions = []
    if len(next_definitions) > 0:
        undefined_type_field_definitions = []
        for definition in next_definitions:
            for field_definition in definition.fields:
                if field_definition.type_name not in defined_type_definitions_map:
                    undefined_type_field_definitions.append(field_definition)
        raise UndefinedTypeFieldDefinitions(undefined_type_field_definitions)
    # TODO: write


if __name__ == '__main__':
    generate()
