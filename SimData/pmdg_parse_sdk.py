import os
import re

fpath = os.path.join(os.getcwd(), "PMDG_NG3_SDK.h")
opath = os.path.join(os.getcwd(), "pmdg_sdk.cpp")

evt_ptrn = re.compile(r"^#define\s(EVT_[\w]+).*")

res_events = []

with open(fpath, "r") as fh:
    for line in fh:
        if evt_ptrn.match(line):
            res_events.append(evt_ptrn.match(line).group(1))

print(res_events)

with open(opath, "w") as fh:
    fh.write("#include \"pmdg_sdk.hpp\"\n")
    fh.write("#include \"PMDG_NG3_SDK.h\"\n\n")

    fh.write("static const std::vector<PmdgEvent> PMDG_EVENTS = {\n")

    for evt in res_events:
        fh.write(f"\t{{ \"{evt}\", {evt} }},\n")

    fh.write("};\n\n")

    fh.write("const std::vector<PmdgEvent>& GetPmdgEvents() {\n")
    fh.write("\treturn PMDG_EVENTS;\n")
    fh.write("}\n")
