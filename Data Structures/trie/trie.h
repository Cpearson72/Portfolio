#ifndef _TRIE_H_
#define _TRIE_H_
#include "node.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class Trie{

protected:
    Node* root;
    unsigned int num_of_words;

    /* Returns node of last char of string, if not exist return nullptr */
    Node* search(std::string& str);

    /* Returns node of last char of string, if not exist return nullptr, starts at node passed through */
    Node* search(std::string& str, Node* p);

    /* count + display (I took the other descendents off because its best if we do NOT allow multiple instances of the same word)*/
    void descendents(Node* p, std::string& str, int& count, bool display);

    friend class TextSearch;


public:

    Trie();
    ~Trie();


    /* Adds string to trie */
    void insert_word(std::string& str);

    /* Adds string to trie & data to node */
    void insert_word(std::string& str, std::vector<std::string> data);

    /* Removes string from trie */
    void delete_word(std::string& str);

    /* Returns true if string is in trie */
    bool is_in(std::string& str);

    /* Return number of words in trie */
    unsigned int size();

    int display_descendents_unordered(std::string str);



};
#endif