#include "text_search.h"
#include <iostream>
#include <string>

/*
g++ -std=c++17 node.cpp trie.cpp  text_search.cpp main.cpp -o test && ./test us_cities_info.txt
*/

int main(int argc, char* argv[]) {

    std::string fname(argv[1]);
    std::cout << std::endl << "File Name: "<< fname << std::endl << std::endl << std::endl;
    TextSearch db(fname, true);
    std::string str = argv[2];

    db.UI_main_menu();


    // std::string str = "LINCOLN";

    // std::cout << "Word Descendents: " << db.display_descendents(str, 2) << std::endl;
    // std::cout << std::endl;
    // std::cout << "Word Descendents: " << db.display_descendents(str, 1) << std::endl;

    // std::string bee = "ARTIODACTYLA ZIPHIIDAE MESOPLODON TRAVERSII";

    // db.delete_word(str);

    // if (db.is_in(bee)) {
    //     std::cout << "Yes1" << std::endl;
    // }


    // bee = "MIDWAY FL-2";
    // if (db.is_in(bee)) {
    //     std::cout << "Yes2" << std::endl;
    // }

    return 0;
}