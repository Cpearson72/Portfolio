#include "text_search.h"
#include <chrono>


// This will never change
const char TextSearch::alpha_order[] = {

        '-', ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};


//alpabet(26) + ' ' + '-'
const unsigned int TextSearch::alpha_order_size = 38;

TextSearch::TextSearch() {}

TextSearch::TextSearch(std::string fname, bool info) {

    // store line
    std::string line;
    // subject (word in the trie)
    std::vector<std::string> word;
    // data
    std::vector<std::string> data;

    //open file
    std::ifstream myfile;
    myfile.open(fname);

    // get headers
    getline(myfile, line);
    split_line(line, word, data);
    this->subject = word.at(0);
    this->headers = data;


    // add words to trie
    int i = 0;
    while (getline(myfile, line)) {

        split_line(line, word, data);


        if (info) { // if there is node data
            this->insert_word(word.at(0), data);
        }
        else {
            this->insert_word(word.at(0));
        }
    }

    myfile.close();
}

TextSearch::~TextSearch() {

}

/* Splits line for paramerized constructor */
void TextSearch::split_line(std::string& line, std::vector<std::string>& vec, std::vector<std::string>& data) {

    if (line.size() == 0) {
        return;
    }
    line.push_back(' ');

    vec.clear();
    data.clear();

    std::string res = "";
    char letter;
    int count = 0;
    for (int i = 0; i < line.size(); i++) {

        letter = line.at(i);

        if (letter >= 'a' && letter <= 'z') { letter -= 32; }

        if (letter == ' ') {

            if (count == 0) {
                vec.push_back(res);
                count++;
            }
            else {
                data.push_back(res);
            }
            res.clear();
        }
        else if (letter == ',') {

            res.push_back(' ');
        }
        else {
            res.push_back( letter );
        }
    }
}

// displays words that are decendents of str
int TextSearch::display_descendents(std::string str, int order) {

    Node* start = search(str);

    if (!start) { return 0; }

    int count = 0;

    switch (order)
    {
        case 0: // count
            descendents(start, str, count, false);
            break;
        case 1: // count + unordered display
            descendents(start, str, count, true);
            break;
        case 2: // count + alphanumerical display
            descendents_alphanum(start, str, count);
            break;
        default:
            break;
    }

    return count;
}


void TextSearch::descendents_alphanum(Node* p, std::string& str, int& count) {

    if (p->isWord) {
        std::cout << count+1 << " - " << str << std::endl;
        citySearchResults.push_back(p);
        count++;
        // added
        // testing for possible pages on display (5 per page)
        // if((count-1) % 5 == 0){
        //     std::string keep_going = user_continue(count);
        // }
    }

    char letter;
    int i = 0;

    // check space, '-' , 0 - 9
    for ( ; i < 12; i++) {
        letter = this->alpha_order[i];
        if (p->next.count(letter)) {
            str.push_back(letter);
            descendents_alphanum(p->next.at(letter), str, count);
            str.pop_back();
        }
    }

    // check Aa - Zz
    for ( ; i < alpha_order_size; i++) {
        letter = this->alpha_order[i];
        if (p->next.count(letter) || p->next.count(letter + 32)) {
            str.push_back(letter);
            descendents_alphanum(p->next.at(letter), str, count);
            str.pop_back();
        }
    }
}

void TextSearch::add_city(){

    std::string addedCity;

    std::cout << "Enter a city you want to add: ";
    std::cin >> addedCity;

    insert_word(addedCity);

}

int TextSearch::starting_with(){

    std::string inputSearch;

    std::cout << "Search for city: ";
    std::cin >> inputSearch;

    // capitalizes string
    std::string capInputSearch = "";
    for(int i = 0; i < inputSearch.size(); i++){
        capInputSearch += std::toupper(inputSearch[i]);
    }

    int count = display_descendents(capInputSearch, 2);

    std::string dataChoice;
    int numCityChoice;

    if(count > 0){
        return count;
    }
    else{
        std::cout << "No matching search results" << std::endl;
    }

    std::cout << std::endl;

    std::vector<Node*> temp;
    citySearchResults = temp;

    return count;

}

void TextSearch::search_city(){

    std::string inputSearch;
    std::string capInputSearch;

    std::cout << "Enter a city you want to search for: ";
    std::cin >> inputSearch;
    for(int i = 0; i < inputSearch.size(); i++){
        capInputSearch += std::toupper(inputSearch[i]);
    }

    if(is_in(capInputSearch)){
        std::cout << capInputSearch << " is in data set" << std::endl;
    }
    else{
        std::cout << capInputSearch << " is not in data set" << std::endl;
    }

}

void TextSearch::remove_city(){

    std::string removedCity;

    std::cout << "Enter a city you want to delete: ";
    std::cin >> removedCity;

    delete_word(removedCity);

}

void TextSearch::descendents_input(){

    std::string input;

    std::cout << "Enter a city: ";
    std::cin >> input;

    display_descendents(input, 0);

}

void TextSearch::UI_main_menu(){

    std::cout << "Welcome to the City Search Program" << std::endl;
    std::cout << "-----------------------------------------" << std::endl << std::endl;

    int choice = UI_main_menu_choice();

    while(choice != 0){
        auto start = std::chrono::high_resolution_clock::now();
        if(choice == 1){
            int count = starting_with();
            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
            std::cout << "Time taken: " << duration.count() << std::endl;
            if(count > 0){
                UI_node_data_menu(count);
            }
        }
        else if(choice == 2){
            search_city();
        }
        else if(choice == 3){
            add_city();
        }
        else if(choice == 4){
            remove_city();
        }
        else{
            std::cout << "Error - Invalid Entry: Displaying choices again"<< std::endl;
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        choice = UI_main_menu_choice();
    }

}

int TextSearch::UI_main_menu_choice(){

    std::cout << "Main Menu" << std::endl;
    std::cout << "---------------" << std::endl;
    std::cout << "1 - Search for city (starting_with)- almost complete" << std::endl;
    std::cout << "2 - See if city is in data set (is_in) - partial" << std::endl;
    std::cout << "3 - Add city - not done" << std::endl;
    std::cout << "4 - Delete city - not done" << std::endl;
    std::cout << "5 - Write file - not done" << std::endl;
    std::cout << "0 - End" << std::endl << std::endl;

    int choice = 0;

    std::cout << "Choice: ";

    std::cin >> choice;

    return choice;

}

void TextSearch::UI_node_data_menu(int count){

    std::string dataChoice;

    std:: cout << std::endl;
    std:: cout << "Choices: " << std::endl;
    std:: cout << "1 - Get data for specific city" << std::endl;
    std:: cout << "0 - Back to Main Menu" << std::endl;
    std:: cout << "Choice: ";

    std::cin >> dataChoice;

    int numCityChoice;

    while(dataChoice != "0"){
        if(dataChoice == "1"){
            std::cout << std::endl;
            std::cout << "Enter number for city (must be integer): ";
            std::cin >> numCityChoice;
            while(numCityChoice < 1 || numCityChoice > count){
                std::cout << "Error - invalid entry" << std::endl;
                std::cout << "Enter number for city: ";
                std::cin >> numCityChoice;
            }

            citySearchResults[numCityChoice-1]->display_node_data();

            std:: cout <<  std::endl;
            std:: cout << "Choices: " << std::endl;
            std:: cout << "1 - Get data for specific city" << std::endl;
            std:: cout << "0 - Back to Main Menu" << std::endl;
            std:: cout << "Choice: ";

            std::cin >> dataChoice;

        }
        else{
            std::cout << "Error - Invalid Entry" << std::endl;
            std::cout << "Enter another choice: ";
            std:: cin >> dataChoice;
        }
    }

}

// if we decide to add pages
// std::string TextSearch::user_continue(int count){

//     int pageNum;

//     if((count-1) % 5 == 0){
//         pageNum = (count-1) / 5;
//     }
//     else{
//         pageNum = ((count-1) / 5) + 1;
//     }

//     std::cout << std::endl;
//     std::cout << "Page: " << pageNum << std::endl;
//     std::string input = "";

//     if(pageNum ==  1){
//         std::cout << "2 - Next Page" << std::endl;
//         std::cout << "0 - Back" << std::endl << std::endl;
//         std:: cout << "Choice: ";

//         std::cin >> input;

//         while(input != "0" && input != "2"){
//             std:: cout << "Error - Invalid entry" << std::endl;
//             std:: cout << "Choice: ";
//             std::cin >> input;
//         }

//     }
//     else{
//         std::cout << "1 - Previous Page" << std::endl;
//         std::cout << "2 - Next Page" << std::endl;
//         std::cout << "0 - Back" << std::endl << std::endl;
//         std::cout << "Choice: ";

//         while(input != "0" && input != "1" && input != "2"){
//             std:: cout << "Error - Invalid entry" << std::endl;
//             std:: cout << "Choice: ";
//             std::cin >> input;
//         }
//     }

//     return input;

// }