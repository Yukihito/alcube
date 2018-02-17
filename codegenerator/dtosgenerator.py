from .utils import *
from .definitionloader import Definition


def create_cpp_text(struct_definitions):
    """
    :type struct_definitions: list[StructDefinition]
    """
    root_template = load_template('cpp/gpu/dtos.h.template')
    class_template = load_template('cpp/gpu/dtos.h-class.template')
    field_template = load_template('cpp/gpu/dtos.h-field.template')
    field_with_index_template = load_template('cpp/gpu/dtos.h-field-with-index.template')
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
    root_template = load_template('kernels/dtos.cl.template')
    struct_template = load_template('kernels/dtos.cl-struct.template')
    field_template = load_template('kernels/dtos.cl-field.template')
    field_with_index_template = load_template('kernels/dtos.cl-field-with-index.template')
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
