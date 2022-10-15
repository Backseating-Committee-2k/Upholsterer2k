from dataclasses import dataclass
import sys
import json

def argument_as_c_enum(argument):
    if "Register" in argument:
        return "ARGUMENT_TYPE_REGISTER" if argument["Register"][2] != "pointer" else "ARGUMENT_TYPE_REGISTER_POINTER"
    argument_mappings = {
        "Immediate": "ARGUMENT_TYPE_IMMEDIATE",
        "Address": "ARGUMENT_TYPE_ADDRESS_POINTER",
    }
    return argument_mappings[argument]

def argument_to_usage(opcode_type, argument):
    if "Register" in argument:
        return "ARGUMENT_USAGE_SOURCE" if argument["Register"][0] == "Source" else "ARGUMENT_USAGE_TARGET"
    argument_mappings = {
        "Immediate": "ARGUMENT_USAGE_SOURCE",
        "Address": "ARGUMENT_USAGE_SOURCE" if opcode_type == "source_address" else "ARGUMENT_USAGE_TARGET",
    }
    return argument_mappings[argument]

@dataclass
class Argument:
    arg_type: str
    usage: str
    offset: int

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
            data["opcodes"] = dict(sorted(data["opcodes"].items(), key=lambda entry: entry[1]["opcode"]))
            num_opcodes = len(data["opcodes"])
            with open(output_filename, 'w') as out_file:
                out_file.write("""// auto-generated with opcodes2c.py

#include "upholsterer2k/opcodes.h"
#include "upholsterer2k/string_view.h"

OpcodeList opcode_specifications(void) {\n""")
                out_file.write(f"    size_t const num_opcodes = {num_opcodes};\n")
                out_file.write("    OpcodeSpecification* specifications = malloc(num_opcodes * sizeof(*specifications));\n")
                for i, opcode in enumerate(data["opcodes"]):
                    argument_count = len(data["opcodes"][opcode]["arguments"])
                    arguments = list()
                    offset_accumulator = 40
                    for j in range(argument_count):
                        argument = data["opcodes"][opcode]["arguments"][j] # Register/Immediate/Address
                        offset = offset_accumulator if "Register" in argument else 0
                        if "Register" in argument:
                            offset_accumulator -= 8
                        arguments.append(Argument(argument_as_c_enum(argument), argument_to_usage(data["opcodes"][opcode]["opcode_type"], argument), offset))
                    arguments = sorted(arguments, key=lambda arg: 0 if arg.usage == "ARGUMENT_USAGE_SOURCE" else 1)
                    out_file.write(f"    specifications[{i}] = (OpcodeSpecification){{\n")
                    out_file.write(f"        .name = string_view_from_string(\"{opcode}\"),\n")
                    out_file.write(f"        .mnemonic = opcode_to_mnemonic(string_view_from_string(\"{opcode}\")),\n")
                    out_file.write(f"        .argument_count = {argument_count},\n")
                    out_file.write(f"        .required_arguments = {{ ")
                    out_file.write("ARGUMENT_TYPE_NONE" if len(arguments) == 0 else ', '.join(x.arg_type for x in arguments))
                    out_file.write(f" }},\n")
                    out_file.write(f"        .offsets = {{ ")
                    out_file.write("0" if len(arguments) == 0 else ', '.join(str(x.offset) for x in arguments))
                    out_file.write(f" }},\n")
                    opcode = data["opcodes"][opcode]["opcode"]
                    out_file.write(f"        .opcode = {hex(opcode)},\n")
                    out_file.write(f"    }};\n")
                out_file.write("""    return (OpcodeList){
    .specifications = specifications,
    .num_specifications = num_opcodes,
};
}
""")
        except KeyError:
            sys.stderr.write("Error reading JSON file: An expected key could not be found.\n")
    except IOError:
        sys.stderr.write(f"IO error. Make sure the input file exists and can be opened.\n")


if __name__ == "__main__":
    main()
