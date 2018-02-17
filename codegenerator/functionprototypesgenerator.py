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


def generate_clc_function_prototypes():
    definition = Definition()
    definition.load()
    aggregated_text = create_aggregated_kernel_text(definition.kernels_directory_paths)
    function_prototypes = clcinspector.create_function_prototypes(aggregated_text)
    function_prototypes_text = clcinspector.create_function_prototypes_text(
        filter(lambda p: not p.is_kernel, function_prototypes))
    print(function_prototypes_text)
