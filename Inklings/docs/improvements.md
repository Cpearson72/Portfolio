## Change 1: <Improved responsivity and funcionality with keyboard inputs>

### Proposed Change Summary

- std::cin.get() employs overhead as it is not designed for
real time input handling. Using read(FILEN0) on this standard input file descriptor, removes this and improves responsivity and functionality with keyboard inputs.

### Describe the Problem

- std::cin.get() employs overhead for the purpose of error handling, type checking, platform independence, and C++ stream compatibility. This causes delayed updates in the inkling behavior and takes multiple refreshes to show around three to four key updates. This occurs every time without fail and is the same issue of laggy response each time.

### Describe the Solution

- We do not need platform compatibility(Unix only), nor do we need stream compatibility or error handling. read(FILEN0, &c, 1) is Unix specific and reads in rawbytes to the size of one character. This size specification protects us from buffer overflow while pertaining more to our specific needs compared to std::cin.get(). This lack of overhead provides immediate updates to the behavior and state.

#### Old Solution Code
void oldKeyListener(){
    enableRawMode();
    char c;
    c = std::cin.get();

    myKeyboard(c);

    disableRawMode();
}

#### New Solution Code

void keyListener() {
    // File control function, FILENO is standard input, F_SETFL
    //allows us to set NON_BLOCK FLAG, which makes input non-blocking, ensuring our
    //operation run even while waiting for input. This works perfectly for the nature of our updating display logic.
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    char c;
    while (true) {
        //if input then read is greater than 0
        if (read(STDIN_FILENO, &c, 1) > 0) { 
            //Lock Key so two inputs don't happen at once
            std::lock_guard<std::mutex> lock(keyMutex);
            //Perform operation
            myKeyboard(c);
        }
        // Add a small sleep to prevent CPU spinning
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}


#### TO TEST PLEASE RUN THIS COMMAND "./keyListenerImprove.sh"





## Change 2: <Better ascii art refresh>

### Proposed Change Summary

- Change the escape codes in the "clearTerminal()" function for better functionality in the terminal.

### Describe the Problem

- Old escape codes in clearTerminal() do not clear scroll back, making the inkling behavior hard to follow and understand. Current codes are \033[H] which moves cursor to the top-left corner, and \033[J] clears everything below the cursor, leaving the scroll back.

### Describe the Solution

- Change escape codes to \033[2J] to clear the entire visible screen, \033[3J] to clear scrollback buffer, \033[H] moves the cursor to the top-left to ensure proper future prints, and std::flush gives immediate output execution.


#### OLD CODE:

void badClearTerminal() {
	/* system specific
    #ifdef _WIN32
        system("cls");
    
        system("clear");
    #endif
	*/

	// this is easier
	std::cout << "\033[H\033[J"; // clear the terminal screen ;)
}


#### NEW CODE:

void clearTerminal() {
    std::cout << "\033[2J\033[3J\033[H" << std::flush;
}


#### TO TEST RUN:

./termImprove.sh