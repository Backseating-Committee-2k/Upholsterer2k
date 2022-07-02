from dataclasses import dataclass
import sys
import json

def argument_to_constant_type(arg):
    type_dict = {
        "Address" : "CONSTANT_TYPE_ADDRESS",
        "Register" : "CONSTANT_TYPE_REGISTER",
        "UnsignedInteger" : "CONSTANT_TYPE_UNSIGNED_INTEGER"
    }
    return type_dict[arg]

def main():
    if len(sys.argv) != 3:
        sys.stderr.write(f"Usage: {sys.argv[0]} <input file> <output file>\n")
        exit(1)

    input_filename = sys.argv[1]
    output_filename = sys.argv[2]

    try:
        with open(input_filename, 'r') as in_file:
            data = json.load(in_file)
        try:
            data["constants"] = dict(sorted(data["constants"].items()))
            num_constants = len(data["opcodes"])
            with open(output_filename, 'w') as out_file:
                out_file.write("// this file has been auto-generated by json2constants.py\n")
                out_file.write(r"""#include "constants.h"

Constant constants[] = {
""")
                for name, constant in data["constants"].items():
                    (constant_type, value), = constant.items()
                    out_file.write("    {\n")
                    out_file.write(f"       .name = \"{name}\",\n")
                    out_file.write(f"       .type = {argument_to_constant_type(constant_type)},\n")
                    out_file.write(f"       .value = {value}\n")
                    out_file.write("    },\n")
                out_file.write("};\n\n")
                out_file.write("size_t num_constants = sizeof(constants) / sizeof(constants[0]);\n")
        except KeyError:
            sys.stderr.write("Error reading JSON file: An expected key could not be found.\n")
    except IOError:
        sys.stderr.write(f"IO error. Make sure the input file exists and can be opened.\n")


if __name__ == "__main__":
    main()
