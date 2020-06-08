cd '/Users/hugogomes/Desktop/Aulas/P/Trabalho/Codigo/SimuladorVirus'
/usr/bin/make -f Makefile CONF=Debug
"/Applications/Xcode.app/Contents/Developer/usr/bin/make" -f nbproject/Makefile-Debug.mk QMAKE= SUBPROJECTS= .build-conf
"/Applications/Xcode.app/Contents/Developer/usr/bin/make"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-MacOSX/simuladorvirus
mkdir -p build/Debug/GNU-MacOSX
rm -f "build/Debug/GNU-MacOSX/main.o.d"
gcc    -c -g -std=c99 -MMD -MP -MF "build/Debug/GNU-MacOSX/main.o.d" -o build/Debug/GNU-MacOSX/main.o main.c
mkdir -p dist/Debug/GNU-MacOSX
gcc     -o dist/Debug/GNU-MacOSX/simuladorvirus build/Debug/GNU-MacOSX/espacos.o build/Debug/GNU-MacOSX/main.o build/Debug/GNU-MacOSX/people.o build/Debug/GNU-MacOSX/utils.o
clear
cd dist/Debug/GNU-MacOSX && ./simuladorvirus
