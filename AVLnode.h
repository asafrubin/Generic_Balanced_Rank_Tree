#ifndef AVL_TREE_AVLNODE_H
#define AVL_TREE_AVLNODE_H

#include <stdlib.h>

using namespace std;

template <class DataType, class SumFunction>
class AVLnode{
private:
    SumFunction sumFunction;
    int bf;
    int hight;
    int rank;
    int sum;
    DataType data;
    AVLnode<DataType, SumFunction> *l_child;
    AVLnode<DataType, SumFunction> *r_child;
    AVLnode<DataType, SumFunction> *parent;

public:
    AVLnode() : bf(0), hight(0), data(),rank(0), sum(0), l_child(NULL), r_child(NULL), parent(NULL) {};
    explicit AVLnode(DataType data) : bf(0), hight(0), rank(1), data(data), l_child(NULL), r_child(NULL), parent(NULL) {
        sum = sumFunction(data);
    };
    ~AVLnode(){
        delete data;
    };
    AVLnode(const AVLnode& avLnode) {
        this->parent = avLnode.parent;
        this->l_child = avLnode.l_child;
        this->r_child = avLnode.r_child;
        this->bf = avLnode.bf;
        this->hight = avLnode.hight;
        this->data = avLnode.data;
    };
    // sets a new right child
    void setRChild(AVLnode *new_r_child){
        this->r_child = new_r_child;
    }
    //get Lchild
    AVLnode* getRChild(){
        return this->r_child;
    }
    //set a new Lchild
    void setLChild(AVLnode *new_l_child){
        this->l_child = new_l_child;
    }
    //get the left child
    AVLnode* getLChild(){
        return this->l_child;
    }
    //sets a new parent to the node
    void setParent(AVLnode *new_parent){
        this->parent = new_parent;
    }
    //gets the parent of a node
    AVLnode* getParent(){
        return this->parent;
    }
    //set a new height the a tree Node
    void setHight(int new_hight){
        this->hight = new_hight;
    }
    //calculate the highet of a node
    void calcHight(){
        int l_tree,r_tree;
        if(!l_child){
            l_tree = 0; }
        else
            l_tree = l_child->getHight();
        if(!r_child){
            r_tree = 0;
        }
        else
            r_tree = r_child->getHight();

        this->setHight((l_tree > r_tree) ? (l_tree + 1) : (r_tree + 1));
    }
    //get the height of a subtree
    int getHight(){
        return this->hight;
    }
    //sets a new data to the tree node
    void setData(DataType &new_data){
        this->data = new_data;
    }
    //get the Data itself
    DataType& getData(){
        return this->data;
    }
    //gets data that can be mallable
    DataType *getRawData(){
        return this->data;
    }
    //get a data in constant form
    const DataType& getConstData(){
        return this->data;
    }
    //get The Rank of a subtree
    int getRank(){
        return rank;
    }
    //get the BF of a subtree
    int getBF(){
        //if(!this) return -100;
        //else
            return this->bf;
    }
    //get the sum of a subtree
    int getSum(){
        //if(this)
            return sum;
        //return 0;
    }
    //stes a tree's BF
    void setBF(){
        int left=0, right=0;
        if(!r_child)
            right = 0;
        else
            right = r_child->getHight();
        if(!l_child)
            left = 0;
        else
            left = l_child->getHight();
        bf = left - right;
    }
    //gets the sum of k leftest nodes in the tree
    int getSumOfIndex(int k){
        int rank;
        if(getLChild() != NULL) {
            if (this->getLChild()->getRank() == k - 1) {
                if (getRChild() == NULL)
                    return this->getSum();
                return this->getSum()-this->getRChild()->getSum();
            }
            if (this->getLChild()->getRank() > k - 1)
                return this->getLChild()->getSumOfIndex(k);
        }
        else if(k-1==0){
            if (getRChild() == NULL)
                return this->getSum();
            return this->getSum()-this->getRChild()->getSum();
        }
        if(getRChild() != NULL) {
            int tmp_sum = this->getSum();
            tmp_sum -= this->getRChild()->getSum();
            if(!this->getLChild()){
                rank = 0;
            } else
                rank = this->getLChild()->getRank();
            return tmp_sum + this->getRChild()->getSumOfIndex(k - 1 - rank);
        }
        return 0;

    }
    //rotate nodes insides the tree using RR,RL,LL,LR cycles
    AVLnode* rotate(int old_hight){
        if(old_hight == this->getHight())
            return NULL;
        else if(bf == 2){
            this->getLChild()->setBF();
            if(this->getLChild()->getBF() >= 0 ){
             return LL(this);
            }
            else if(this->getLChild()->bf < 0){
                return LR(this);
            }
        }
        else if(bf == -2){
            this->getRChild()->setBF();
           if(this->getRChild()->getBF() > 0){
               return RL(this);
           }
           else if(this->getRChild()->getBF() <= 0){
               return RR(this);
           }
        }
            return NULL;
    }

    /*
    AVLnode* LL_temp(AVLnode* b){
        AVLnode* tmp;
        AVLnode* a = b->getLChild();
        if(b->getLChild()){
            tmp = b->getLChild()->getRChild();
        }
        else
            tmp = NULL;
        b->setLChild(tmp);
        if(b->getParent()) {
            if (b->getParent()->getLChild() == b) {
                b->getParent()->setLChild(a);
            } else
                b->getParent()->setRChild(a);
        }
        else
            a->setParent(NULL);

        a->setRChild(b);
        a->getLChild()->calcHight();
        a->getRChild()->calcHight();
    }
    */

    AVLnode* LL(AVLnode* b){
        AVLnode *b_parent = b->getParent();
        AVLnode* tmp;
        AVLnode *a = b->getLChild();
        if(b->getLChild()){
            tmp = b->getLChild()->getRChild();
        }
        else
            tmp = NULL;
        if(b_parent == NULL){
            b->setLChild(tmp);
            if(tmp)
                b->getLChild()->setParent(b);
            a->setRChild(b);
            b->setParent(a);
            a->setParent(NULL);
            b->calcRank();
            b->calcSum();
            a->getLChild()->calcHight();
            a->getLChild()->calcRank();
            a->getLChild()->calcSum();
            a->getRChild()->calcHight();
            a->getRChild()->calcRank();
            a->getRChild()->calcSum();
            a->calcRank();

            return a;
        }
        else {
            b->setLChild(a->getRChild());
            if (b->getLChild()) {
                b->getLChild()->setParent(b);
            }
            a->setRChild(b);
            b->setParent(a);
            if (b_parent->getLChild() == b) {
                b_parent->setLChild(a);
            } else
                b_parent->setRChild(a);
        }
            a->setParent(b_parent);
            if(a->getLChild()) {
                a->getLChild()->calcHight();
                a->getLChild()->calcRank();
                a->getLChild()->calcSum();
            }
            if(a->getRChild()) {
                a->getRChild()->calcHight();
                a->getRChild()->calcRank();
                a->getRChild()->calcSum();
            }

        a->calcRank();
        a->calcSum();
        return NULL;
    }

    AVLnode* RR(AVLnode* b) {
        AVLnode *b_parent = b->getParent();
        AVLnode *a = b->getRChild();
        AVLnode* tmp;
        if(b->getRChild()){
            tmp = b->getRChild()->getLChild();
        }
        else
            tmp = NULL;
        if(b_parent == NULL){
            b->setRChild(tmp);
            if(tmp)
                b->getRChild()->setParent(b);
            a->setLChild(b);
            b->setParent(a);
            a->setParent(NULL);
            if(a->getLChild()) {
                a->getLChild()->calcHight();
                a->getLChild()->calcRank();
                a->getLChild()->calcSum();
            }
            if(a->getRChild()) {
                a->getRChild()->calcHight();
                a->getRChild()->calcRank();
                a->getRChild()->calcSum();
            }

            a->calcRank();
            a->calcSum();
            return a;
        }
        else {
            b->setRChild(a->getLChild());
            if (tmp)
                b->getRChild()->setParent(b);
            a->setLChild(b);
            b->setParent(a);
            if (b_parent->getLChild() == b) {
                b_parent->setLChild(a);
            } else
                b_parent->setRChild(a);
        }

        a->setParent(b_parent);
        if(a->getLChild()) {
            a->getLChild()->calcHight();
            a->getLChild()->calcRank();
            a->getLChild()->calcSum();
        }
        if(a->getRChild()) {
            a->getRChild()->calcHight();
            a->getRChild()->calcRank();
            a->getRChild()->calcSum();
        }
        a->calcHight();
        a->calcRank();
        a->calcSum();

        return NULL;
    }
    AVLnode* LR(AVLnode* c){
        RR(c->getLChild());
        return LL(c);
    }
    AVLnode* RL(AVLnode* c) {
        LL(c->getRChild());
        return RR(c);
    }
    //calculating the rank of a subtree
    void calcRank(){
        int rank_l_child, rank_r_child;
        if(!l_child)
            rank_l_child = 0;
        else
            rank_l_child = l_child->getRank();
        if(!r_child)
            rank_r_child = 0;
        else
            rank_r_child = r_child->getRank();

        rank = rank_l_child + rank_r_child + 1;
    }
    //sets a new rank to the tree
    void setRank(int rank){
        this->rank = rank;
    }
    //calculating the sum of a function
    void calcSum(){
        int sum_l_child, sum_r_child;
        if(!l_child)
            sum_l_child = 0;
        else
            sum_l_child = l_child->getSum();
        if(!r_child)
            sum_r_child = 0;
        else
            sum_r_child = r_child->getSum();

        sum = sum_l_child + sum_r_child + sumFunction(this->getData());
    }

};

#endif //AVL_TREE_AVLNODE_H