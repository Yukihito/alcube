import re


class TypeWithModifiers:
    def __init__(self, tokens, is_pointer):
        reserved_modifiers = {'__global', 'const'}
        names = []
        self.name = ''
        self.modifiers = []
        self.is_pointer = is_pointer
        for token in tokens:
            if token in reserved_modifiers:
                self.modifiers.append(token)
            else:
                names.append(token)
        self.name = ' '.join(names)

    def __str__(self):
        result = 'type name "{}", modifiers {}'.format(self.name, self.modifiers)
        if self.is_pointer:
            result += ' (pointer)'
        return result

    def create_clc_text(self):
        modifiers_text = ' '.join(self.modifiers)
        name_text = '{}*'.format(self.name) if self.is_pointer else self.name
        return '{} {}'.format(modifiers_text, name_text) if len(self.modifiers) > 0 else name_text


class ParamPrototype:
    def __init__(self, param_type: TypeWithModifiers, name):
        self.param_type = param_type
        self.name = name

    def __str__(self):
        return '{}: {}'.format(self.name, str(self.param_type))

    def create_clc_text(self):
        return '{} {}'.format(self.param_type.create_clc_text(), self.name)


class FunctionPrototype:
    """
    :type params: list[ParamPrototype]
    """
    def __init__(self, is_kernel, is_pointer_type, type_tokens, name, params):
        self.is_kernel = is_kernel
        self.rtype = TypeWithModifiers(type_tokens, is_pointer_type)
        self.name = name
        self.params = params

    def __str__(self):
        name = self.name
        if self.is_kernel:
            name = '(kernel) {}'.format(name)
        return '{}: {}\n\t{}'.format(name, str(self.rtype), '\n\t'.join(map(str, self.params)))

    def create_clc_text(self):
        params_text = '({})'.format(', '.join(map(lambda p: p.create_clc_text(), self.params)))
        rtype_text = '__kernel {}'.format(
            self.rtype.create_clc_text()) if self.is_kernel else self.rtype.create_clc_text()
        return '{} {}{};'.format(rtype_text, self.name, params_text)


def trim_comment(text):
    text = re.sub(r'//[^\n]*', '', text)
    text = re.sub(r'/\*([^*]|(\*[^/]))*\*/', '', text)
    return text


def trim_define_macro(text):
    return re.sub(r'#define[^\n]*', '', text)


def create_function_prototypes(text):
    text_without_comment = trim_define_macro(trim_comment(text))
    function_name_regex = r'([\w\*][\w\*\s]*)'
    params_regex = r'([^\)]*)'
    function_regex = r'\s*' + function_name_regex + '\(' + params_regex + '\)\s*\{'
    raw_function_texts = re.findall(function_regex, text_without_comment)
    function_prototypes = []
    for raw_function_text in raw_function_texts:
        reserved_keyword = re.match(r'^\s*(for|if|while|switch|do)(\s|\()*', raw_function_text[0])
        if reserved_keyword is None:
            function_name_text = raw_function_text[0]
            function_name_tokens = re.findall(r'[\w]+', function_name_text)
            param_texts = raw_function_text[1].split(',')
            params = []
            for param_text in param_texts:
                param_tokens = re.findall(r'[\w]+', param_text)
                is_pointer_param = '*' in param_text
                param_type = TypeWithModifiers(param_tokens[:-1], is_pointer_param)
                param_name = param_tokens[-1]
                params.append(ParamPrototype(param_type, param_name))

            is_kernel = function_name_tokens[0] == '__kernel'
            is_pointer_rtype = '*' in function_name_text
            rtype_tokens = function_name_tokens[:-1] if not is_kernel else function_name_tokens[1:-1]
            function_name = function_name_tokens[-1]
            fun = FunctionPrototype(
                is_kernel,
                is_pointer_rtype,
                rtype_tokens,
                function_name,
                params
            )
            function_prototypes.append(fun)
    return function_prototypes


# def create_clc_primitive_type_names():
#     number_type_names = ['char', 'short', 'int', 'long', 'float', 'double']
#     primitive_type_names = {'void'}
#     for number_type_name in number_type_names:
#         scalar_type_names = [
#             number_type_name,
#             'u{}'.format(number_type_name),
#             'unsigned {}'.format(number_type_name)
#         ]
#         for scalar_type_name in scalar_type_names:
#             primitive_type_names.add(scalar_type_name)
#             for i in range(2, 10):
#                 primitive_type_names.add(scalar_type_name + str(i))
#     return primitive_type_names
#
#
# def collect_struct_names(function_prototypes):
#     """
#     :type function_prototypes: list[FunctionPrototype]
#     :return: list[name]
#     """
#     primitive_type_names = create_clc_primitive_type_names()
#     struct_type_names = set()
#     for function_prototype in function_prototypes:
#         if function_prototype.rtype.name not in primitive_type_names:
#             struct_type_names.add(function_prototype.rtype.name)
#         for param in function_prototype.params:
#             if param.param_type.name not in primitive_type_names:
#                 struct_type_names.add(param.param_type.name)
#     return sorted(list(struct_type_names))


def create_function_prototypes_text(function_prototypes):
    return '\n\n'.join(map(lambda p: p.create_clc_text(), function_prototypes))
