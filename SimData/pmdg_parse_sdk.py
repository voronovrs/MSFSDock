# This module is for extracting events and variables data from PMDG SDK.
import argparse
import re
import shutil
import subprocess
import time
import tempfile

from pathlib import Path

SDK_DATA = {"737": {"PMDG_HEADER_NAME": "PMDG_NG3_SDK.h", "PMDG_DATA_STRUCT_NAME": "PMDG_NG3_Data"},
            "777" : {"PMDG_HEADER_NAME": "PMDG_777X_SDK.h", "PMDG_DATA_STRUCT_NAME": "PMDG_777X_Data"}
            }


data_id_ptrn = re.compile(r"^(#define)\s+(\w+_DATA_(NAME|ID|DEFINITION))\s+([\w\"]+)")
mouse_ptrn = re.compile(r"^(#define)\s+(MOUSE_FLAG_\w+)\s+([\w\"]+)")
evt_id_min_ptrn = re.compile(r"^(#define)\s(THIRD_PARTY_EVENT_ID_MIN)\s+([0-9a-fA-FxX]+)")
evt_ptrn = re.compile(r"^#define\s(EVT_[\w]+).*\(THIRD_PARTY_EVENT_ID_MIN \+ (\d+)\)")
field_pattern = re.compile(r'^\s*(?P<type>.+?)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)(?:\s*\[\s*(?P<size>\d+)\s*\])?\s*;')


def parse_data_id(sdk_path: Path) -> list:
    """Extract data IDs from SDK for plugin."""
    data_ids = []
    with open(sdk_path, "r") as fh:
        for line in fh:
            if data_id_ptrn.match(line):
                define = data_id_ptrn.match(line).group(1)
                name = data_id_ptrn.match(line).group(2)
                val = data_id_ptrn.match(line).group(4)
                data_ids.append(f"{define} {name}\t\t\t{val}")
            if evt_id_min_ptrn.match(line):
                define = evt_id_min_ptrn.match(line).group(1)
                name = evt_id_min_ptrn.match(line).group(2)
                val = evt_id_min_ptrn.match(line).group(3)
                data_ids.append(f"{define} {name}\t\t\t{val}")
            if mouse_ptrn.match(line):
                define = mouse_ptrn.match(line).group(1)
                name = mouse_ptrn.match(line).group(2)
                val = mouse_ptrn.match(line).group(3)
                data_ids.append(f"{define} {name}\t\t\t{val}")

    return data_ids


def save_data_id_header(content: list, out_path: Path) -> None:
    """Save data header."""
    with open(out_path, "w") as fh:
        for line in content:
            fh.write(f"{line}\n")


def generate_events_inc(sdk_path: Path, out_path: Path) -> None:
    """Extract events from PMDG SDK and create .inc file for plugin."""
    events = []
    evt_min_id = None
    with open(sdk_path, "r") as fh:
        for line in fh:
            if evt_id_min_ptrn.match(line):
                evt_min_id = int(evt_id_min_ptrn.match(line).group(3), 0)

            if evt_ptrn.match(line):
                if evt_min_id is None:
                    print(f"evt_min_id is not known for line '{line}'")
                    continue
                events.append((evt_ptrn.match(line).group(1), evt_min_id + int(evt_ptrn.match(line).group(2), 0)))

    with open(out_path, "w") as fh:
        for event in events:
            fh.write(f"X({event[0]}, {event[1]})\n")


def extract_variables(sdk_path: Path, data_struct_name: str) -> list:
    """Extract variables from PMDG SDK dara structure for futher processing."""
    variables = []
    sdk_lines = []
    with open(sdk_path, "r") as fh:
        sdk_lines = fh.readlines()

    inside_struct = False

    for line in sdk_lines:
        if f"struct {data_struct_name}" in line:
            inside_struct = True
            continue

        if inside_struct and "};" in line:
            break

        if not inside_struct:
            continue

        m = field_pattern.match(line)
        if m:
            ctype = m.group("type").strip()
            name = m.group("name")
            size = m.group("size")

            if "reserved" in name.lower():
                continue

            variables.append((ctype, name, None if size is None else int(size)))

    return variables


def generate_offset_dumper(fields: list, header_name: str, out_c: Path, data_struct_name: str):
    """Generate C++ code to dump data register fields offsets."""

    lines = []

    lines.append('#include <stdio.h>\n')
    lines.append('#include <stddef.h>\n')
    lines.append(f'#include "{header_name}"\n\n')

    lines.append("int main()\n{\n")

    for ctype, name, arr_size in fields:

        if arr_size:
            lines.append(
                f'    for (int i = 0; i < {arr_size}; ++i)\n'
                '    {\n'
                f'        size_t off = offsetof({data_struct_name}, {name}) + '
                f'i * sizeof((({data_struct_name}*)0)->{name}[0]);\n'
                f'        printf("{ctype}\t{name}:%d\t%zu\\n", i, off);\n'
                '    }\n'
            )

        else:
            lines.append(
                f'    printf("{ctype}\t{name}\t%zu\\n", '
                f'offsetof({data_struct_name}, {name}));\n'
            )

    lines.append(f'    printf("STRUCT_SIZE\\t%zu\\n", sizeof({data_struct_name}));\n')
    lines.append("    return 0;\n}\n")

    out_c.write_text("".join(lines))
    print("Generated", out_c)


def compile_and_run(c_file: Path) -> list:
    """Compile code, run and extract output."""
    workdir = c_file.parent
    subprocess.run(["cl", str(c_file)], cwd=workdir, check=True)
    exe_file = c_file.parent / c_file.name.replace(".cpp", ".exe")
    print(f"EXE {exe_file}")
    result = subprocess.run(
        [str(exe_file)],
        capture_output=True,
        text=True,
        cwd=workdir,
        check=True
    )
    return result.stdout.splitlines()


def generate_variables_inc(variables: list, out_path: Path) -> None:
    """Generate variables .inc file for plugin."""
    with open(out_path, "w") as fh:
        for line in variables:
            ctype, name, offset = line.split("\t")
            fh.write(f"X({ctype}, {name}, {offset})\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Extract data from PMDG SDK");
    parser.add_argument("-t", "--type", choices=("737", "777", "all"), help="Aircraft type", default="all")
    args = parser.parse_args()

    sdks = ("737", "777") if args.type == "all" else (args.type,)
    workdir = Path().cwd()
    data_header_lines = ["#pragma once"]
    data_header_file = workdir / "pmdg_data.h"

    for sdk in sdks:
        header_name = SDK_DATA[sdk]["PMDG_HEADER_NAME"]
        data_struct_name = SDK_DATA[sdk]["PMDG_DATA_STRUCT_NAME"]

        print(f"Processing SDK for {sdk}")
        print(f"\tHeader file name: {header_name}")
        print(f"\tData structure name: {data_struct_name}")

        sdk_path = workdir / header_name
        variablres_file_path = workdir / f"pmdg_variables_{sdk}.inc"
        events_file_path = workdir / f"pmdg_events_{sdk}.inc"

        if not sdk_path.is_file():
            raise FileNotFoundError(f"No {header_name} file found in {workdir}")

        data_header_lines.extend(parse_data_id(sdk_path))

        generate_events_inc(sdk_path, events_file_path)

        offsets_dump = []
        vars = extract_variables(sdk_path, data_struct_name)

        with tempfile.TemporaryDirectory(dir=workdir) as tmp_dir_name:
            tmp_path = Path(tmp_dir_name)

            shutil.copy(sdk_path, tmp_path)
            offset_dumper_c = tmp_path / "pmdg_offset_dump.cpp"

            generate_offset_dumper(vars, header_name, offset_dumper_c, data_struct_name)

            offsets_dump = compile_and_run(offset_dumper_c)
            time.sleep(1)
            # _, struct_size = int(offsets_dump.pop(-1).split("\t"))
            struct_size = offsets_dump.pop(-1).split()[1]
            data_header_lines.append(f"#define {data_struct_name.upper()}_SIZE\t\t\t{struct_size}")

        generate_variables_inc(offsets_dump, variablres_file_path)
        print("Done")

    data_header_lines = list(dict.fromkeys(data_header_lines))
    if (data_header_lines):
        save_data_id_header(data_header_lines, data_header_file)
