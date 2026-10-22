"""Build automation for Nexora Engine.

Actions run in the order they are given, the first failure stops the script:
    python Automation/automation.py clean rebuild test --config Debug
    python Automation/automation.py rebuild
    python Automation/automation.py format
"""

import argparse
import platform
import shutil
import subprocess
import sys
from pathlib import Path

##################### manual configuration ####################

ROOT_DIR = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT_DIR / "Build"
CONAN_DIR = BUILD_DIR / "conan"
SOURCE_DIR = ROOT_DIR / "Engine"
CONAN_PROFILES_DIR = ROOT_DIR / "Automation" / "ConanProfiles"
CMAKE_GENERATOR = "Visual Studio 18 2026"  # https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
CMAKE_PLATFORM = "x64"
TEST_RUNNER = "NexoraEngineTestRunner"
FORMAT_EXTENSIONS = (".h", ".hpp", ".cpp")
CONFIGURATIONS = ("Debug", "Release")

###############################################################


def run(command, cwd=ROOT_DIR):
    print(f"> {subprocess.list2cmdline(command)}", flush=True)
    if subprocess.run(command, cwd=cwd).returncode != 0:
        sys.exit(f"Command failed: {command[0]}")


def clean(config):
    if BUILD_DIR.exists():
        shutil.rmtree(BUILD_DIR)
        print(f"Removed {BUILD_DIR}")
    else:
        print(f"{BUILD_DIR} does not exist, nothing to clean.")


def generate(config):
    profile = CONAN_PROFILES_DIR / f"{platform.system().lower()}{config}"
    run(["conan", "install", ".", "--output-folder", str(CONAN_DIR), "--build=missing", f"--profile:all={profile}"])
    run(["cmake", "--fresh", "-S", ".", "-B", str(BUILD_DIR), "-G", CMAKE_GENERATOR, "-A", CMAKE_PLATFORM,
         f"-DCMAKE_TOOLCHAIN_FILE={CONAN_DIR / 'conan_toolchain.cmake'}"])


def build(config):
    run(["cmake", "--build", str(BUILD_DIR), "--config", config])


def rebuild(config):
    generate(config)
    run(["cmake", "--build", str(BUILD_DIR), "--config", config, "--clean-first"])


def test(config):
    bin_dir = BUILD_DIR / "bin" / config
    runner = bin_dir / (TEST_RUNNER + (".exe" if platform.system() == "Windows" else ""))
    if not runner.exists():
        sys.exit(f"Test runner not found: {runner}. Build the project first.")
    run([str(runner)], cwd=bin_dir)


def format_code(config):
    files = [str(path) for path in SOURCE_DIR.rglob("*") if path.suffix in FORMAT_EXTENSIONS]
    if not files:
        print(f"No source files found in {SOURCE_DIR}.")
        return
    run(["clang-format", "-i", *files])
    print(f"Formatted {len(files)} files.")


ACTIONS = {
    "clean": clean,
    "generate": generate,
    "build": build,
    "rebuild": rebuild,
    "test": test,
    "format": format_code,
}


def main():
    parser = argparse.ArgumentParser(description="Nexora Engine build automation")
    parser.add_argument("actions", nargs="+", choices=ACTIONS.keys(), help="actions to run in the given order")
    parser.add_argument("--config", choices=CONFIGURATIONS, default="Debug", help="build configuration")
    args = parser.parse_args()

    for action in args.actions:
        print(f"===== {action} ({args.config}) =====", flush=True)
        ACTIONS[action](args.config)


if __name__ == "__main__":
    main()
