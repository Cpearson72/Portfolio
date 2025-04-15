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

OBJS="src/game.o src/game_setup.o src/game_over.o src/render.o src/common.o src/linked_list.o src/mbstrings.o"

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
echo "Linking snake executable..."
$CC $FLAGS src/snake.c $OBJS $LIBS -o snake -lm

if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Link failed!"
    exit 1
fi


echo
sleep 4
echo "Running unit test on custom board in order to test proper snake movement according to inputs,"
sleep 4
echo "snake failure cases of hitting the wall and hitting itself, board decompression from RLE input, food functionality to ensure"
sleep 4
echo "food, score, and growth updates upon snake head eating food, name input using any ASCII or unicode characters, and proper game over screen"

sleep 4

echo
echo "first, let's do a default board so you can test movement"

sleep 4

./snake 1

sleep 4

echo
echo "starting custom board program now.."

sleep 2

echo
./snake 1 "B10x10|W10|W1E8W1|W3E6W1|W1E3S1E4W1|W1E8W1|W1E8W1|W1E5W4|W1E1W1E6W1|W1E1W1E6W1|W10"


sleep 2

echo
echo "let's run one more time so you get the chance to test any other features you did not get to. Here is an apple to test out UNICODE: 🍎"

sleep 2

echo
echo "Running"
sleep 4

echo
./snake 1 "B10x10|W10|W1E8W1|W3E6W1|W1E3S1E4W1|W1E8W1|W1E8W1|W1E5W4|W1E1W1E6W1|W1E1W1E6W1|W10"


sleep 2

echo

echo "Great work. Now let's run two faulty boards to show the decompression catches bad boards."

sleep 2

echo
echo "This board will have a row that is too long"
echo
echo "Running ./snake 0 B5x10|W10|W1E8W5|W1E8W1|W1E3S1E4W1|W10"
sleep 4
echo

./snake 0 "B5x10|W10|W1E8W5|W1E8W1|W1E3S1E4W1|W10"

sleep 2
echo
echo "This board will have too few rows"
echo
echo "Running ./snake 0 B5x10|W10|W1E8W1|W1E3S1E4W1|W10"

sleep 4


echo
./snake 0 "B5x10|W10|W1E8W1|W1E3S1E4W1|W10"