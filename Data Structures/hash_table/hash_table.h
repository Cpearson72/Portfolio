//
// Created by pears on 4/5/2023.
//
#include <iostream>
#include <vector>

#ifndef LAB_10_HASH_TABLE_H
#define LAB_10_HASH_TABLE_H


class HashTable {
    private:
        std::vector<int> tableArray{0};
        std::vector<std::vector<int>> chainingArray{{0}};
        unsigned int sizeOfTable;
        unsigned int numOfEntries;
        unsigned int currentInsertMode;
        unsigned int numOfCollisions;

        //**Insert methods**
        void linear_probing(int data);
        void double_hashing(int data);
        void chaining(int data);

    public:
        HashTable();
        HashTable(unsigned int currentInsertMode);
        //Helper function if table is at max capacity(over 70%) and needs to be
        //resized.
        bool is_at_max_capacity();
        //Function to resize table
        void resize_table();
        void resize_1D_table();
        void resize_2D_table();
        //Hash function
        int get_hash_value(int data);
        int get_hash2_value(int data);
        int get_double_hashing_value(int data);
        //Function that will insert data
        void insert(int data);
        //Function to test if there will be collision
        bool is_there_collision(int index);
        //Print table
        void print_table();
        unsigned int get_num_collisions();
        //If we are chaining, this turns our table vector
        //into a 2d one.
        void print_1D_table();
        void print_2D_table();


};


#endif //LAB_10_HASH_TABLE_H
