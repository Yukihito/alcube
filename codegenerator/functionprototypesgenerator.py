from .utils import *
from . import clcinspector
from .definitionloader import Definition


def generate_clc_function_prototypes():
    definition = Definition()
    definition.load()
    aggregated_text = create_aggregated_kernel_text(definition.kernels_directory_paths)
    function_prototypes = clcinspector.create_function_prototypes(aggregated_text)
    function_prototypes_text = clcinspector.create_function_prototypes_text(
        filter(lambda p: not p.is_kernel, function_prototypes))
    print(function_prototypes_text)
