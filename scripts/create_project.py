import os
import sys
import subprocess

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

def create_project():
    if not os.path.exists(project_dir):
        os.makedirs(project_dir)
    os.chdir(project_dir)
    os.makedirs("src")
    os.makedirs("lib")
    os.makedirs("include")
    src = os.path.abspath(os.path.join(os.getcwd(), "src"))
    lib = os.path.abspath(os.path.join(os.getcwd(), "lib"))
    include = os.path.abspath(os.path.join(os.getcwd(), "include"))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python create_project.py <project_name>")
        sys.exit(1)

    project_dir = os.path.join(engine_dir, sys.argv[1])
    build_engine()
    print("Built bee dog engine")
    create_project()
    print(f"Created project at {project_dir}")
