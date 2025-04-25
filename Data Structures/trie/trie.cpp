#include "trie.h"


// constructor
Trie::Trie() {
    this->root = new Node();
    this->num_of_words = 0;
}

// node descendents deletion handeled within node destructor
Trie::~Trie() {delete this->root;}

// if letter exists go to node, else make new node (This needs to change bc of node data)
// TO DO: disallow repeats and add data to node
void Trie::insert_word(std::string& str) {

    Node* tmp = this->root;
    char letter;

    for (int i = 0; i < str.size(); i++) {
        letter = str.at(i);
        // std::cout << letter;
        if (!tmp->next.count(letter)) {
            tmp->next.emplace(letter, new Node(tmp));
        }
        tmp = tmp->next.at(letter);
    }

    //std::cout << std::endl;
    tmp->isWord = true;
    tmp->word_count++;
    this->num_of_words++;
    return;
}


void Trie::insert_word(std::string& str, std::vector<std::string> data)  {

    Node* tmp = this->root;
    char letter;

    for (int i = 0; i < str.size(); i++) {

        letter = str.at(i);
        // std::cout << letter;
        if (!tmp->next.count(letter)) {
            tmp->next.emplace(letter, new Node(tmp));
        }
        tmp = tmp->next.at(letter);
    }

    // std::cout << std::endl;

    // this is not good practice
    if (tmp->isWord == true) {

        // add a dash
        if (!tmp->next.count('-')) {
            tmp->next.emplace('-', new Node(tmp));
        }
        tmp = tmp->next.at('-');

        // find next valid number
        int count = 2;
        std::string vers = std::to_string(count);
        while (search(vers, tmp) != nullptr) {
            count++;
            vers = std::to_string(count);
        }

        for (int i = 0; i < vers.size(); i++) {

            letter = vers.at(i);
            if (!tmp->next.count(letter)) {
                tmp->next.emplace(letter, new Node(tmp));
            }
            tmp = tmp->next.at(letter);
        }
    }


    tmp->add_node_data(data);
    tmp->isWord = true;
    tmp->word_count++;
    this->num_of_words++;
    return;
}

Node* Trie::search(std::string& str, Node* p) {

    Node* tmp = p;
    char letter;

    for (int i = 0; i < str.size(); i++) {
        letter = str.at(i);
        if (!tmp->next.count(letter)) {
            return nullptr;
        }
        tmp = tmp->next.at(letter);
    }

    return tmp;
}

//returns node pointer of last character
//nullptr == node does not exist
Node* Trie::search(std::string& str) {

    Node* tmp = this->root;
    char letter;

    for (int i = 0; i < str.size(); i++) {
        letter = str.at(i);
        if (!tmp->next.count(letter)) {
            return nullptr;
        }
        tmp = tmp->next.at(letter);
    }

    return tmp;
}

// true if str is in trie, else false
bool Trie::is_in(std::string& str) {

    Node* tmp = search(str);

    if (tmp && tmp->isWord) {
        return true;
    }

    return false;
}

// removes word and applicable nodes
void Trie::delete_word(std::string &str){

    Node* tmp = search(str);
    if (tmp == nullptr || !tmp->isWord) {
        std::cout << str << ": is not in trie." << std::endl;
        return;
    }

    tmp->word_count--;
    if (!tmp->word_count) {tmp->isWord = false;} // if last instance of word is deleted isWord boolean must become false

    if (!tmp->next.empty() || tmp->isWord) { // deleted word is a branch or deleted word has more than one instance
        //std::cout << str << " is a 'branch', or multiple one instance deleted." << std::endl;
        return;
    }

    // must delete all nodes that only lead to word
    int i = str.size();
    while (tmp->parent != nullptr && tmp->next.size() <= 1 && tmp->word_count == 0) {
        tmp = tmp->parent;
        i--;
    }


    /* std::cout << str << ", is a 'leaf' with only one instance, node deleted." << std::endl;
    std::cout << str.at(i) << std::endl;
    std::cout << i << std::endl; */

    delete tmp->next.at(str.at(i));
    tmp->next.erase(str.at(i));

    return;
}

// returns number of words in trie
unsigned int Trie::size() { return this->num_of_words; }

// display unordered + count
// display unordered + count
void Trie::descendents(Node* p, std::string& str, int& count, bool display) {

    if (p->isWord) {
        std::cout << str << std::endl;
        count++;
    }

    char letter;
    int i = 0;

    // check space, '-' , 0 - 9
    for (auto i = p->next.begin(); i != p->next.end(); i++) {
        letter = i->first;
        if (p->next.count(letter)) {
            str.push_back(letter);
            descendents(p->next.at(letter), str, count, false);
            str.pop_back();
        }
    }

}

int Trie::display_descendents_unordered(std::string str) {

    Node* begin = search(str);

    if(!begin){
        return 0;
    }

    int count = 0;
    bool display = false;

    descendents(begin, str, count, display);

    return count;
}
