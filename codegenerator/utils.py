from string import Template


def load_text(text_path):
    text = ''
    with open(text_path, encoding='utf-8') as file:
        for line in file:
            text += line
    return text


def load_template(template_path):
    return Template(load_text(template_path))
