#ifndef _TEXT_SEARCH_H_
#define _TEXT_SEARCH_H_
#include "trie.h"
#include <fstream>
#include <sstream>


class TextSearch : public Trie {

private:

    const static char alpha_order[];
    const static unsigned int alpha_order_size;

    // word type
    std::string subject;

    // headers for node data
    std::vector<std::string> headers;

    std::vector<Node*> citySearchResults;

    /* get descendents */
    //int display_descendents(std::string str, int order);

    /*
    Display Descendents Alphanumerically

    recursivly look for words(cities) in alphabetical order
    time complexity O(n*k) where n is the size of alpha_order
    & k is the height of Trie starting at the node of first function call
    function is here since alphanumerical order is a special case
    that will not work for every version of a trie
    */
    void descendents_alphanum(Node* p, std::string& str, int& count);

    void split_line(std::string& line, std::vector<std::string>& vec, std::vector<std::string>& data);

public:

    TextSearch();
    TextSearch(std::string fname, bool info);
    ~TextSearch();

    void UI_main_menu();

    int UI_main_menu_choice();

    // ask for input call upon display_descendents()
    int starting_with();

    // ask for input, call delete_word(str)
    void remove_city();

    // ask for input, call insert(str)
    void add_city();

    // ask for input, call is_in(str)
    void search_city();

    // writes changes to file
    void write_file();

    /* ask for input, call display_descendents */
    void descendents_input();

    /* get descendents */
    int display_descendents(std::string str, int order);

    /*asks user if they want city data displayed*/
    void UI_node_data_menu(int count);
};
#endif