import sys
import argparse
from . import errors
from . import dtosgenerator
from . import simulatorbasegenerator


def route():
    parser = argparse.ArgumentParser()
    parser.add_argument('target', help='Build target. e.g. dto')
    parser.add_argument(
        '-f', '--format',
        type=str,
        help='File format to generate. e.g. cpp, clc'
    )
    args = parser.parse_args()
    if args.target == 'dto':
        if args.format == 'cpp':
            dtosgenerator.generate(dtosgenerator.create_cpp_text)
        elif args.format == 'clc':
            dtosgenerator.generate(dtosgenerator.create_clc_text)
        elif args.format is None:
            raise errors.FormatNotSpecified(parser)
        else:
            raise errors.UnsupportedFormat(args.format)
    elif args.target == 'prototypes':
        simulatorbasegenerator.generate_clc_function_prototypes()
    else:
        raise errors.UnknownTarget(args.target)


def handle_error(f):
    try:
        f()
    except errors.InvalidFieldFormatDefinition as e:
        print('Invalid field format in definition of {}: {}'.format(
            e.type_name, e.specified_text), file=sys.stderr)
        exit(1)
    except errors.UndefinedTypeFieldDefinitions as e:
        for field_definition in e.field_definitions:
            print('Undefined type used in definition of {}#{}: {}'.format(
                field_definition.owner.name, field_definition.name, field_definition.type_name), file=sys.stderr)
        exit(1)
    except errors.InputNotSpecified:
        print('Input not specified', file=sys.stderr)
        exit(1)
    except errors.UnsupportedFormat as e:
        print('Unsupported format: {} ("cpp" or "clc" required.)'.format(e.format), file=sys.stderr)
        exit(1)
    except errors.FormatNotSpecified as e:
        print('--format or -f required', file=sys.stderr)
        e.parser.print_help()
        exit(1)
    except errors.UnknownTarget as e:
        print('Unknown build target: {}'.format(e.target))
        exit(1)


def _main():
    handle_error(route)


if __name__ == '__main__':
    _main()
