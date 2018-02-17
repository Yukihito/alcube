from os import path
from .utils import *
from .definitionloader import Definition


def create_cpp_text(struct_definitions):
    """
    :type struct_definitions: list[StructDefinition]
    """
    source_dir_path = path.dirname(path.abspath(__file__))
    template_dir_path = '{}/src/templates/cpp/gpu'.format(source_dir_path[:-len('/codegenerator')])
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


def create_clc_text(struct_definitions):
    """
    :type struct_definitions: list[StructDefinition]
    """
    source_dir_path = path.dirname(path.abspath(__file__))
    template_dir_path = '{}/src/templates/kernels'.format(source_dir_path[:-len('/codegenerator')])
    root_template = load_template('{}/dtos.cl.template'.format(template_dir_path))
    struct_template = load_template('{}/dtos.cl-struct.template'.format(template_dir_path))
    field_template = load_template('{}/dtos.cl-field.template'.format(template_dir_path))
    field_with_index_template = load_template('{}/dtos.cl-field-with-index.template'.format(template_dir_path))
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
    definition = Definition()
    definition.load()
    print(create_text(definition.dto_structs))
