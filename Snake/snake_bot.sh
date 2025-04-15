#!/bin/bash

CC=gcc
FLAGS="-ggdb3 -Wall -Wextra -Wshadow -std=gnu11 -Wno-unused-parameter -Wno-unused-but-set-variable -Werror"

# Check the host system for proper ncurses linking
HOST_SYSTEM=$(uname | cut -f 1 -d_)
if [ "$HOST_SYSTEM" = "Darwin" ]; then
    LIBS="-lncurses"
    FLAGS="$FLAGS -D_XOPEN_SOURCE_EXTENDED"
else
    LIBS=$(ncursesw5-config --libs)
    FLAGS="$FLAGS $(ncursesw5-config --cflags)"
fi

OBJS="src/game.o src/game_setup.o src/game_over.o src/render_apple.o src/common.o src/linked_list.o src/mbstrings.o"

# Compile each source file into object file
for src in ${OBJS//.o/.c}; do
    if [ -f "$src" ]; then
        echo "Compiling $src..."
        $CC $FLAGS -c $src -o ${src%.c}.o
        if [ $? -ne 0 ]; then
            echo "Error compiling $src"
            exit 1
        fi
    else
        echo "Source file $src not found!"
        exit 1
    fi
done

# Link everything together with snake_bot.c instead of snake.c
echo "Linking snake_bot executable..."
$CC $FLAGS src/snake_bot.c $OBJS $LIBS -o snake_bot -lm

if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Link failed!"
    exit 1
fi

echo
echo "Running snake_bot with improvements: "
echo
echo "Bot controller, solid snake, apple for food"
echo
sleep 2
echo "Disclaimer: The food introduces some buggy visual behavior, but is fully functional"
sleep 2
echo
echo "Running now. Game will stop at score of 10 but can run forever"
sleep 2
echo

./snake_bot 0

echo
sleep 2
echo "Great. Now let's run the snake with growth. This seed will fail at a growth of 8."
sleep 2
echo

./snake_bot 1


echo
sleep 2
echo "Thanks for testing!"

