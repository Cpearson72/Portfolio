

#!/bin/bash

exec 2>/dev/null


# Compile the first version (good one)
echo "Compiling main.cpp and ascii_art.cpp..."
g++ main.cpp ascii_art.cpp -o good_exe || { echo "Compilation failed for good_exe"; exit 1; }

# Compile the second version (with older key listener)
echo "Compiling main.cpp and ascii_art_slow.cpp..."
g++ main.cpp ascii_art_slow.cpp -o buggy_exe || { echo "Compilation failed for buggy_exe"; exit 1; }

# Display a message about the buggy version
echo 

echo "Try this buggy mess, spam "-" or "+" fast and the terminal will suffer >:)"
sleep 4

./buggy_exe 20 20 20

sleep 4

echo 
echo 

echo "Didn't that suck. If not, make sure to SPAMMMMM."

echo
echo

sleep 4

# Run the improved executable with arguments 20 20 20
echo "Now try this beauty. Spam as much as your heart desires."

sleep 4

./good_exe 20 20 20

# Final message after the executable finishes

echo
echo

echo "Isn't that much better!"

echo

sleep 4
