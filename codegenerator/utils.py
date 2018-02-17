from string import Template
from os import path


def load_text(text_path):
    text = ''
    with open(text_path, encoding='utf-8') as file:
        for line in file:
            text += line
    return text


def get_template_dir_path():
    source_dir_path = path.dirname(path.abspath(__file__))
    return '{}/src/templates'.format(source_dir_path[:-len('/codegenerator')])


template_dir_path = get_template_dir_path()


def load_template(template_path):
    return Template(load_text('{}/{}'.format(template_dir_path, template_path)))
