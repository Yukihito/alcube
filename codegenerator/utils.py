from string import Template
import os.path
from glob import glob


def load_text(text_path):
    text = ''
    with open(text_path, encoding='utf-8') as file:
        for line in file:
            text += line
    return text


def get_template_dir_path():
    source_dir_path = os.path.dirname(os.path.abspath(__file__))
    return '{}/src/templates'.format(source_dir_path[:-len('/codegenerator')])


template_dir_path = get_template_dir_path()


def load_template(template_path):
    return Template(load_text('{}/{}'.format(template_dir_path, template_path)))


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


input_file_name = None
