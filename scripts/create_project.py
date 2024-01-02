import os
import sys
import subprocess
import shutil
import glob

script_dir = os.path.dirname(os.path.realpath(__file__))
engine_dir = os.path.abspath(os.path.join(script_dir, '..'))
project_dir = ""

def build_engine():
    os.chdir(engine_dir)
    build_dir = os.path.join(engine_dir, 'build')

    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)

    cmake_command = ['cmake', '..']
    subprocess.run(cmake_command, check=True)

    build_command = ['cmake', '--build', '.']
    subprocess.run(build_command, check=True)
    print("Built bee dog engine")

def copy_files_matching_pattern(source_dir, destination_dir, pattern):
    files_to_copy = glob.glob(os.path.join(source_dir, pattern))
    for file_path in files_to_copy:
        shutil.copy2(file_path, destination_dir)

def create_project():
    updating = False
    if not os.path.exists(project_dir):
        os.makedirs(project_dir)
    else:
        q = input("Project already exists, update project's engine? (Y/N): ")
        if str(q).lower() != "y":
            return
        updating = True
    os.chdir(project_dir)
    os.makedirs("src", exist_ok=True)
    os.makedirs("lib", exist_ok=True)
    os.makedirs("include", exist_ok=True)
    src = os.path.abspath(os.path.join(os.getcwd(), "src"))
    lib = os.path.abspath(os.path.join(os.getcwd(), "lib"))
    include = os.path.abspath(os.path.join(os.getcwd(), "include"))
    if not updating:
        # copy a main.cpp file into src
        shutil.copy(os.path.join(script_dir, "example_main.txt"), os.path.join(src, "main.cpp"))
        # copy sample cmakelists.txt
        shutil.copy(os.path.join(script_dir, "example_CMakeLists.txt"), os.path.join(project_dir, "CMakeLists.txt"))
    # copy all libraries (raylib and bdengine)
    shutil.copy2(os.path.join(engine_dir, "build/_deps/raylib-build/raylib/libraylib.a"), lib)
    shutil.copy2(os.path.join(engine_dir, "build/libbdengine.a"), lib)
    # copy all includes (vendors, *.hpp related to bdengine, *.h related to raylib)
    vendor_path  = os.path.join(include, "vendor")
    if os.path.exists(vendor_path):
        shutil.rmtree(vendor_path)
    shutil.copytree(os.path.join(engine_dir, "vendor"), vendor_path)
    copy_files_matching_pattern(os.path.join(engine_dir, "engine"), include, "*.hpp")
    copy_files_matching_pattern(os.path.join(engine_dir, "build/_deps/raylib-build/raylib/include"), include, "*.h")

    if updating:
        print(f"Updated {project_dir} to latest bee dog engine")
    else:
        print(f"Created project at {project_dir}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python create_project.py <project_name>")
        sys.exit(1)

    project_dir = os.path.join(engine_dir, sys.argv[1])
    build_engine()
    create_project()
