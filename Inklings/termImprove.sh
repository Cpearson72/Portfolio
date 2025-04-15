

#!/bin/bash

exec 2>/dev/null


# Compile the first version (good one)
echo "Compiling main.cpp and ascii_art.cpp..."
g++ main.cpp ascii_art.cpp -o good_exe || { echo "Compilation failed for good_exe"; exit 1; }

# Compile the second version (with older key listener)
echo "Compiling main.cpp and ascii_art_bad_term.cpp..."
g++ main.cpp ascii_art_bad_term.cpp -o bad_term_exe || { echo "Compilation failed for buggy_exe"; exit 1; }

# Display a message about the buggy version
echo 

echo "Try your best following all the inkling movements with the scroll back"
sleep 4

./bad_term_exe 20 20 20

sleep 2

echo 
echo 

echo "Tough to follow with the scroll back right?"

echo
echo

sleep 2

# Run the improved executable with arguments 20 20 20
echo "Now try this beauty. Open your terminal to full size!!!"

sleep 4

./good_exe 20 20 20

# Final message after the executable finishes

echo
echo

sleep 2

echo "Isn't that much better?"

echo

sleep 4
