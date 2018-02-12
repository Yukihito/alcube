from string import Template


def load_template(template_path):
    text = ''
    with open(template_path, encoding='utf-8') as file:
        for line in file:
            text += line
    return Template(text)
