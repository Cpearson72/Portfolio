#ifndef _NODE_H_
#define _NODE_H_
#include "node_data.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>


class Node{
private:

    bool isWord;
    //0 =
    std::vector<std::string> info;
    unsigned int word_count;

    // need a data structure to allow multiple node_data in case of repeats
    NodeData* data = nullptr;

    // to backtrack for delete
    Node* parent;

    // can store any character
    std::unordered_map<char, Node*> next;

    // make new NodeData (WE CAN NOT ALLOW REPEATS)
    void add_node_data(std::vector<std::string> dt);

    friend class Trie;
    friend class TextSearch;

public:

    // the name of the function (Zip codes are very long so maybe have it be an extra choice???)
    void display_node_data();

    Node();
    Node(Node* p);
    void new_node();
    ~Node();
};
#endif