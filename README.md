# Assignment 2 for IS F311 Computer Graphics

## RUN TASK 1 & 2 (castle.cpp):
g++ castle.cpp -o castle.exe -lfreeglut -lopengl32 -lglu32 "-IC:/msys64/mingw64/include" "-LC:/msys64/mingw64/lib"


OR
g++ castle.cpp -IC:"\MinGW\freeglut\include" -LC:"\MinGW\freeglut\lib" -lfreeglut -lopengl32 -lglu32 -o castle.exe


## RUN TASK 3(mesh.cpp):
g++ mesh.cpp -o mesh.exe
