//
// Created by pears on 4/5/2023.
//
#include <iostream>

#include "hash_table.h"

//**Both Constructor Functions**
HashTable::HashTable() {
    this->sizeOfTable = 1;
    this->numOfCollisions = 0;
    this->numOfEntries = 0;
    this->currentInsertMode = 0;
}

HashTable::HashTable(unsigned int currentInsertMode) {
    this->currentInsertMode = currentInsertMode;
    this->sizeOfTable = 1;
    this->numOfEntries = 0;
    this->numOfCollisions = 0;
}

//**Helper Functions**

int HashTable::get_hash_value(int data) {

    return (data % this->sizeOfTable);
}

int HashTable::get_hash2_value(int data) {

    return ((data * data) % this->sizeOfTable);
}
int HashTable::get_double_hashing_value(int data) {
    int collision = 0;
    int dblHash = 0;
    bool placedVal = false;
    while(placedVal == false) {
        dblHash = (get_hash_value(data) + (collision + get_hash2_value(data))) % this->sizeOfTable;
        if(is_there_collision(dblHash)){
            collision++;
        }
        else{
            placedVal = true;
        }
    }
    return dblHash;

}

bool HashTable::is_there_collision(int index) {

    if(this->tableArray.at(index) == 0){
        return false;
    }
    this->numOfCollisions++;
    return true;
}

bool HashTable::is_at_max_capacity() {
    double numOfEntries = this->numOfEntries;
    double sizeOfTable = this->sizeOfTable;

    double percentage = (numOfEntries/sizeOfTable) * 100;
    if(percentage >= 70){
        return true;
    }
    return false;
}


void HashTable::resize_1D_table() {

    //Holds the old table and sets current table
    std::vector<int> oldTable = this->tableArray;
    this->tableArray = std::vector<int>(this->sizeOfTable * 2, 0);
    this->numOfEntries = 0;
    this->sizeOfTable = this->tableArray.size();

    for(int i = 0; i < oldTable.size(); i++){

        if(oldTable.at(i) != 0){
            insert(oldTable.at(i));
        }
    }


}

void HashTable::resize_2D_table() {

    std::vector<std::vector<int>> old2DTable = this->chainingArray;
    this->chainingArray = std::vector<std::vector<int>>(this->chainingArray.size() * 2, std::vector<int>{0});
    this->numOfEntries = 0;
    this->sizeOfTable = this->sizeOfTable * 2;

    for(int i = 0; i < old2DTable.size(); i++){

        if(old2DTable[i][0] != 0){

            for(int j = 0; j < old2DTable[i].size(); j++){
                insert(old2DTable[i][j]);
            }
        }
    }



}

void HashTable::resize_table() {

    if(this->currentInsertMode != 2){
        resize_1D_table();
    }
    else{
        resize_2D_table();
    }
}


//**Insert Methods**

void HashTable::linear_probing(int data) {

    int hashVal = get_hash_value(data);
    bool placedValue = false;
    while(placedValue == false) {
        if (is_there_collision(hashVal)) {
            hashVal++;
            if(hashVal >= sizeOfTable){
                hashVal = 0;
            }
        }
        else{
            this->tableArray.at(hashVal) = data;
            this->numOfEntries++;
            placedValue = true;
        }
    }
    //After inserting checks to see if it is at capacity
}

void HashTable::double_hashing(int data) {

    int hashVal = get_double_hashing_value(data);
    this->tableArray.at(hashVal) = data;
    this->numOfEntries++;



}

void HashTable::chaining(int data) {

    int hashVal = get_hash_value(data);
    //Since we are using pushback, we check to see
    //if the value at this index's vector is 0
    if(this->chainingArray[hashVal][0] == 0){
        //Sets the zero to the data
        this->chainingArray[hashVal][0] = data;
    }

    //there is a value at this vector, so pushes back.
    else{
        this->numOfCollisions++;
        this->chainingArray[hashVal].push_back(data);
    }

    this->numOfEntries++;


}

void HashTable::insert(int data) {

    if(this->currentInsertMode == 0){
        linear_probing(data);
    }
    else if(this->currentInsertMode == 1){
        double_hashing(data);
    }
    else if(this->currentInsertMode == 2){
        chaining(data);
    }
}

void HashTable::print_table() {

    if(this->currentInsertMode == 0 or this->currentInsertMode == 1){
        print_1D_table();
    }
    else{
        print_2D_table();
    }


}
void HashTable::print_1D_table() {

    for(int i = 0; i < this->sizeOfTable; i++){

        if(this->tableArray.at(i) != 0) {
            std::cout << i << ": " << this->tableArray.at(i) << std::endl;
        }

        else{
            std::cout << i << ": 0" << std::endl;
        }
    }

}
void HashTable::print_2D_table() {

    for(int i = 0; i < this->chainingArray.size(); i++){
        std::cout << i << ": ";
        //Starts at beginning of this index's vector
        for(int j = 0; j < this->chainingArray[i].size(); j++){

            if(this->chainingArray[i][j] != 0) {
                std::cout << this->chainingArray[i][j] << " ";
            }

        }
        std::cout << std::endl;
    }
}

unsigned int HashTable::get_num_collisions() {

    return this->numOfCollisions;
}