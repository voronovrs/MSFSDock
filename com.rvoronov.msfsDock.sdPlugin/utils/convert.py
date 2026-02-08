# This module converts streamdoc profile (SDProfile) files to new(0.8.0.0+) UUID naming.
import argparse
import tempfile
import zipfile

from pathlib import Path

pre08_uuid = "com.rvoronov.msfsDock"
post08_uuid = "com.rvoronov.msfsdock"


def _parse_manifest(path: Path) -> bool:
    converted = False
    for manifest_file in path.rglob('manifest.json'):
        changed = False
        manifest_content = []
        with open(manifest_file, "r") as mfh:
            manifest_content = mfh.readlines()

        for i, line in enumerate(manifest_content):
            if pre08_uuid in line:
                changed = True
                new_line = line.replace(pre08_uuid, post08_uuid)
                manifest_content[i] = new_line

        if changed:
            converted = True
            with open(manifest_file, "w") as mfh:
                mfh.writelines(manifest_content)

    return converted


def convert(path: Path) -> None:
    if not zipfile.is_zipfile(path):
        print(f"File {path} is not a valid profile")
        return

    print(f"{path} - start conversion")

    new_path = path.parent / path.name.replace(".SDProfile", "_new.SDProfile")

    with tempfile.TemporaryDirectory(dir=path.parent) as tmp_dir_name:
        tmp_path = Path(tmp_dir_name)

        with zipfile.ZipFile(path) as zip_fh:
            zip_fh.extractall(tmp_path)

        if not _parse_manifest(tmp_path):
            print(f"{path} - no conversion needed.")
            return

        with zipfile.ZipFile(new_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
            for file in tmp_path.rglob('*'):
                if file.is_file():
                    zipf.write(file, arcname=file.relative_to(tmp_path))

    print(f"{new_path} - converted")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("filepath", help="Set path to SDProfile file")
    args = parser.parse_args()
    path = Path(args.filepath).absolute()

    convert(path)
