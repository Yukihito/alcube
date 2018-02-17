from .utils import *
from . import clcinspector
from .definitionloader import Definition
import os.path
from glob import glob


def create_aggregated_kernel_text(kernels_directory_paths):
    aggregated_kernel = ''
    for kernels_directory_path in kernels_directory_paths:
        file_names = sorted([
            os.path.relpath(x, kernels_directory_path)
            for x in glob(os.path.join(kernels_directory_path, '*.cl'))
        ])
        for file_name in file_names:
            aggregated_kernel += load_text(os.path.join(kernels_directory_path, file_name)) + '\n'
    return aggregated_kernel


def generate():
    # definition = yaml.load(sys.stdin.read())
    # variables = definition['variables']
    # aggregated_text = create_aggregated_kernel_text(definition)
    # function_prototypes = clcinspector.create_function_prototypes(aggregated_text)
    # print('\n'.join(map(str, function_prototypes)))
    # print('\n'.join(clcinspector.collect_struct_names(function_prototypes)))
    # function_prototypes_text = clcinspector.create_function_prototypes_text(
    # filter(lambda p: not p.is_kernel, function_prototypes))
    # print(function_prototypes_text)
    pass


def generate_cpp_header_file():
    pass


def generate_clc_function_prototypes():
    definition = Definition()
    definition.load()
    aggregated_text = create_aggregated_kernel_text(definition.kernels_directory_paths)
    function_prototypes = clcinspector.create_function_prototypes(aggregated_text)
    function_prototypes_text = clcinspector.create_function_prototypes_text(filter(lambda p: not p.is_kernel, function_prototypes))
    print(function_prototypes_text)


if __name__ == '__main__':
    # generate()
    # generate_function_prototypes()
    pass
