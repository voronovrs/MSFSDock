import argparse
import re
import json
import shutil
import subprocess
from pathlib import Path
import zipfile

PLATFORM_ELGATO = "elgato"
PLATFORM_MIRABOX = "mirabox"
PLATFORMS = [PLATFORM_ELGATO, PLATFORM_MIRABOX]
VERSION_PATTERN = re.compile(r'^\d+\.\d+\.\d+\.\d+$')

ROOT = Path(__file__).resolve().parent
BIN_BUILD_PATH = ROOT / "out" / "build"
RESULT_PATH = ROOT / "build"
PLUGIN_BASE_PATH = ROOT / "com.rvoronov.msfsDock.sdPlugin"


def compile(cmake_preset: str) -> None:
    """Compile msfsDock exe file."""
    build_dir = BIN_BUILD_PATH / cmake_preset
    if build_dir.exists():
        shutil.rmtree(build_dir)

    subprocess.check_call(["cmake", "--preset", cmake_preset])
    subprocess.check_call(["cmake", "--build", "--preset", cmake_preset])

    return build_dir


def copy_plugin(path: Path) -> Path:
    """Copy plugin base folder."""
    dest = path / PLUGIN_BASE_PATH.name
    if dest.exists():
        shutil.rmtree(dest)
    shutil.copytree(PLUGIN_BASE_PATH, dest)
    return dest


def copy_binary(bin_path: Path, plugin_path: Path) -> None:
    """Copy compiled binary file."""
    shutil.copy(bin_path / "msfsDock.exe", plugin_path)


def update_manifest(path: Path, platform: str, version: str | None = None) -> None:
    """Update manifest.json depending on a platform."""
    manifest_path = path / "manifest.json"
    with open(manifest_path, "r") as f:
        data = json.load(f)

    if version is not None:
        data["Version"] = version

    if platform == PLATFORM_ELGATO:
        for action_data in data["Actions"]:
            for i, x in enumerate(action_data.get("Controllers", [])):
                if x == "Knob":
                    action_data["Controllers"][i] = "Encoder"

    with open(manifest_path, "w") as f:
        json.dump(data, f, indent=2)


def deploy_plugin(path: Path, platform: str) -> None:
    """Create plugin archive with correct format for a platform."""
    new_path = path.parent / f"{path.name}{'.streamDeckPlugin' if platform == PLATFORM_ELGATO else '.zip'}"
    with zipfile.ZipFile(new_path, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for file in path.rglob('*'):
            if file.is_file():
                zipf.write(file, arcname=file.relative_to(path.parent))


def build(preset: str, platform: str, version: str | None = None, deploy: bool = False):
    """Build and deploy."""

    print(f"Building {preset}...")
    out_path = RESULT_PATH / preset

    compiled_path = compile(preset)

    plugin_path = copy_plugin(out_path)

    copy_binary(compiled_path, plugin_path)

    update_manifest(plugin_path, platform, version)

    if deploy:
        deploy_plugin(plugin_path, platform)

    print(f"{preset}: Build done. Build files: {plugin_path.parent}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-p", "--platform", choices=[*PLATFORMS, "all"], default="mirabox", help="Set platform to build")
    parser.add_argument("-t", "--target", choices=["debug", "release"], default="release", help="Set debug or release target")
    parser.add_argument("-d", "--deploy", action="store_true", help="Create archive for deploy")
    parser.add_argument("-v", "--version", default=None, help="Version in forman major.minor.revision.build")
    args = parser.parse_args()

    version = None
    if args.version is not None and VERSION_PATTERN.match(args.version):
        version = VERSION_PATTERN.match(args.version).group(0)

    shutil.rmtree(RESULT_PATH, ignore_errors=True)

    if args.platform == "all":
        for platform in PLATFORMS:
            preset = f"x64-{args.target.capitalize()}-{platform.capitalize()}"
            build(preset, platform, version, args.deploy)
    else:
        preset = f"x64-{args.target.capitalize()}-{args.platform.capitalize()}"
        build(preset, args.platform, version, args.deploy)
