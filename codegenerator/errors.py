class InvalidFieldFormatDefinition(Exception):
    def __init__(self, type_name, specified_text):
        self.type_name = type_name
        self.specified_text = specified_text


class UndefinedTypeFieldDefinitions(Exception):
    """
    :type field_definitions: list[FieldDefinition]
    """
    def __init__(self, field_definitions):
        self.field_definitions = field_definitions


class InputNotSpecified(Exception):
    pass


class UnsupportedFormat(Exception):
    def __init__(self, specified_format):
        self.format = specified_format


class FormatNotSpecified(Exception):
    def __init__(self, parser):
        self.parser = parser


class UnknownTarget(Exception):
    def __init__(self, specified_target):
        self.target = specified_target
