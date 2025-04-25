#include "node.h"


Node::Node() {
    new_node();
    parent = nullptr;
}

Node::Node(Node* p) {
    new_node();
    parent = p;
}

//delete nodes in map
Node::~Node() {

    std::unordered_map<char, Node*>::iterator it;
    it = next.begin();
    while (!this->next.empty()) {
        //std::cout << "Deleting " << (*it).first << std::endl;
        delete (*it).second;
        next.erase(next.begin());
        it = next.begin();
    }

    if (this->data != nullptr) {
        delete this->data;
    }

}

void Node::new_node() {
    isWord = false;
    word_count = 0;
}

void Node::add_node_data(std::vector<std::string> dt){
    NodeData* temp = new NodeData;

    temp->data = dt;

    this->data = temp;

}

void Node::display_node_data() {

    /*

    Choices Key:

        1 = County
        2 = Population
        3 = Pop Dens
        4 = County & Population
        5 = County & Pop Dens
        6 = Pop & Pop Dens
        7 = All

    */

    int choice = 7;

    // County
    if(choice == 1 || choice == 4 || choice == 5 || choice == 7){
        std::cout << "County Name: " << data->data[0] << std::endl;
    }
    // Population
    if(choice == 2 || choice == 4 || choice == 6 || choice == 7){
        std::cout << "Population: " << data->data[1] << std::endl;
    }
    // Population Density
    if(choice == 3 || choice == 5 || choice == 6 || choice == 7){
        std::cout << "Population Density: " << data->data[2] << std::endl;
    }



}