//
// Created by pears on 3/22/2023.
//

#ifndef LAB_08_BST_H
#define LAB_08_BST_H

class BSTnode{
    private:
        BSTnode* right;
        BSTnode* left;
        int data;
        friend class BSTree;
    public:
        BSTnode(int data);
        BSTnode();
        ~BSTnode();


};

class BSTree{
private:

    BSTnode* root;
    void destroy(BSTnode* node);
    void insert(BSTnode* node, BSTnode* prevNode, int prev, int data);
    void remove(BSTnode* node, BSTnode* prev, int data);
    //Traversals
    void preorder(BSTnode* node);
    void inorder(BSTnode* node);
    void postorder(BSTnode* node);
    int height(BSTnode* node);
    bool search(BSTnode* node, int data);
    friend class BSTnode;
public:

    BSTree(int data);
    BSTree();
    ~BSTree();
    void insert(int data);
    void preorder();
    void inorder();
    void postorder();
    int height();
    void remove(int data);
    void destroy();
    bool search(int data);


};

#endif //LAB_08_BST_H
