import os, glob

files = glob.glob("src/*.cpp")
files += glob.glob("src/*.hpp")
files += glob.glob("src/**/*.hpp")
files += glob.glob("src/**/*.cpp")

i = 0

print("Building Object Files...")

for file in files:
    print(f"\t [{i + 1} / {len(files)}] {file}")
    os.system(f"clang-format {file} --style=file -i")
    os.system(f"mkdir -p obj && cd obj && g++ -c ../{file} -g")
    i += 1

print("Building Executable...")

os.system(f"mkdir -p bin bin/Debug && cd bin/Debug && g++ ../../obj/*.o -o bensh")

print("Done!")