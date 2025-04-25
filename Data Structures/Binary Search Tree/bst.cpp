//
// Created by pears on 3/22/2023.
//
#include "bst.h"
#include <iostream>

//Default constructor
BSTree::BSTree() {

    root = new BSTnode();
    root = NULL;

}

BSTree::BSTree(int data) {

    root = new BSTnode(data);
}
//Code for insertion
void BSTree::insert(BSTnode *node, BSTnode* prevNode, int prev, int data) {

    //If the data we are inserting is greater than the current
    //node's data we return to the right node
    if(node != NULL){
        if(node->data < data){

            //prev is = 1 for right and 0 for left
            return insert(node->right, node, 1, data);

        }

            //For the other way, is the data less than the nodes data
        else if(node->data > data){

            return insert(node->left, node, 0, data);

        }
    }

    else if(node == NULL and node == root){
        root = new BSTnode(data);
        root->left = nullptr;
        root->right = nullptr;
    }

    //If the current node has no children, we have reached our destination
    else{
        node = new BSTnode(data);
        if(prev == 0){
            prevNode->left = node;
        }
        else{
            prevNode->right = node;
        }
    }

}

void BSTree::insert(int data) {

    insert(root, root, 0, data);

}

//Code for removal
//prevDirection 0 is left, and 1 is right
void BSTree::remove(BSTnode *node, BSTnode *prev, int data) {

    //If we have reached the node and it is a leaf, we just delete the node
    //**NO CHILD**
    if (node == NULL) {

        delete node;
        //We must account for the pointers
        if (data > prev->data) {
            prev->right = nullptr;
        } else {

            prev->left = nullptr;
        }
    }
        //**ROOT NODE WITH NO CHILDREN**
        else if(data == root->data and root->left == nullptr and root->right == nullptr){
            root = NULL;
        }

        //**ROOT WITH ONE CHILD**
        else if(data == root->data and root->left != nullptr and root->right == nullptr or data == root->data and root->right != nullptr and root->left ==
                                                                                                                                                     nullptr){

            BSTnode* tmp;
            tmp = root;
            //Means there is one child to the right
            if(root->right != nullptr){
                root = root->right;
                delete tmp;
            }
            //Child to the left
            else{
                root = root->left;
                delete tmp;
            }
        }

        //ROOT WITH TWO CHILDREN
        else if(data == root->data and root->left != nullptr and root->right != nullptr){

        BSTnode *travelNode = node;
        travelNode = root->right;
        if (travelNode->left != nullptr) {
            while (travelNode->left->left != nullptr) {
                travelNode = travelNode->left;
            }

            node->data = travelNode->left->data;
            if(travelNode->left->right != nullptr){
                travelNode->left = travelNode->left->right;
                delete travelNode->left->right;
            }
            else {
                delete travelNode->left;
                travelNode->left = nullptr;
            }
        } else {
            node->data = travelNode->data;
            BSTnode *tmp = travelNode;
            node->right = travelNode->right;
            delete travelNode;
        }



        }

        //**ONE CHILD**
        //Checks to see if we are at the right node, and if the left or right pointers are null, meaning it
        //has one child. If it had two nulls it would have passed above, so no need to worry about the other
        //being a null as well.
    else if (node->data == data and node->right == nullptr or node->data == data and node->left == nullptr) {

        //We set the left child as the new node
        if (node->right == nullptr) {

            //If the node we are deleting came from its parents right side
            if (prev->right == node) {

                //Sets the new right
                prev->right = node->left;

                //deletes the node
                delete node;
            } else {

                //otherway
                prev->left = node->left;

                delete node;
            }


        }
            //Else if it is the nodes right side with the child
        else {

            if (prev->right == node) {

                //Sets the new right
                prev->right = node->right;

                //deletes the node
                delete node;
            } else {

                //otherway
                prev->left = node->right;

                delete node;
            }


        }
    }

        //**TWO CHILDREN**
        //This is the most complicated part
    else if (node->data == data and node->right != nullptr and node->left != nullptr) {

        //Get the smallest value in the right subtree of the node
        //if (node->data < root->data) {
            BSTnode *travelNode = node;
            travelNode = node->right;
            if (travelNode->left != nullptr) {
                while (travelNode->left->left != nullptr) {
                    travelNode = travelNode->left;
                }

                node->data = travelNode->left->data;
                if(travelNode->left->right != nullptr){
                    travelNode->left = travelNode->left->right;
                    delete travelNode->left->right;
                }
                else {
                    delete travelNode->left;
                    travelNode->left = nullptr;
                }
            } else {
                node->data = travelNode->data;
                BSTnode *tmp = travelNode;
                node->right = travelNode->right;
                delete travelNode;
            }
        //} else {
         /*

            BSTnode *travelNode = node;
            travelNode = node->left;
            if (travelNode->right != nullptr) {
                while (travelNode->right->right != nullptr) {
                    travelNode = travelNode->right;
                }

                node->data = travelNode->right->data;
                if(travelNode->right != nullptr){
                    travelNode = travelNode->right;
                    delete travelNode->right;
                }
                else {
                    delete travelNode->right;
                    travelNode->right = nullptr;
                }
            } else {
                node->data = travelNode->data;
                BSTnode *tmp = travelNode;
                node->left = travelNode->left;
                delete travelNode;
            }

        }
    */


    }


    else{

        if(data > node->data) {
            return remove(node->right, node, data);
        }
        else{
            return remove(node->left, node, data);
        }
    }


}

void BSTree::remove(int data) {

    remove(root, root, data);

}

//**NOW WE ARE ON TO TRAVERSAL**

void BSTree::preorder(BSTnode* node) {

    if(node == NULL){
        return;
    }

    std::cout << node->data << " ";
    preorder(node->left);
    preorder(node->right);



}
void BSTree::preorder(){

    preorder(this->root);

}

void BSTree::inorder(BSTnode *node) {

    if(node == NULL){
        return;
    }

    inorder(node->left);
    std::cout << node->data << " ";
    inorder(node->right);


}

void BSTree::inorder() {

    inorder(root);
}

void BSTree::postorder(BSTnode* node) {

    if(node == NULL){
        return;
    }

    postorder(node->left);
    postorder(node->right);
    std::cout << node->data << " ";

}

void BSTree::postorder() {

    postorder(root);
}

//Now for height
int BSTree::height(BSTnode* node){

    if(node == NULL){
        return 0;
    }

    int num = height(node->left);
    int num2 = height(node->right);
    if(num > num2){
        return num + 1;
    }
    else{
        return num2 + 1;
    }



}

int BSTree::height() {

    int num = height(root);
    std::cout << num << std::endl;

}


void BSTree::destroy(BSTnode* node) {

    if(node == NULL){
        return;
    }
    destroy(node->left);
    destroy(node->right);
    if(node == root){
        root = NULL;
        return;
    }
    else {
        delete node;
    }



}

void BSTree::destroy() {

    destroy(root);

}

bool BSTree::search(BSTnode* node, int data) {

    if(node == NULL){
        return false;
    }
    if(node->data == data){
        //std::cout << "Found " << data << std::endl;
        return true;
    }
    bool left = search(node->left, data);
    bool right = search(node->right, data);
    if(left == true or right == true){
        return true;
    }
    else{
        return false;
    }


}

bool BSTree::search(int data) {

    return search(root, data);

}

BSTree::~BSTree() {

    destroy(root);

}


BSTnode::BSTnode() {

    this->data = NULL;
    this->left = nullptr;
    this->right = nullptr;
}

BSTnode::BSTnode(int data) {

    this->data = data;
    this->right = nullptr;
    this->left = nullptr;

}

BSTnode::~BSTnode() {


}

