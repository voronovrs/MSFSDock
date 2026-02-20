# This module is for extracting events and variables data from PMDG SDK.
import re
import shutil
import subprocess
import time
import tempfile

from pathlib import Path

PMDG_HEADER_NAME = "PMDG_NG3_SDK.h"
PMDG_DATA_STRUCT_NAME = "PMDG_NG3_Data"

evt_ptrn = re.compile(r"^#define\s(EVT_[\w]+).*")
field_pattern = re.compile(r'^\s*(?P<type>.+?)\s+(?P<name>[A-Za-z_][A-Za-z0-9_]*)(?:\s*\[\s*(?P<size>\d+)\s*\])?\s*;')


def generate_events_inc(sdk_path: Path, out_path: Path) -> None:
    """Extract events from PMDG SDK and create .inc file for plugin."""
    events = []
    with open(sdk_path, "r") as fh:
        for line in fh:
            if evt_ptrn.match(line):
                events.append(evt_ptrn.match(line).group(1))

    with open(out_path, "w") as fh:
        for event in events:
            fh.write(f"X({event})\n")


def extract_variables(sdk_path: Path) -> list:
    """Extract variables from PMDG SDK dara structure for futher processing."""
    variables = []
    sdk_lines = []
    with open(sdk_path, "r") as fh:
        sdk_lines = fh.readlines()

    inside_struct = False

    for line in sdk_lines:
        if f"struct {PMDG_DATA_STRUCT_NAME}" in line:
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


def generate_offset_dumper(fields: list, header_name, out_c: Path):
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
                f'        size_t off = offsetof({PMDG_DATA_STRUCT_NAME}, {name}) + '
                f'i * sizeof((({PMDG_DATA_STRUCT_NAME}*)0)->{name}[0]);\n'
                f'        printf("{ctype}\t{name}:%d\t%zu\\n", i, off);\n'
                '    }\n'
            )

        else:
            lines.append(
                f'    printf("{ctype}\t{name}\t%zu\\n", '
                f'offsetof({PMDG_DATA_STRUCT_NAME}, {name}));\n'
            )

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
    workdir = Path().cwd()
    sdk_path = workdir / PMDG_HEADER_NAME
    variablres_file_path = workdir / "pmdg_variables.inc"
    events_file_path = workdir / "pmdg_events.inc"

    if not sdk_path.is_file():
        raise FileNotFoundError(f"No {PMDG_HEADER_NAME} file found in {workdir}")

    generate_events_inc(sdk_path, events_file_path)

    offsets_dump = []
    vars = extract_variables(sdk_path)

    with tempfile.TemporaryDirectory(dir=workdir) as tmp_dir_name:
        tmp_path = Path(tmp_dir_name)

        shutil.copy(sdk_path, tmp_path)
        offset_dumper_c = tmp_path / "pmdg_offset_dump.cpp"

        generate_offset_dumper(vars, PMDG_HEADER_NAME, offset_dumper_c)

        offsets_dump = compile_and_run(offset_dumper_c)
        time.sleep(1)

    generate_variables_inc(offsets_dump, variablres_file_path)
