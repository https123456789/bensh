import os, glob
from time import time

files = glob.glob("src/*.cpp")
files += glob.glob("src/*.hpp")
files += glob.glob("src/**/*.hpp")
files += glob.glob("src/**/*.cpp")

i = 0

if (time() / 100) % 10 == 0:
    print("Clearing object file cache...")
    os.system("rm -r obj")

print("Building object files...")

for file in files:
    print(f"\t\x1b[32m[{i + 1} / {len(files)}]\x1b[0m {file}")
    #os.system(f"clang-format {file} --style=file -i")
    os.system(f"mkdir -p obj && cd obj && g++ -c ../{file} -g")
    i += 1

print("Building executable...")

os.system(f"mkdir -p bin bin/Debug && cd bin/Debug && g++ ../../obj/*.o -o bensh")

print("Done!")