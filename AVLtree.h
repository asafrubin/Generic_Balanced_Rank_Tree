#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <iostream>
#include <stdlib.h>
#include "AVLnode.h"

using namespace std;

template <class DataType, class CompareFunction, class SumFunction>
class AVLtree{
private:
    SumFunction sumFunction;
    CompareFunction compare;
    AVLnode<DataType, SumFunction>* root;
    AVLnode<DataType, SumFunction>* iterator;
    int size;
public:

    AVLtree() : root(NULL), iterator(NULL), size(0) {}
    ~AVLtree(){
        destroyTree(root);
    }

     AVLnode<DataType, SumFunction>* find(DataType data){
        int result;
        iterator = root;
        //AVLnode<DataType, SumFunction> *temp;
        while(iterator){
            result = compare(iterator->getData(), data);
            //temp = iterator;
            if(result == 0){
                return iterator;
            }
            else if(result < 0){
                iterator = iterator->getLChild();
                continue;
            } else
                iterator = iterator->getRChild();
        }
        return NULL;
    }

    void insert(DataType new_data) {
            if (root == NULL) {
                AVLnode<DataType, SumFunction> *new_node = new AVLnode<DataType, SumFunction>(new_data);
                root = new_node;
                root->setHight(1);
                root->setRank(1);
                size++;
                return;
            }
            if (find(new_data) != NULL)
                return;

            AVLnode<DataType, SumFunction> *new_node = new AVLnode<DataType,SumFunction>(new_data);

            rec_insert(root, new_node);
            int tmp_hight = root->getHight();
            root->calcHight();
            root->calcRank();
            root->calcSum();
            root->setBF();
            root->rotate(tmp_hight);

            size++;
    }


    void rec_insert(AVLnode<DataType,SumFunction> *parent, AVLnode<DataType,SumFunction> *new_node){
        //if parent is smaller than new_node go to right child
        int tmp_hight = parent->getHight();
        AVLnode<DataType, SumFunction> *new_root;
        if(compare(parent->getData(), new_node->getData()) > 0){
            if(parent->getRChild() == NULL){
                parent->setRChild(new_node);
                new_node->setParent(parent);
                parent->getRChild()->calcHight();
                parent->getRChild()->calcRank();
                parent->getRChild()->calcSum();
                parent->getRChild()->setBF();
                parent->calcHight();
                parent->calcRank();
                parent->calcSum();
                parent->setBF();
                parent->getRChild()->rotate(tmp_hight);
            }
            else
                rec_insert(parent->getRChild(), new_node);
        }
        else if(parent->getLChild() == NULL){
            parent->setLChild(new_node);
            new_node->setParent(parent);
            parent->getLChild()->calcHight();
            parent->getLChild()->calcRank();
            parent->getLChild()->calcSum();
            parent->getLChild()->setBF();
            parent->calcHight();
            parent->calcRank();
            parent->calcSum();
            parent->setBF();
            parent->getLChild()->rotate(tmp_hight);
        }
        else
            rec_insert(parent->getLChild(), new_node);

        parent->calcHight();
        parent->calcRank();
        parent->calcSum();
        parent->setBF();
        new_root = parent->rotate(tmp_hight);
        parent->calcRank();
        parent->calcSum();
        if(new_root){
            root = new_root;
        }
    }

    void remove1(DataType data) {
        AVLnode<DataType, SumFunction> *node = find(data);
        AVLnode<DataType, SumFunction> *new_root, *it, *temp, *parent;
        if (!node)
            return;
        if (!node->getParent() && !node->getLChild() && !node->getRChild()) {
            delete node;
            root = NULL;
            return;
        }
        parent = node->getParent();
        //node is a leaf
        if (!node->getLChild() && !node->getRChild()) {
            if (node->getParent()->getLChild() == node)
                node->getParent()->setLChild(NULL);
            else
                node->getParent()->setRChild(NULL);
            delete node;
            it = parent;
        }

        //node has 1 child
        else if(node->getLChild() && !node->getRChild()){
            swap2data(node, node->getLChild());
            delete node->getLChild();
            node->setLChild(NULL);
        }
        else if(node->getRChild() && !node->getLChild()){
            swap2data(node, node->getRChild());
            delete node->getRChild();
            node->setRChild(NULL);
        }

        //node has 2 children
        else if (node->getLChild() && node->getRChild()) {
            it = node->getRChild();
            temp = it;
            it = findMin(it);
            swap2data(node, it);
            if (!it->getRChild()) {
                if(it->getParent()->getRChild() == it)
                    it->getParent()->setRChild(NULL);
                else
                    it->getParent()->setLChild(NULL);
                delete it;
                it=temp;
            }
            else {
                swap2data(it, it->getRChild());
                delete it->getRChild();
                it->setRChild(NULL);
            }
        }
        if(parent== NULL){
            parent = it;
        }
        //calculate bf rotate
        while(it != NULL){
            it->calcHight();
            it->setBF();
            new_root = it->rotate(-1);
            if(new_root){
                root = new_root;
            }
            it = it->getParent();
        }
        size--;
    }

    AVLnode<DataType, SumFunction>* removeNoChilds(AVLnode<DataType, SumFunction>* node){
        //if node is root
        AVLnode<DataType, SumFunction>* parent = node->getParent();
        if(node->getParent() == NULL){
            delete node;
            this->root = NULL;
            return NULL;
        }
        if(node->getParent()->getRChild() == node)
            node->getParent()->setRChild(NULL);
        else
            node->getParent()->setLChild(NULL);
        delete node;
        return parent;
    }

    AVLnode<DataType, SumFunction>* removeOneChild(AVLnode<DataType, SumFunction>* node){
        AVLnode<DataType, SumFunction>* tmp, *tmp_parent;
        //case removing root
        tmp_parent = node->getParent();
        if(!node->getParent()){
            if(!node->getRChild()){
                root = node->getLChild();
            } else
                root = node->getRChild();
            root->setParent(NULL);
            delete node;
            return root;
        }
        if(!node->getRChild())
            tmp = node->getLChild();
        else
            tmp = node->getRChild();

        if(node->getParent()->getRChild() == node)
            node->getParent()->setRChild(tmp);
        else
            node->getParent()->setLChild(tmp);
        tmp->setParent(tmp_parent);
        delete node;
        return tmp_parent;
    }
    AVLnode<DataType, SumFunction>* removeTwoChildren(AVLnode<DataType, SumFunction>* node){
        AVLnode<DataType, SumFunction>* min_node = findMin(node->getRChild());
        swap2data(node, min_node);
        if(min_node->getRChild())
            return removeOneChild(min_node);
        else
            return removeNoChilds(min_node);
    }

    int operator[](int k){
        return root->getSumOfIndex(k);
    }
    void remove(DataType data){
        AVLnode<DataType, SumFunction>* node = find(data);
        AVLnode<DataType, SumFunction>* node_to_check= NULL, *new_root;
        if(!node)
            return;
        //node is a leaf
        if(!node->getLChild() && !node->getRChild()){
            node_to_check = removeNoChilds(node);
        }
        //node has 1 child
        else if(!node->getLChild() && node->getRChild())
            node_to_check = removeOneChild(node);
        else if(node->getLChild() && !node->getRChild())
            node_to_check = removeOneChild(node);
        //node has 2 childs
        else if(node->getLChild() && node->getRChild())
            node_to_check = removeTwoChildren(node);

        while(node_to_check){
            node_to_check->calcHight();
            node_to_check->calcRank();
            node_to_check->calcSum();
            node_to_check->setBF();
            new_root = node_to_check->rotate(-1);
            if(new_root){
                root = new_root;
            }
            node_to_check = node_to_check->getParent();
        }
        size--;
    }



    AVLnode<DataType, SumFunction>* findMin(AVLnode<DataType, SumFunction>* root){
        if(root == NULL){
            return NULL;
        }
        if(root->getLChild() != NULL){
            return findMin(root->getLChild());
        }
        return root;
    }

    /*
    void swap2Nodes(AVLnode<DataType, SumFunction>* node_a, AVLnode<DataType, SumFunction>* node_b) {
        AVLnode<DataType, SumFunction>* tmp = node_b->getParent();
        if(node_a->getParent()){
            if(node_a->getParent()->getRChild() == node_a)
                node_a->getParent()->setRChild(node_b);
            else
                node_a->getParent()->setLChild(node_b);
            node_b->setParent(node_a->getParent());
        }
        else
            node_b->setParent(NULL);
        if(node_a->getRChild())
            node_a->getRChild()->setParent(node_b);
        if(node_a->getLChild())
            node_a->getLChild()->setParent(node_b);

        if(tmp){
            if(tmp->getLChild() == node_b)
                tmp->setLChild(node_a);
            else
                tmp->setRChild(node_a);
            node_a->setParent(tmp);
        } else
            node_a->setParent(NULL);
        if(node_b->getRChild())
            node_b->getRChild()->setParent(node_a);
        if(node_b->getLChild())
            node_b->getLChild()->setParent(node_a);

        tmp = node_b->getRChild();
        node_b->setRChild(node_a->getRChild());
        node_a->setRChild(tmp);

        tmp = node_b->getLChild();
        node_b->setLChild(node_a->getLChild());
        node_a->setLChild(tmp);
        node_b->calcHight();
    }
     */
    void swap2data(AVLnode<DataType, SumFunction>* node1, AVLnode<DataType, SumFunction>* node2){
        DataType data = node2->getData();
        node2->setData(node1->getData());
        node1->setData(data);
    }

    template<typename Function>
    void inOrder(Function f){
        rec_startInOrder(root, f);
    }

    template<typename Function>
    void rec_startInOrder(AVLnode<DataType, SumFunction>* node, Function &f){
        if(!node)
            return;
        rec_startInOrder(node->getLChild(), f);
        f(node->getData());
        rec_startInOrder(node->getRChild(), f);
    }
    void destroyTree(AVLnode<DataType, SumFunction>* root){
        destroyTree_rec(root);
        root= nullptr;
    }

    void destroyTree_rec(AVLnode<DataType, SumFunction>* root){
        if(!root)
            return;
        destroyTree(root->getLChild());
        destroyTree(root->getRChild());
        delete root;
        size--;
    }

    int getSize(){
        return size;
    }


    AVLnode<DataType, SumFunction>* getRoot() const{
        return this->root;
    }


    template <class Func>
    void mergeTrees(AVLtree<DataType,CompareFunction, SumFunction>& tree, Func func){
        int mergedSize, array1size = this->getSize(), array2size = tree.getSize();
        DataType *arrayTree1 = new DataType[array1size];
        DataType *arrayTree2 = new DataType[array2size];
        DataType *mergedArray = new DataType[array1size + array2size];


        AVLtree<DataType, CompareFunction,SumFunction>* mergedTree;


        if(array1size != 0)
            array1size = this->inOrderMerge(func, arrayTree1);
        if(array2size != 0)
            array2size = tree.inOrderMerge(func, arrayTree2);

        mergedSize = merge2array(arrayTree1, array1size, arrayTree2, array2size, mergedArray);
       // printArray(mergedArray, mergedSize);
        this->destroyTree(root);
        createNewTree(mergedArray, mergedSize-1);
        size=mergedSize;

        delete[] arrayTree1;
        delete[] arrayTree2;
        delete[] mergedArray;
    }

    template <typename Function>
    int inOrderMerge(Function f, DataType *array){
        int i = 0;
        rec_startInOrderMerge(root, f, i, array);

        return i;
    }

    template<typename Function>
    static void rec_startInOrderMerge(AVLnode<DataType, SumFunction>* node, Function &f, int &i, DataType* array){
        if(!node)
            return;
        rec_startInOrderMerge(node->getLChild(), f, i, array);
        f(node, array, &i);
        rec_startInOrderMerge(node->getRChild(), f, i, array);
    }

    void printArray(DataType* arrayTree1,int array1size){
        int i=0;
        while(i < array1size){
            std::cout << arrayTree1[i] << std::endl;
            i++;
        }

    }
     int merge2array(DataType* arrayTree1, int array1size, DataType* arrayTree2, int array2size, DataType* mergedArray){
        int i = 0, j = 0, k = 0;
        while( i < array1size && j < array2size ){
            if(compare(arrayTree1[i], arrayTree2[j]) > 0){
                mergedArray[k] = arrayTree1[i];
                k++;
                i++;
            }
            else{
                mergedArray[k] = arrayTree2[j];
                k++;
                j++;
            }
        }
        while(i < array1size){
            mergedArray[k] = arrayTree1[i];
            i++;
            k++;
        }
        while(j < array2size){
            mergedArray[k] = arrayTree2[j];
            j++;
            k++;
        }
        return k;
    }

    AVLnode<DataType, SumFunction>* newNode(DataType data, AVLnode<DataType, SumFunction>* parent){
        AVLnode<DataType, SumFunction> *node = new AVLnode<DataType, SumFunction>;
        node->setParent(parent);
        node->setData(data);
        size++;
        return node;
    }

    void createNewTree(DataType *mergedArray, int n){
        int start = 0;
        root = sortedArrayToAVL(mergedArray, start, n, NULL);
    }

    AVLnode<DataType, SumFunction>* sortedArrayToAVL(DataType* arr, int start, int end, AVLnode<DataType, SumFunction>* parent)
    {
        if (start > end) {
            return NULL;
        }

        // get middle
        int mid = (start + end)/2;
        AVLnode<DataType, SumFunction> *root = newNode(arr[mid], parent);

         //  left child of root
        root->setLChild(sortedArrayToAVL(arr, start, mid-1, root));

        //   right child of root
        root->setRChild(sortedArrayToAVL(arr, mid+1, end, root));
        root->calcHight();
        root->calcRank();
        root->calcSum();
        size++;

        return root;
    }


};
#endif //AVL_TREE_AVL_TREE_H