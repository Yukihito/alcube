from .utils import *
from .definitionloader import Definition
from . import clcinspector


class TemplateProvider:
    """
    :type root: Template
    """
    def __init__(self, export_file_name):
        self.export_file_name = export_file_name
        self.root = load_template('{}.template'.format(export_file_name))
        self.templates = {}

    def of(self, name):
        """
        :rtype: Template
        """
        if name not in self.templates:
            self.templates[name] = load_template('{}-{}.template'.format(
                self.export_file_name,
                name
            ))
        return self.templates[name]


def generate_cpp_header_file():
    definition = Definition()
    definition.load()
    type_definitions = definition.dto_types
    aggregated_text = create_aggregated_kernel_text(definition.kernels_directory_paths)
    function_prototypes = clcinspector.create_function_prototypes(aggregated_text)
    kernel_function_prototypes = list(filter(lambda f: f.is_kernel, function_prototypes))

    root_template = load_template('cpp/gpu/GPU.h.template')
    dto_template = load_template('cpp/gpu/GPU.h-dto.template')
    dtos_template = load_template('cpp/gpu/GPU.h-dtos.template')
    kernel_function_template = load_template('cpp/gpu/GPU.h-kernel-function.template')
    kernels_template = load_template('cpp/gpu/GPU.h-kernels.template')
    memories_template = load_template('cpp/gpu/GPU.h-memories.template')
    memory_class_template = load_template('cpp/gpu/GPU.h-memory-class.template')
    memory_instance_template = load_template('cpp/gpu/GPU.h-memory-instance.template')
    raw_kernel_template = load_template('cpp/gpu/GPU.h-raw-kernel.template')

    variables_text = ',\n'.join(map(lambda s: '        unsigned int {}'.format(s), definition.variables))

    # Construct dtos part text.
    dto_texts = []
    for dto_instance in definition.dto_instances:
        dto_type_name = type_definitions[dto_instance.type_name].get_cpp_dto_type_name_with_namespace()
        name = dto_instance.name
        dto_text = dto_template.substitute(type_name=dto_type_name, name=name)
        dto_texts.append(dto_text)
    dtos_text = dtos_template.substitute(dtos='\n'.join(dto_texts))

    # Construct kernels part text.
    raw_kernel_texts = []
    for function_prototype in kernel_function_prototypes:
        raw_kernel_texts.append(raw_kernel_template.substitute(name=function_prototype.name))
    raw_kernels_text = '\n'.join(raw_kernel_texts)
    kernel_function_texts = []

    for function_prototype in kernel_function_prototypes:
        param_texts = []
        for param in function_prototype.params:
            type_definition = type_definitions[param.param_type.name]
            if param.param_type.is_pointer:
                type_name = '{}&'.format(type_definition.get_cpp_memory_type_name_with_namespace())
            else:
                type_name = type_definition.cpp_name
            param_text = '{} {}'.format(type_name, param.name)
            param_texts.append(param_text)
        params_text = ', '.join(param_texts)
        kernel_function_text = kernel_function_template.substitute(name=function_prototype.name, parameters=params_text)
        kernel_function_texts.append(kernel_function_text)

    kernel_functions_text = '\n'.join(kernel_function_texts)
    kernels_text = kernels_template.substitute(raw_kernels=raw_kernels_text, kernel_functions=kernel_functions_text)

    # Construct memories part text.
    memory_instance_texts = []
    for dto_instance in definition.dto_instances:
        type_definition = type_definitions[dto_instance.type_name]
        memory_type_name = type_definition.get_cpp_memory_type_name_with_namespace()
        memory_instance_text = memory_instance_template.substitute(
            memory_type_name=memory_type_name, name=dto_instance.name)
        memory_instance_texts.append(memory_instance_text)
    memory_instances_text = '\n'.join(memory_instance_texts)

    memory_class_texts = []
    sorted_type_definitions = sorted(type_definitions.values(), key=lambda x: x.name)
    for type_definition in sorted_type_definitions:
        dto_type_name = type_definition.get_cpp_dto_type_name_with_namespace()
        memory_type_name = type_definition.get_cpp_memory_type_name()
        memory_class_text = memory_class_template.substitute(name=memory_type_name, dto_type_name=dto_type_name)
        memory_class_texts.append(memory_class_text)
    memory_classes_text = '\n\n'.join(memory_class_texts)

    memories_text = memories_template.substitute(
        memory_classes=memory_classes_text, memory_instances=memory_instances_text)

    # Aggregate
    text = root_template.substitute(
        variables=variables_text,
        memories=memories_text,
        dtos=dtos_text,
        kernels=kernels_text
    )
    print(text)


def generate_cpp_file():
    definition = Definition()
    definition.load()
    type_definitions = definition.dto_types
    aggregated_text = create_aggregated_kernel_text(definition.kernels_directory_paths)
    function_prototypes = clcinspector.create_function_prototypes(aggregated_text)
    kernel_function_prototypes = list(filter(lambda f: f.is_kernel, function_prototypes))
    template = TemplateProvider('cpp/gpu/GPU.cpp')
    variables_text = ',\n'.join(map(lambda s: '    unsigned int {}'.format(s), definition.variables))

    dto_initialization_texts = []
    for dto_instance in definition.dto_instances:
        dto_type = type_definitions[dto_instance.type_name]
        if dto_instance.length is None:
            dto_initialization_text = template.of('dto-initialization').substitute(
                instance_name=dto_instance.name,
                type_name=dto_type.get_cpp_dto_type_name_with_namespace()
            )
        else:
            dto_initialization_text = template.of('dto-initialization-with-length').substitute(
                instance_name=dto_instance.name,
                type_name=dto_type.get_cpp_dto_type_name_with_namespace(),
                length=dto_instance.length
            )
        dto_initialization_texts.append(dto_initialization_text)
    dto_initializations_text = '\n'.join(dto_initialization_texts)

    kernel_initialization_texts = []
    for function_prototype in kernel_function_prototypes:
        kernel_initialization_texts.append(
            template.of('kernel-initialization').substitute(name=function_prototype.name))
    kernel_initializations_text = '\n'.join(kernel_initialization_texts)

    kernel_texts = []
    for function_prototype in kernel_function_prototypes:
        param_texts = []
        arg_texts = []
        for param in function_prototype.params:
            type_definition = type_definitions[param.param_type.name]
            if param.param_type.is_pointer:
                type_name = '{}&'.format(type_definition.get_cpp_memory_type_name_with_namespace())
                wrapper_name = 'memArg'
            else:
                type_name = type_definition.cpp_name
                wrapper_name = '{}Arg'.format(param.param_type.name)
            param_texts.append(template.of('kernel-parameter').substitute(
                type_name=type_name,
                name=param.name
            ))
            if param.param_type.is_pointer:
                arg_texts.append(template.of('kernel-assignment').substitute(
                    wrapper_name=wrapper_name,
                    name=param.name
                ))
            else:
                arg_texts.append(template.of('kernel-assignment-for-builtin-type').substitute(
                    wrapper_name=wrapper_name,
                    name=param.name
                ))
        params_text = ',\n'.join(param_texts)
        args_text = ',\n'.join(arg_texts)
        kernel_texts.append(template.of('kernel').substitute(
            name=function_prototype.name,
            parameters=params_text,
            assignments=args_text
        ))
    kernels_text = '\n\n'.join(kernel_texts)

    memory_texts = []
    sorted_type_definitions = sorted(type_definitions.values(), key=lambda x: x.name)
    for type_definition in sorted_type_definitions:
        dto_type_name = type_definition.get_cpp_dto_type_name_with_namespace()
        memory_type_name = type_definition.get_cpp_memory_type_name()
        memory_text = template.of('memory').substitute(
            memory_type_name=memory_type_name,
            dto_type_name=dto_type_name
        )
        memory_texts.append(memory_text)
    memories_text = '\n\n'.join(memory_texts)

    memory_initialization_texts = []
    memory_dto_assignment_texts = []
    memory_resources_provider_assignment_texts = []
    for dto_instance in definition.dto_instances:
        type_definition = type_definitions[dto_instance.type_name]
        dto_type_name = type_definition.get_cpp_dto_type_name_with_namespace()
        memory_dto_assignment_texts.append(template.of('memory-dto-assignment').substitute(
            name=dto_instance.name
        ))
        length = '1' if dto_instance.length is None else dto_instance.length
        if dto_instance.is_host_ptr:
            memory_initialization_texts.append(template.of('host-memory-initialization').substitute(
                name=dto_instance.name,
                type_name=dto_type_name,
                length=length
            ))
        else:
            memory_initialization_texts.append(template.of('gpu-memory-initialization').substitute(
                name=dto_instance.name,
                type_name=dto_type_name,
                length=length
            ))
        memory_resources_provider_assignment_texts.append(
            template.of('memory-resources-provider-assignment').substitute(
                name=dto_instance.name
            ))
    memory_initializations_text = '\n'.join(memory_initialization_texts)
    memory_dto_assignments_text = '\n'.join(memory_dto_assignment_texts)
    memory_resources_provider_assignments_text = '\n'.join(memory_resources_provider_assignment_texts)
    memories_initialization_text = template.of('memories-initialization').substitute(
        memory_initializations=memory_initializations_text,
        dto_assignments=memory_dto_assignments_text,
        resources_provider_assignments=memory_resources_provider_assignments_text
    )

    text = template.root.substitute(
        memories=memories_text,
        kernels=kernels_text,
        variables=variables_text,
        kernels_initialization=kernel_initializations_text,
        dtos_initialization=dto_initializations_text,
        memories_initialization=memories_initialization_text
    )
    print(text)


if __name__ == '__main__':
    pass
