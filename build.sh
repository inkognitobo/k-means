#!/usr/bin/env bash
if [ ! -d "./build" ]; then
    mkdir -p "./build"
fi

pushd "./build" > /dev/null

NAME="kmeans"

WARNINGS="-Wall -Wextra"
SANIZIZERS="-fsanitize=address,undefined,leak,signed-integer-overflow"
DEBUG="-ggdb -O3"
STD="-std=c23"
CFLAGS="$WARNINGS $SANITIZERS $STD $DEBUG"

gcc $CLFLAGS -c ../src/main.c ../src/kmeans.c ../src/draw.c -lraylib -lm -I../src/ext/raylib/include -L../src/ext/raylib/lib
gcc $CLFLAGS -o $NAME main.o kmeans.o draw.o -lraylib -lm -I../src/ext/raylib/include -L../src/ext/raylib/lib

popd > /dev/null
