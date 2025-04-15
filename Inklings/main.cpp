//
//  main.cpp
//  inklings
//
//  Authors: Jean-Yves Hervé, Shaun Wallace, and Luis Hernandez
//

 /*-------------------------------------------------------------------------+
 |	A graphic front end for a grid+state simulation.						|
 |																			|
 |	This application simply creates a a colored grid and displays           |
 |  some state information in the terminal using ASCII art.			        |
 |	Only mess with this after everything else works and making a backup		|
 |	copy of your project.                                                   |
 |																			|
 |	Current Keyboard Events                                     			|
 |		- 'ESC' --> exit the application									|
 |		- 'r' --> add red ink												|
 |		- 'g' --> add green ink												|
 |		- 'b' --> add blue ink												|
 +-------------------------------------------------------------------------*/

#include <random>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sys/stat.h>   
#include <errno.h>      
#include <string>
#include <chrono>       
#include <iomanip>     
#include <sstream>  
#include <termios.h> 

struct termios Original_Termios;


#include "ascii_art.h"

//==================================================================================
//	Function prototypes
//==================================================================================
void displayGridPane(void);
void displayStatePane(void);
void initializeApplication(void);
void threadFunction(InklingInfo* inkling, int threadNum, bool firstLog);
void getNewDirection(InklingInfo* inkling);
bool checkIfInCorner(InklingInfo* inkling);
void redColorThreadFunc();
void greenColorThreadFunc();
void blueColorThreadFunc();
bool checkEnoughInk(InklingInfo* inkling, int* moveAmount);
bool uniqueStartPosition(int row, int col);
void moveInkling(int**grid, InklingInfo* inkling, int moveAmount);
bool validInklingMove(InklingInfo* inkling, int moveAmount);
void logThread(InklingInfo* inkling, int threadNum, bool firstLog);
void clearLogFile(int threadNum);
void acquireInkFromType(InklingInfo* inkling, int moveAmount);
void terminationLog(InklingInfo* inkling, int threadNum);
bool wasLogTerminated(InklingInfo* inkling, int threadNum);
std::ofstream openDirAndFile(int threadNum);

//==================================================================================
//	Application-level global variables
//==================================================================================

bool logTermination = false;

//	The state grid and its dimensions
int** grid;
int NUM_ROWS, NUM_COLS;

//	the number of live threads (that haven't terminated yet)
int MAX_NUM_TRAVELER_THREADS;
int numLiveThreads = 0;

//vector to store each struct
std::vector<InklingInfo> info;
bool DRAW_COLORED_TRAVELER_HEADS = true;

//	the ink levels
int MAX_LEVEL = 50;
int MAX_ADD_INK = 10;
int REFILL_INK = 10;
int redLevel = 20, greenLevel = 10, blueLevel = 40;

// create locks for color levels
std::mutex redLock;
std::mutex blueLock;
std::mutex greenLock;
std::mutex blueCellLock;
std::mutex redCellLock;
std::mutex greenCellLock;
std::mutex gridMutex;
std::mutex updatingState;

// ink producer sleep time (in microseconds)
// [min sleep time is arbitrary]
const int MIN_SLEEP_TIME = 30000; // 30000
int producerSleepTime = 10000; // 100000

// inkling sleep time (in microseconds)
int inklingSleepTime = 100000; // 1000000


//==================================================================================
//	These are the functions that tie the simulation with the rendering.
//	Some parts are "don't touch."  Other parts need your help to ensure
//	that access to critical data and the ASCII art are properly synchronized
//==================================================================================

void displayGridPane(void) {
	//---------------------------------------------------------
	//	This is the call that writes ASCII art to render the grid.
	//
	//	Should we synchronize this call?
	//---------------------------------------------------------
	//lock grid
	std::lock_guard<std::mutex> lock(gridMutex);
    drawGridAndInklingsASCII(grid, NUM_ROWS, NUM_COLS, info);

}

void displayStatePane(void) {
	//---------------------------------------------------------
	//	This is the call that updates state information
	//
	//	Should we synchronize this call?
	//---------------------------------------------------------
	//lock state
	std::lock_guard<std::mutex> lock(updatingState);
	drawState(numLiveThreads, redLevel, greenLevel, blueLevel);

}

//------------------------------------------------------------------------
//	These are the functions that would be called by a inkling thread in
//	order to acquire red/green/blue ink to trace its trail.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------
// You probably want to edit these...
bool acquireRedInk(int theRed) {

	//NOTE: OK IS RETURNED TRUE IF IT WAS ABLE TO GET THE INK
	bool ok = false;


	if (redLevel >= theRed)
	{	
		//locks state to ensure ink is not being accessed
		//by two threads
		std::lock_guard<std::mutex> lock(updatingState);
		redLevel -= theRed;
		ok = true;
	}

	return ok;
}

bool acquireGreenInk(int theGreen) {
	bool ok = false;


	
	if (greenLevel >= theGreen)
	{	
		std::lock_guard<std::mutex> lock(updatingState);
		greenLevel -= theGreen;
		ok = true;
	}

	return ok;
}

bool acquireBlueInk(int theBlue) {
	bool ok = false;
	
	if (blueLevel >= theBlue)
	{	
		std::lock_guard<std::mutex> lock(updatingState);
		blueLevel -= theBlue;
		ok = true;
	}


	return ok;
}


//------------------------------------------------------------------------
//	These are the functions that would be called by a producer thread in
//	order to refill the red/green/blue ink tanks.
//	You *must* synchronize access to the ink levels (C++ lock and unlock)
//------------------------------------------------------------------------
// You probably want to edit these...
bool refillRedInk(int theRed) {
	bool ok = false;
	//lock red resource
	if (redLevel + theRed <= MAX_LEVEL)
	{	
		std::lock_guard<std::mutex> lock(updatingState);
		//std::cout << "refilling red" << std::endl;
		redLevel += theRed;
		ok = true;
	}

	return ok;
}

bool refillGreenInk(int theGreen) {
	bool ok = false;
	//lock green resource
	if (greenLevel + theGreen <= MAX_LEVEL)
	{	
		//std::cout << "refilling green" << std::endl;
		std::lock_guard<std::mutex> lock(updatingState);
		greenLevel += theGreen;
		ok = true;
	}

	return ok;
}

bool refillBlueInk(int theBlue) {
	bool ok = false;
	if (blueLevel + theBlue <= MAX_LEVEL)
	{	
		//std::cout << "refilling blue" << std::endl;
		std::lock_guard<std::mutex> lock(updatingState);
		blueLevel += theBlue;
		ok = true;
	}

	return ok;
}

//------------------------------------------------------------------------
//	You shouldn't have to touch this one.  Definitely if you do not
//	add the "producer" threads, and probably not even if you do.
//------------------------------------------------------------------------
void speedupProducers(void) {
	// decrease sleep time by 20%, but don't get too small
	int newSleepTime = (8 * producerSleepTime) / 10;
	
	if (newSleepTime > MIN_SLEEP_TIME) {
		producerSleepTime = newSleepTime;
	}
}

void slowdownProducers(void) {
	// increase sleep time by 20%
	producerSleepTime = (12 * producerSleepTime) / 10;
}

//-------------------------------------------------------------------------------------
//	You need to change the TODOS in the main function to pass the the autograder tests
//-------------------------------------------------------------------------------------
int main(int argc, char** argv) {
    // a try/catch block for debugging to catch weird errors in your code
    try {
        // check that arguments are valid, must be a 20x20 or greater and at least 8 threads/inklings
        if (argc == 4) {
            if (std::stoi(argv[1]) >= 20 && std::stoi(argv[2]) >= 20 && std::stoi(argv[3]) >= 8) {
                NUM_ROWS = std::stoi(argv[1]);
                NUM_COLS = std::stoi(argv[2]);
                MAX_NUM_TRAVELER_THREADS = std::stoi(argv[3]);
                numLiveThreads = std::stoi(argv[3]);
            }
        } else {
            std::cout << "No arguments provided, running with 8x8 grid and 4 threads.\n\tThis message will dissapear in 2 seconds... \n";
            sleep(2); // so the user can read the message in std::cout one line up
            //clearTerminal();
            // some small defaults, will these run?
            NUM_ROWS = 8;
            NUM_COLS = 8;
            MAX_NUM_TRAVELER_THREADS = 4;
            numLiveThreads = 4;
        }
        
        
    

		
		//Initialize inkling information
		initializeApplication();
		std::cout << "initialize app success/n" << std::endl;

		//Initialize front end with the inkling information
		initializeFrontEnd(argc, argv, displayGridPane, displayStatePane);
		std::cout << "initialize front end success/n" << std::endl;

        

        // TODO: create producer threads that check the levels of each ink
		if (MAX_NUM_TRAVELER_THREADS > 0)
			{	
				//Producer threads detached so they run solo
				//and do not require a rejoin.
				std::thread redThread(redColorThreadFunc);
				redThread.detach();
				std::thread greenThread(greenColorThreadFunc);
				greenThread.detach();
				std::thread blueThread(blueColorThreadFunc);
				blueThread.detach();
			}



		//At this point we have all the inkling info in our info vector. So
		//now we use this to create the threads
		
		//Variable used for the initial log in our txt files
        bool firstLogEntry = true;
        // TODO: create threads for the inklings
		for (long unsigned int i = 0; i < info.size(); i++)
			{	
				//Using inkling initial info, create the threads.
				std::thread t(threadFunction, &info[i], i + 1, firstLogEntry);
				t.detach();
			}

        
        // now we enter the main event loop of the program
        myEventLoop(0);

        // ensure main does not return immediately, killing detached threads
        std::this_thread::sleep_for(std::chrono::seconds(30));
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR :: Oh snap! unhandled exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "ERROR :: Red handed! unknown exception caught" << std::endl;
    }

	cleanupAndQuit("Goodbye");
	return 0;
}


//==================================================================================
//
//	TODO this is a part that you have to edit and add to.
//
//==================================================================================

void cleanupAndQuit(const std::string& msg) {

	//Unix system call that set's terminal attributes
	//to what it was before application initialized.
	//This fixes the weird bug where terminal characters aren't shown when typed after execution.
	tcsetattr(STDIN_FILENO, TCSANOW, &Original_Termios);

    std::cout << "Somebody called quits, goodbye sweet digital world, this was their message: \n" << msg << std::endl;
	// should we join all the threads before you free the grid and other allocated data structures.  
    // you may run into seg-fault and other ugly termination issues otherwise.

	
	// also, if you crash there, you know something is wrong in your code.
	for (int i=0; i< NUM_ROWS; i++)
		delete []grid[i];
	delete []grid;

	// clear the inkling list
	for(int i = 0; i < info.size(); i++){
		terminationLog(&info[i], i + 1);
	}
	info.clear();
	
    exit(0);
}



void initializeApplication(void) {
	//Saves original terminal state to fix buggy terminal after execution
	tcgetattr(STDIN_FILENO, &Original_Termios);
    
	//Initialize grid that inklings traverse
	grid = new int*[NUM_ROWS];
	for (int i=0; i<NUM_ROWS; i++)
		grid[i] = new int[NUM_COLS];
	
	//---------------------------------------------------------------
	//	The code block below to be replaced/removed
	//	I initialize the grid's pixels to have something to look at
	//---------------------------------------------------------------
	//	Random generator for the initial colors
	//	The initial cell color generation code should go away, but you
	//	should use tese for position and inkling type generation.
	std::random_device myRandDev;
	std::default_random_engine myEngine(myRandDev());
	
	for (int i=0; i<NUM_ROWS; i++) {
		for (int j=0; j<NUM_COLS; j++) {
			// the intialization you should use
            grid[i][j] = 3;
		}
	}

	//---------------------------------------------------------------
	//	TODO you are going to have to properly initialize your  
	//	inklings at random locations on the grid:
	//		- not at a corner
	//		- not at the same location as an existing inkling
	//---------------------------------------------------------------

    // TODO random number generation initialization
	std::random_device myRan;
	std::default_random_engine engine(myRan());


    // TODO maybe some kind of loop here?

	//Loop to create all inkling info(not the threads) using random
	for(int thread = 0; thread < MAX_NUM_TRAVELER_THREADS; thread++){
		int row = 0;
		int col = 0;
		//get random color
		InklingType color = static_cast<InklingType>(engine() % 3);
		
		//randomizes row and col until a unique Position is found.
		bool uniqueCell = false;
		while(uniqueCell == false){
			row = engine() % NUM_ROWS;
			//get random column
			col = engine() % NUM_COLS;

			//is position fresh?
			uniqueCell = uniqueStartPosition(row, col);
		}
		
		//get random direction
		TravelDirection direction = static_cast<TravelDirection>(engine() % 4);

		InklingInfo inked = { color, row, col, direction, true};
		info.push_back(inked);
		grid[row][col] = color;
	}

    // TODO here is some starter code so everything does not crash
    // we prob won't need this exactly in the final solution
	/*
    InklingInfo inked = {	RED_TRAV,
							1, 5,
							EAST, true};
	info.push_back(inked); // aka the inklings
	*/
}


// TODO help me please, I am a vagabond
void threadFunction(InklingInfo* inkling, int threadNum, bool firstLogEntry) {
	
	//clear it's logs
	clearLogFile(threadNum);

	//firstLog
	logThread(inkling, threadNum, firstLogEntry);
	firstLogEntry = false;

	//get random device
	std::random_device myRan;
	std::default_random_engine engine(myRan());


	//at this point all the inkling threads will enter this function
	//we run until it dies
	int moveAmount = 0;
	while(inkling->isLive){
		//get random move number using numrows and cols to ensure it does not
		//fall out
		if(inkling->dir == 0 or inkling->dir == 2){
			moveAmount = engine() % (NUM_ROWS);
		}
		if(inkling->dir == 1 or inkling->dir == 3){
			moveAmount = engine() % (NUM_COLS);
		}

	
		//Move
		moveInkling(grid, inkling, moveAmount);

		//Log after move
		logThread(inkling, threadNum, firstLogEntry);


		if(checkIfInCorner(inkling)){
			//kill the inkling if in corner
			inkling->isLive = false;
			numLiveThreads--;
			logThread(inkling, threadNum, firstLogEntry);
		}


		//get new direction for next move since we are not in corner
		getNewDirection(inkling);

    

		//sleep
		std::this_thread::sleep_for(std::chrono::microseconds(inklingSleepTime));

	}

	}




// TODO help me please, I have no direction
void getNewDirection(InklingInfo* inkling){

	//get random device
	std::random_device myRan;
	std::default_random_engine engine(myRan());
	//get random direction
	TravelDirection direction = static_cast<TravelDirection>(engine() % 4);

	inkling->dir = direction;

    
}

// TODO help me please, I am blank and need some code
bool checkIfInCorner(InklingInfo* inkling) {

	int row = inkling->row;
	int col = inkling->col;
	//Check if in corner
	if((row == 0 and col == 0) or (row == 0 and col == NUM_COLS - 1) or (row == NUM_ROWS - 1 and col == 0)){
		//kill the inkling
		return true;
	}

	if(row == NUM_ROWS - 1 and col == NUM_COLS - 1){
		return true;
	}

	return false;

}

// TODO check if you have enough ink depending on what kind of inkling you are, maybe some locks/unlocks
bool checkEnoughInk(InklingInfo* inkling, int* moveAmount) {

	//Red Inkling
	//lock red resource
	if(inkling->type == RED_TRAV){
		std::lock_guard<std::mutex> lock(redLock);
		if(redLevel >= *moveAmount){
			//if good move reserve the ink with lock
			if(validInklingMove(inkling, *moveAmount)){
				acquireRedInk(*moveAmount);
				return true;
			}
			
		}
		//if less ink than required, take rest of the ink
		*moveAmount = redLevel;
		if(validInklingMove(inkling, *moveAmount)){
			acquireRedInk(*moveAmount);
			return true;
		}
	
		return false;
	}

	if(inkling->type == GREEN_TRAV){
		std::lock_guard<std::mutex> lock(greenLock);
		//if good move reserve the ink with lock
		if(greenLevel >= *moveAmount){
			if(validInklingMove(inkling, *moveAmount)){
				acquireGreenInk(*moveAmount);
				return true;
			}
		}
		//if less ink than required, take rest of the ink
		*moveAmount = greenLevel;
		if(validInklingMove(inkling, *moveAmount)){
			acquireGreenInk(*moveAmount);
			return true;
		}
	
		
		return false;
	}

	if(inkling->type == BLUE_TRAV){
		std::lock_guard<std::mutex> lock(blueLock);
		//if good move reserve the ink with lock
		if(blueLevel >= *moveAmount){
			if(validInklingMove(inkling, *moveAmount)){
				acquireBlueInk(*moveAmount);
				return true;
			}
		}
		//if less ink than required, take rest of the ink
		*moveAmount = blueLevel;
		if(validInklingMove(inkling, *moveAmount)){
			acquireBlueInk(*moveAmount);
			return true;
		}
		
		moveAmount = 0;
		return false;
	}


	//raise error that there is no type
	printf("something wrong, no color type for this inkling");
	return false;


}

// TODO thread function for a red ink producer
void redColorThreadFunc() {
	//infinite loop that terminates with main event loop
	while(true){
		if(redLevel == 0){
			//refill red ink
			refillRedInk(REFILL_INK);
		}

	//sleep to prevent constant rechecks
	std::this_thread::sleep_for(std::chrono::microseconds(producerSleepTime));

	}
}

// TODO thread function for a green ink producer
void greenColorThreadFunc() {
	//infinite loop that terminates with main event loop
	while(true){
		if(greenLevel == 0){
			//refill green ink
			refillGreenInk(REFILL_INK);
		}

	std::this_thread::sleep_for(std::chrono::microseconds(producerSleepTime));

	}

}

// TODO thread function for a blue ink producer
void blueColorThreadFunc() {
	//infinite loop that terminates with main event loop
	while(true){
		if(blueLevel == 0){
			//refill blue ink
			refillBlueInk(REFILL_INK);
		}
	
	std::this_thread::sleep_for(std::chrono::microseconds(producerSleepTime));
	}

}







//-----------------------------------------------------------------------------
//MY PERSONEL HELPER FUNCTIONS
//-----------------------------------------------------------------------------


//Find unique cell position for an inkling(not shared by others)
bool uniqueStartPosition(int row, int col){
	
	//Traverse through info and see if row and col match another inkling
	for(long unsigned int inkling = 0; inkling < info.size(); inkling++){
		if(info.at(inkling).row == row and info.at(inkling).col == col){
			return false;
		}
	}

	//Check if in corner
	if((row == 0 and col == 0) or (row == 0 and col == NUM_COLS - 1) or (row == NUM_ROWS - 1 and col == 0)){
		return false;
	}

	//Simply a continutation as my IDE was bugging out.
	if(row == NUM_ROWS - 1 and col == NUM_COLS - 1){
		return false;
	}

	return true;
}

//Checks if move goes out of bounds
bool validInklingMove(InklingInfo* inkling, int moveAmount){
	//check if move is valid
	if(inkling->dir == NORTH){
		if(inkling->row - moveAmount < 0){
			return false;
		}
	}
	if(inkling->dir == WEST){
		if(inkling->col - moveAmount > NUM_COLS - 1){
			return false;
		}
	}
	if(inkling->dir == SOUTH){
		if(inkling->row + moveAmount > NUM_ROWS - 1){
			return false;
		}
	}
	if(inkling->dir == EAST){
		if(inkling->col + moveAmount < 0){
			return false;
		}
	}

	//If move does not go out of any bounds, we are good.
	return true;
}


//Move inkling and consume ink
void moveInkling(int** grid, InklingInfo* inkling, int moveAmount) {
	//Used to leave a trail on traversed cells
    int inklingColor = inkling->type;

	//checkEnoughInk consumes the resources, so at this point
	//our ink is reserved for this individual move.
	checkEnoughInk(inkling, &moveAmount);


    while (moveAmount > 0) {
        {	
			//lock the grid while moving
            std::lock_guard<std::mutex> lock(gridMutex);

            // Calculate new position
            int newRow = inkling->row;
            int newCol = inkling->col;
			//Move in direction inkling faces
            switch (inkling->dir) {
                case NORTH: newRow--; break;
                case SOUTH: newRow++; break;
                case EAST:  newCol++; break;
                case WEST:  newCol--; break;
                default: break;
            }

            // Check validity of move
            if (newRow < 0 || newRow >= NUM_ROWS || newCol < 0 || newCol >= NUM_COLS) {
                return;
            }

            // Update position and grid
            inkling->row = newRow;
            inkling->col = newCol;
            grid[inkling->row][inkling->col] = inklingColor;
        }
		//mutex is now released, so other inklings can move while we reset this one.

        moveAmount--;
        std::this_thread::sleep_for(std::chrono::microseconds(inklingSleepTime));
    }
}


void logThread(InklingInfo* inkling, int threadNum, bool firstLog){

    

	std::ofstream logFile = openDirAndFile(threadNum);

    // Get current time with milliseconds
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    auto timer = std::chrono::system_clock::to_time_t(now);
    std::tm bt = *std::localtime(&timer);
    
    std::ostringstream timestamp;
    timestamp << std::put_time(&bt, "%H:%M:%S") // HH:MM:SS
              << '.' << std::setfill('0') << std::setw(3) << ms.count(); // .milliseconds
    
    // Write timestamp to file
    logFile << timestamp.str() << ",";

	//Now we have the time so let's add the rest
	logFile << "inkling" << threadNum << ",";

	//if termination
	if(inkling->isLive == false){
		logFile << "terminated";
		return;
	}

	//add direction if not the initial log
	if(firstLog == false){
		switch(inkling->dir){
			case 0:
				logFile << "north" << ",";
				break;
			case 1:
				logFile << "west" << ",";
				break;
			case 2:
				logFile << "south" << ",";
				break;
			case 3:
				logFile << "east" << ",";
				break;
			default:
				break;
	}
}	//else add color rather than direction

	else{
		switch(inkling->type){
			case 0:
				logFile << "red" << ",";
				break;
			case 1:
				logFile << "green" << ",";
				break;
			case 2:
				logFile << "blue" << ",";
				break;
			default:
				break;
		}
	}

	//now we add row and col
	logFile << "row" << inkling->row << ",";
	logFile << "col" << inkling->col << "\n";
    
    logFile.close();
}


void clearLogFile(int threadNum){

    std::string dirPath = "logFolder";

    // Create directory in case it does not exist to avoid error
    mkdir(dirPath.c_str(), S_IRWXU);

    // Create the full filepath using the directory
    std::string filename = dirPath + "/inkling" + std::to_string(threadNum) + ".txt";
    std::ofstream logFile;

    // Trunc file to clear it
    logFile.open(filename, std::ios::trunc);

    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open or create log file: " << filename << std::endl;
        return;
    }

    logFile.close();

}


void terminationLog(InklingInfo* inkling, int threadNum){
	
	std::ofstream logFile = openDirAndFile(threadNum);

    // Get current time with milliseconds
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    auto timer = std::chrono::system_clock::to_time_t(now);
    std::tm bt = *std::localtime(&timer);

    std::ostringstream timestamp;
    timestamp << std::put_time(&bt, "%H:%M:%S") // HH:MM:SS
              << '.' << std::setfill('0') << std::setw(3) << ms.count(); // .milliseconds

    // Write terminated log if inkling was terminated by main event
	if(!wasLogTerminated(inkling, threadNum)){

		logFile << timestamp.str() << ",";

		//Now we have the time so let's add the rest
		logFile << "inkling" << threadNum << ",";
	
		logFile << "terminated";
		return;

	}
    
}

bool wasLogTerminated(InklingInfo* inkling, int threadNum){
	//open file and get last line

    std::string dirPath = "logFolder";
    mkdir(dirPath.c_str(), S_IRWXU);
    std::string filename = dirPath + "/inkling" + std::to_string(threadNum) + ".txt";
	
	//Just reading to file
    std::ifstream logFile(filename);
    std::ofstream logFile2(filename, std::ios::app);
	

    std::string line;
    std::string lastLine;
	// Loop until lastLine in logFile
    while (std::getline(logFile, line)) {
        lastLine = line;
    }

	//If terminated then we are good, else we will ad terminated log
    if (lastLine.find("terminated") != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

std::ofstream openDirAndFile(int threadNum){

	// Create the logFolder directory if it doesn't exist
    std::string dirPath = "logFolder";

    // Create directory with read/write/execute permissions for owner
    mkdir(dirPath.c_str(), S_IRWXU);

    // Create the full filepath using the directory
    std::string filename = dirPath + "/inkling" + std::to_string(threadNum) + ".txt";
    std::ofstream logFile;

    // Open file in append mode - will create if doesn't exist
    logFile.open(filename, std::ios::app);

	// Set permissions to 0755
    chmod(filename.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open or create log file: " << filename << std::endl;
        exit(1);
    }

	return std::move(logFile);

}