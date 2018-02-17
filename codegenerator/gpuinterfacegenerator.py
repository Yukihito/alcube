from .utils import *


def generate_cpp_header_file():
    root_template = load_template('cpp/gpu/GPU.h.template')
    dto_template = load_template('cpp/gpu/GPU.h-dto.template')
    dtos_template = load_template('cpp/gpu/GPU.h-dtos.template')
    kernel_function_template = load_template('cpp/gpu/GPU.h-kernel-function.template')
    kernels_template = load_template('cpp/gpu/GPU.h-kernels.template')
    memories_template = load_template('cpp/gpu/GPU.h-memories.template')
    memory_class_template = load_template('cpp/gpu/GPU.h-memory-class.template')
    memory_instance_template = load_template('cpp/gpu/GPU.h-memory-instance.template')
    raw_kernel_template = load_template('cpp/gpu/GPU.h-raw-kernel.template')


def generate_cpp_file():
    pass


if __name__ == '__main__':
    pass
