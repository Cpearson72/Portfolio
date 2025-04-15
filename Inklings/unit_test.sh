

#!/bin/bash

exec 2>/dev/null


# Compile the first version (good one)
echo "Compiling main.cpp and ascii_art.cpp..."
g++ main.cpp ascii_art.cpp -o good_exe || { echo "Compilation failed for good_exe"; exit 1; }


echo
echo "First we will compile with no arguments. Test out any key input you would like"
sleep 2

echo "If successful, they will show up on the terminal state :)"

sleep 4

./good_exe

sleep 4

echo 
echo 

echo "That was great, but let's increase the size, to show the program functions properly"

echo

sleep 4

echo "Running with 20 inklings on 20 by 20 grid"

sleep 4

./good_exe 20 20 20

# Final message after the executable finishes

echo 
echo
sleep 2

echo "Are you running all the keyboard inputs you want? Try em out"

sleep 4

echo "Okay let's get more inklings!!!"

echo

sleep 4

echo "Running with 100 inklings on 20 by 20 grid"

./good_exe 20 20 100

sleep 4

echo "That was cool. EVEN MORE!!"

echo

sleep 4

echo "Running with 200 inklings on 20 by 20 grid"

sleep 4

./good_exe 20 20 200


sleep 2

echo "EVEN MORE!!!:"

echo

sleep 4

echo "Running with 400 inklings on 20 by 20 grid"

sleep 4

echo "Just kidding lol. The odds of the last inkling randomly finding the last open cell is very very low."

sleep 2

echo "I hope that was enough to show you the program functions properly"

sleep 2


