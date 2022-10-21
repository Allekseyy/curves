DLL_OBJS = obj/curve.o
EXE_OBJS = obj/curvetest.o

WARNS = -Wall

# Names of tools to use when building
CC = g++
RC = windres
EXE = CurvesTest.exe
DLL = CurvesLib.dll

# Compiler flags
EXE_CFLAGS = -O2 ${WARNS} -Iinclude
DLL_CFLAGS = ${EXE_CFLAGS} -D ADD_EXPORTS

# Linker flags
DLL_LDFLAGS = -shared -s -Wl,--subsystem,windows,--out-implib,lib/libaddlib.a
EXE_LDFLAGS = -Llib -laddlib -s -Wl,--subsystem,console

.PHONY: all clean

# Build DLL and executable by default
all: bin/${DLL} bin/${EXE}

# Delete all build output
clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Create build output directories if they don't exist
bin lib obj:
	@if not exist "$@" mkdir "$@"

# Compile object files for DLL
obj/curve.o: src/curve.cpp src/curve.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Compile object files for executable
obj/curvetest.o: src/main.cpp | obj
	${CC} ${EXE_CFLAGS} -c "$<" -o "$@"

# Build the resource files
obj/%.o: res/%.rc | obj
	${RC} -Iinclude -Ires -i "$<" -o "$@"

# Build the DLL
bin/${DLL}: ${DLL_OBJS} | bin lib
	${CC} -o "$@" ${DLL_OBJS} ${DLL_LDFLAGS}

# Buld the executable
bin/${EXE}: ${EXE_OBJS} | bin
	${CC} -o "$@" ${EXE_OBJS} ${EXE_LDFLAGS}