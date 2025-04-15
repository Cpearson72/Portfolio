# Part 2 - Clinton Ijifisoye
Set up the grid and thread creation:

    Parse command-line arguments for grid dimensions and the number of inkling threads.

    Initialize the grid as a 2D array of integers to represent cell colors.

    Create and manage inkling threads using pthread or std::thread.

Implement inkling behavior:

    Assign each inkling a random color, starting position, and orientation.

    Implement movement logic: randomly select a perpendicular direction and displacement length, ensuring the inkling stays within grid bounds.

    Grid updates and rendering:

    Update the grid with the inkling's color and direction (arrow) as they move.

    Ensure thread-safe updates to the grid to avoid race conditions.

Thread termination:

    Terminate inkling threads when they reach a grid corner.

    Optionally, join terminated threads after the myEventLoop() call to avoid blocking the event loop.

    Testing and debugging:

    Test the program with different grid sizes and numbers of inklings.


2. Potential Challenges and Research Areas

Thread synchronization:

Ensuring thread-safe updates to the grid when multiple inklings move simultaneously.

Handling race conditions when accessing shared resources like ink levels and grid cells.

Movement logic:

Implementing random movement while ensuring inklings stay within grid bounds.

Calculating valid displacement lengths and directions efficiently.

Research Areas:
Thread synchronization mechanisms:
    Research pthread_mutex or std::mutex to protect shared resources.
    Explore condition variables for managing ink resource refills.
    Random number generation:
    Find out how to generate random numbers within specific ranges for movement directions and lengths.
    Ensure randomness is sufficiently distributed to avoid biased movement patterns.

Functions and Data Structures
Instructor
| 03/10 at 3:31 pm
Grading comment:
The code outlines you have here look very well thought out and well developed

    Data Structures:
        Grid: A 2D array of integers (int grid[height][width]) to store cell colors.

InklingInfo: A struct to store inkling-specific data:
    struct InklingInfo {
        int id; // Unique inkling ID
        InklingType color; // Red, green, or blue
        int row, col; // Current position
        TravelDirection direction; // Current orientation
        int inkLevel; // Current ink resources
        bool isLive; // Whether the inkling is active
    };
Functions:
Thread function:

    void* inklingThread(void* arg) {
        InklingInfo* info = (InklingInfo*) arg;
        while (info->isLive) {
            // Move inkling
            // Update grid
            // Check for termination conditions
        }
        return nullptr;
    }

Movement logic:

    void moveInkling(InklingInfo* info) {
        // Randomly select a perpendicular direction
        // Calculate valid displacement length
        // Update inkling position and grid
    }
Grid update:


    void updateGrid(int row, int col, InklingType color, TravelDirection direction) {
        // Update grid cell with color and direction
    }

Ink management:

    void refillInk(InklingInfo* info) {
        // Refill ink resources
    }

Research Plan
    Thread synchronization:
        Study pthread_mutex and std::mutex for protecting shared resources.

Random number generation:
Research the <random> library in C++ for generating random numbers.
Lastly, Implement a function to generate random directions.


# Part 3: Create Log Files for the Inklings -William

The easiest part of this is creating a logFolder with 0755 permissions using mkdir and chmod() while also checking if (if necessary) that logFolders exists already.

    struct stat info;
    if(stat(directoryName.c_str(), &info) != 0){
        mkdir(directoryName.c_str(), 0755);
    }

Looking back at how threadX.txt files were made in lab4, inklingX.txt files are going to be made similarily

    // Create threads
    std::vector<std::thread> threadList;
    for (int i = 1; i <= randomNumber; i++) {
        threadList.emplace_back(threadFunc, i);
    }

    // Wait for all threads to finish
    for (auto& t : threadList) {
        t.join();
    }

We have the number of inkling files we would like to make in a vector, and then we have 
the second loop to ensure that all threads finish
before continuing to execute the program to prevent leaks

As for outputing the content to the files (hour:minute:second:ms,inkingX,color,row,col), using std::chrono::system_clock to obtain the current time and creating the time stamp with std::put_time() and std::ostringstream, this is how I would imagine

// From Lab4
outFile << "This thread's value is " << num << ".\n";

// What I might possibly do

    // Get current time
    auto currentTime = std::chrono::system_clock::now();

    // time_t to seconds
    std::time_t now_time = std::chrono::system_clock::to_time_t(currentTime)

    // time (for miliseconds)
    auto currentMS = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;

    // Formatting the string
    std::ostringstream 0ss;
    outFile << oss << std::put_time(std::localtime(&now_time_t), "%H:%M%:%S) << '.' << std::setfill('0') << std::setw(3) << now_ms.count() << 
    ",inkling" << x << "," << positionOrColor << row << "," << col;

Given the goal of using a "make logs" to real ALL the files from logFolder, combine the and sort by time and finally saving the file in logFolder with "actions.txt" with 0755 permissions

    // Using a struct to organize the time stamp
    struct Logs {
        std::string timestamp;
        std::string line;

        // Compare time for sorting
        bool operator<(const Logs& other) const {
            return timestamp < other.timestamp;
        }
    };

    /**
    * @brief Reads all log files from the specified directory.
    * @param logFolder Path to the directory containing log files.
    * @return A vector of LogEntry objects, each representing a log entry.
     */
    std::vector<Logs> readLogs(std::string& logDirectory) {
        std::vector<Logs> logEntries;

        for(const auto& entry : fs::directory_iterator(logDirectory)){
        if (entry.path().extension() == ".txt" && entry.path().filename().string() != "actions.txt") {
            std::ifstream file(entry.path());
            std::string line;
            while (std::getline(file, line)) {
                size_t commaPos = line.find(',');  // Find first comma
                if (commaPos != std::string::npos) {
                    std::string timestamp = line.substr(0, commaPos);  // Extract timestamp
                    logEntries.push_back({timestamp, line});
                    }
                }
            }
        }
    }

    /**
     * @brief Writes sorted log entries to actions.txt.
     * @param logFolder Path to the directory where actions.txt will be created.
     * @param logEntries Vector of sorted LogEntry objects to write to the file.
    */
    void writeSortedLogs(const std::string& logFolder, const std::vector<Logs>& logEntries){
        std::ofstream outFile(logFolder + "/actions.txt");
        chmod(outFile.c_str(), 0755);


        for (const auto& entry : logEntries) {
            outFile << entry.fullLine << '\n';
        }
    }

# Part 4 - Conor Pearson

This part be our last move. To better understand the functions and the 
efficiency of the data structure holding the inklings, we must use the 
debugger. From there we can brainstorm better data structures and work out 
bugs in the code. Understanding how the inklings are used and manipulated 
in memory will be key to this as well, as every line of code surrounding 
the behavior of the inklings can have major consequences on said behavior.
The easy guess as of now is to research common bugs while using threads. 
While our programming until now has been easy to follow and highly 
predictable, the introduciton of threads will add another layer of 
abstraction and unusual behavior.

#Thread bugs to look out for as of now:

- Race Conditons:

        #include <iostream>
        #include <thread>

        int counter = 0;

        void increment() {
            for (int i = 0; i < 100000; ++i) {
                counter++;  // Not thread-safe!
            }
        }

        int main() {
            std::thread t1(increment);
            std::thread t2(increment);

            t1.join();
            t2.join();

            std::cout << "Counter: " << counter << std::endl;  // Unpredictable result
        }

        Deadlocks:

            #include <iostream>
            #include <thread>
            #include <mutex>

            std::mutex mtx1, mtx2;

            void thread1() {
                std::lock_guard<std::mutex> lock1(mtx1);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::lock_guard<std::mutex> lock2(mtx2);  // Waiting for mtx2
            }

            void thread2() {
                std::lock_guard<std::mutex> lock2(mtx2);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                std::lock_guard<std::mutex> lock1(mtx1);  // Waiting for mtx1
            }

            int main() {
                std::thread t1(thread1);
                std::thread t2(thread2);

                t1.join();
                t2.join();

                std::cout << "Finished" << std::endl;
            }


- Inefficient Locking:

    #include <iostream>
    #include <thread>
    #include <vector>
    #include <mutex>

    std::vector<int> numbers;
    std::mutex mtx;

    void add_numbers(int start) {
        std::lock_guard<std::mutex> lock(mtx);  // Holding lock for too long
        for (int i = start; i < start + 5; ++i) {
            numbers.push_back(i);
        }
    }

    int main() {
        std::thread t1(add_numbers, 0);
        std::thread t2(add_numbers, 5);

        t1.join();
        t2.join();

        for (int n : numbers) std::cout << n << " ";
    }

- This part will not require data structures but we could write a logging function, that 
    allows us to see what processes were executed and when in comparison to the other threads:

    std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto timeT = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S") 
            << "." << ms.count();
        return oss.str();
    }

    // Thread-safe logging function
    void logMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::ostringstream logEntry;
        logEntry << "[" << getTimestamp() << "] [Thread " << std::this_thread::get_id() << "] " << message << "\n";

        // Print to console
        std::cout << logEntry.str();
        
        // Log to file
        if (logFile.is_open()) {
            logFile << logEntry.str();
            logFile.flush();
        }
    }
