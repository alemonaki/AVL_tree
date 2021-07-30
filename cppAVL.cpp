/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AVL.cpp
 * Author: alkisti
 *
 * Created on May 3, 2020, 1:26 PM
 */

#include <cstdlib>
#include<iostream>
#include<algorithm>
#include<string.h>
#include<stack>
#include "AVL.hpp"
static bool Left = true;
static bool Right = false;

#define MAX_LINE_LEN 128
#define MAX_ENTRIES 64


using namespace std;

//NODE CLASS

AVL::Node::Node(const string& e, Node *parent, Node *left, Node *right){
    element = e;
    this->parent = parent;
    this->right = right;
    this->left = left;
    height = 1;
}

AVL::Node* AVL::Node::getParent() const{
    if(this != NULL){
        return(parent);
    }
    return NULL;
}

AVL::Node* AVL::Node::getLeft() const{
    if(this != NULL){
        return(left);
    }
    return NULL;
}

AVL::Node* AVL::Node::getRight() const{
    if(this != NULL){
        return(right);
    }
    return NULL;
}

string AVL::Node::getElement() const{
    if(this != NULL){
        return(element);
    }
    return "";
}

int AVL::Node::getHeight() const{
    if(this != NULL){
        return(height);
    }
    return 0;
}

void AVL::Node::setLeft(Node* v){
    left = v;
}

void AVL::Node::setRight(Node* v){
    right = v;
}

void AVL::Node::setParent(Node* v){
    parent = v;
}

void AVL::Node::setElement(string e){
    element= e;
}

void AVL::Node::setHeight(int h){
    height = h;
}

bool AVL::Node::isLeft() const{
    if(parent == NULL){
        return(false);
    }
    return(this == parent->left);
}

bool AVL::Node::isRight() const{
    if(parent == NULL){
        return(false);
    }
    return(this == parent->right);
}

int AVL::Node::rightChildHeight() const{
    if(this == NULL){
        return -1;
    }
    return(right != NULL ? right->height : 0);
}

int AVL::Node::leftChildHeight() const{
    if(this == NULL){
        return -1;
    }
    return(left != NULL ? left->height : 0);
}

int  AVL::Node::updateHeight(){
    if(this == NULL){
        return 0;
    }
    else{
        setHeight(1 + max(rightChildHeight(), leftChildHeight()));
    }
    return(height);
}

bool AVL::Node::isBalanced(){
    if(this == NULL){
        return -1;
    }
    int balance = leftChildHeight() - rightChildHeight();
    return((-1 <= balance) && (balance <= 1));
}

//ITERATOR CLASS

AVL::Iterator::Iterator(){
    curr = NULL;
}

AVL::Iterator::Iterator(Node * root){
    if (root!=NULL) {
        st.push(root);
        curr = root;
    }
    else{
        curr = NULL;
    }
}

bool AVL::Iterator::hasNext(){
    return !st.empty();
}

AVL::Iterator& AVL::Iterator::operator++(){
    if(!hasNext()){
        curr = NULL;
        return(*this);
    }
    Node *res=st.top();
    st.pop();
    if (res->getRight()!=NULL) {
        st.push(res->getRight());
    }
    if (res->getLeft()!=NULL) {
        st.push(res->getLeft());
    }
    if(!hasNext()){
        curr = NULL;
        return(*this);
    }
    curr = st.top();
    return(*this);
}

AVL::Iterator AVL::Iterator::operator++(int a){
    Iterator* it = new Iterator();
    it->st = this->st;
    it->curr = this->curr;
    if(!hasNext()){
        curr = NULL;
        return(*it);
    }
    Node *res=st.top();
    st.pop();
    if (res->getRight()!=NULL) {
        st.push(res->getRight());
    }
    if (res->getLeft()!=NULL) {
        st.push(res->getLeft());
    }
    if(!hasNext()){
        curr = NULL;
        return(*this);
    }
    curr = st.top();
    return(*it);
}

string AVL::Iterator::operator*(){
    return(st.top()->getElement());
}

bool AVL::Iterator::operator!=(Iterator it){
    if(this->curr->getElement() == it.curr->getElement()){
        return(false);
    }
    return(true);
}

bool AVL::Iterator::operator==(Iterator it){
    if(this->curr->getElement() == it.curr->getElement()){
        return(true);
    }
    return(false);
}

//AVL-ITERATOR

AVL::Iterator AVL::begin() const{
    Iterator *newIt = new Iterator(root);
    return(*newIt);
}

AVL::Iterator AVL::end() const{
    Iterator *newIt = new Iterator();
    return(*newIt);
}

//AVL CLASS

AVL::AVL(){
    size = 0;
    root = NULL;
}

AVL::AVL(const AVL& tree){
    //cout<<"mphke ston copy constructor tou AVL"<<endl;
    size = 0;
    root = NULL;
    Iterator start;
    Iterator end = tree.end();
    for(start = tree.begin(); start != end; ++start){
        add(*start);
    }
    //cout << *this<<endl<<"AVL apo copy const"<<endl<<endl;
}


AVL::Node * AVL::rebalanceSon(Node* start){
    if(start == NULL){
        return NULL;
    }
    if(start->getLeft()->getHeight() > start->getRight()->getHeight()){
        return(start->getLeft());
    }
    else if(start->getLeft()->getHeight() < start->getRight()->getHeight()){
        return(start->getRight());
    }
    else if(start->isLeft()){
        return(start->getLeft());
    }
    else return(start->getRight());
}

void AVL::rebalance(Node * start){
    if(start == NULL){
        return;
    }
    Node * u;
    Node * w;
    while(start != NULL){
        start->updateHeight();
        if(!(start->isBalanced())){
            w = rebalanceSon(start);
            u = rebalanceSon(w);
            start = reconstruct(start, w, u);
            start->getLeft()->updateHeight();
            start->getRight()->updateHeight();
            start->updateHeight();
        }
        start = start->getParent();
    }
    //cout<<"teleiwse"<<endl;
}

AVL::Node * AVL::reconstruct(Node * v, Node * w, Node * u){
    if(w->isLeft() && u->isLeft()){                    //deksia aplh peristrofh
        if(!(v == root)){
            if(v->isLeft()){
                v->getParent()->setLeft(w);
            }
            else{
                v->getParent()->setRight(w);
            }
            w->setParent(v->getParent());
        }
        v->setLeft(w->getRight());
        if(w->getRight() != NULL){
            w->getRight()->setParent(v);
        }
        w->setRight(v);
        v->setParent(w);
        if(v == root){
            root = w;
            w->setParent(NULL);
        }
        return w;
    }
    else if(w->isRight() && u->isRight()){              //aristerh aplh peristrofh
        if(!(v == root)){
            if(v->isRight()){
                v->getParent()->setRight(w);
            }
            else{
                v->getParent()->setLeft(w);
            }
            w->setParent(v->getParent());
        }
        v->setRight(w->getLeft());
        if(w->getLeft() != NULL){
            w->getLeft()->setParent(v);
        }
        w->setLeft(v);
        v->setParent(w);
        if(v == root){
            root = w;
            w->setParent(NULL);
        }
        return w;
    }
    else if(u->isLeft()){                    //aristerh diplh peristrofh
        v->setRight(u->getLeft());
        if(u->getLeft() != NULL){
            u->getLeft()->setParent(v);
        }
        w->setLeft(u->getRight());
        if(u->getRight() != NULL){
            u->getRight()->setParent(w);
        }
        if(!(v == root)){
            if(v->isRight()){
                v->getParent()->setRight(u);
            }
            else{
                v->getParent()->setLeft(u);
            }
            u->setParent(v->getParent());
        }
        v->setParent(u);
        w->setParent(u);
        u->setLeft(v);
        u->setRight(w);
        if(v == root){
            root = u;
            u->setParent(NULL);
        }
        return u;
    }
    else{
        v->setLeft(u->getRight());          //deksia diplh peristrofh
        if(u->getRight() != NULL){
            u->getRight()->setParent(v);
        }
        w->setRight(u->getLeft());
        if(u->getLeft() != NULL){
            u->getLeft()->setParent(w);
        }
        if(!(v == root)){
            if(v->isLeft()){
                v->getParent()->setLeft(u);
            }
            else{
                v->getParent()->setRight(u);
            }
            u->setParent(v->getParent());
        }
        v->setParent(u);
        w->setParent(u);
        u->setLeft(w);
        u->setRight(v);
        if(v == root){
            root = u;
            u->setParent(NULL);
        }
        return u;
    }
}

AVL::Node * AVL::contains(Node * start, string e){
    if(start->getElement() > e){
        if(start->getLeft() == NULL){
            return start;
        }
        else{
            return(contains(start->getLeft(), e));
        }
    }
    else if(start->getElement() < e){
        if(start->getRight() == NULL){
            return start;
        }
        else{
            return(contains(start->getRight(), e));
        }
    }
    else{
        return start;
    }
}

bool AVL::contains(string e){
    Node* res;
    Node* start = root;
    res = contains(start, e);
    if(res->getElement() == e){
        return(true);
    }
    return(false);
}

void AVL::addLeaf(Node * v, bool kindofson, string e){
    if(kindofson == Left){
        if(v->getLeft() != NULL){
            return;
        }
        //cout<<"to vazw aristero paidi" << endl;
        v->setLeft(new Node(e, v, NULL, NULL));
        //cout<<v->getLeft()->getElement() << " kainourgio" << endl;
    }
    else{
        if(v->getRight() != NULL){
            return;
        }
        //cout<<"to vazw deksi paidi" << endl;
        v->setRight(new Node(e, v, NULL, NULL));
    }
    size = size + 1;
}

bool AVL::add(string e){
    if(size == 0){
        root = new Node(e, NULL, NULL, NULL);
        size = 1;
        return(true);
    }
    if(contains(e)){
        return(false);
    }
    else{
        //cout<< e <<endl;
        Node * res = contains(root, e);
        //cout<<res->getElement()<<" edw tha mpei" <<endl;
        if(res->getElement() > e){
            addLeaf(res, Left, e);
            if(res->getLeft() == NULL){
                cout<<"falseleft"<<endl;
                return(false);
            }
            //cout<<"mphke " <<res->getLeft()->getElement()<<endl;
            rebalance(res->getLeft());
            //cout<<"mphke"<<endl;
            return(true);
        }
        else{
            addLeaf(res, Right, e);
            if(res->getRight() == NULL){
                cout<<"false"<< "\n" << endl;
                return(false);
            }
            rebalance(res->getRight());
            return(true);
        }
    }
}

void AVL::deleteNode(Node* v){
    if(v->getLeft() != NULL && v->getRight() != NULL){
        return;
    }
    if(v == root){
        Node * notNullChildOfv = (v->getLeft() != NULL ? v->getLeft() : v->getRight());
        root = notNullChildOfv;
        if(root != NULL){
            root->setParent(NULL);
        }
    }
    else{
        Node * parentOfv = v->getParent();
        Node * notNullChildOfv = (v->getLeft() != NULL ? v->getLeft() : v->getRight());
        if(v->isLeft()){
            parentOfv->setLeft(notNullChildOfv);
        }
        else{
            parentOfv->setRight(notNullChildOfv);
        }
        if(notNullChildOfv != NULL){
            notNullChildOfv->setParent(parentOfv);
        }
    }
    size = size - 1;
    v->setLeft(NULL);
    v->setRight(NULL);
    v->setParent(NULL);
}

void AVL::exchangeElements(Node * a, Node * b){
    string temp = a->getElement();
    a->setElement(b->getElement());
    b->setElement(temp);
}
AVL::Node * AVL::deleteItem(string e){
    if(size == 0){
        return(NULL);
    }
    if(!contains(e)){
       return(NULL);
    }
    else{
        //cout << "mphke" <<endl;
        Node * res = contains(root, e);
        Node * returnNode;
        if((res->getLeft() == NULL) || (res->getRight() == NULL)){
            returnNode = (res->getLeft() != NULL ? res->getLeft() :
                          res->getRight() != NULL ? res->getRight() :
                          res->getParent());

           deleteNode(res);
           return(returnNode);
        }
        else{
            Node * cursor = res->getRight();
            Node * temp;
            Node * parentDelNode;
            while(((temp = cursor->getLeft()) != NULL)){
                cursor = temp;
            }
            exchangeElements(cursor, res);
            parentDelNode = cursor->getParent();
            deleteNode(cursor);
            return(parentDelNode);
        }
    }
    return(NULL);
}

bool AVL::rmv(string e){
    Node * dltNode = deleteItem(e);
    //cout << "mphke" <<endl;
    if(dltNode == NULL){
        return(false);
    }
    rebalance(dltNode);
    return(true);
}

ostream& operator<<(ostream& out, const AVL& tree) {
    AVL::Iterator start;
    AVL::Iterator last = tree.end();
    for(start = tree.begin(); start != last; ++start){
        out << *start << " " ;
    }
    return out;
}

void AVL::pre_order(ostream& out){
    Iterator start;
    Iterator last = end();
    for(start = begin(); start != last; ++start){
        out << *start << " " ;
    }
    out<<endl;
}

AVL &AVL::operator =(const AVL& avl){
    //cout << "mphke sto ="<<endl;
    Iterator start;
    Iterator last;
//     cout<<*this<<endl<<"molis mphke sto = to aristero AVL"<<endl<<endl;
//     cout<<avl<<endl<<"sto = to deksi AVL"<<endl<<endl;
//     if(this->root == NULL){
//         cout<<"einai null"<<endl;
//         this->root = avl.root;
//         this->size = avl.size;
//         return(*this);
//     }
    while(1){
        start = begin();
        if(start == (last = end())){
            break;
        }
        rmv(*start);
    }
    for(start = avl.begin(); start != (last = avl.end()); ++start){
        add(*start);
    }
    //cout << *this<<endl<<"AVL apo ="<<endl<<endl;
    return(*this);
}

AVL AVL::operator +(const AVL& avl){
    Iterator start;
    Iterator last;
    AVL newAVL;
    for(start = begin(); start != (last = end()); ++start){
        newAVL.add(*start);
    }
    for(start = avl.begin(); start != (last = avl.end()); ++start){
        newAVL.add(*start);
    }
    //cout<<newAVL<<endl<<endl;
    return(newAVL);
}

AVL &AVL::operator +=(const AVL& avl){
    Iterator start;
    Iterator last = avl.end();
    for(start = avl.begin(); start != last; ++start){
        add(*start);
    }
    return(*this);
}

AVL &AVL::operator +=(const string& e){
    if(!contains(e)){
        add(e);
    }
    return(*this);
}

AVL &AVL::operator -=(const string& e){
    if(contains(e)){
        rmv(e);
    }
    return(*this);
}

AVL AVL::operator +(const string& e){
    AVL newAVL;
    Iterator start;
    Iterator last = end();
    for(start = begin(); start != last; ++start){
        newAVL.add(*start);
    }
    newAVL.add(e);
    return(newAVL);
}
AVL AVL::operator  -(const string& e){
    AVL newAVL;
    Iterator start;
    Iterator last = end();
    for(start = begin(); start != last; ++start){
        newAVL.add(*start);
    }
    newAVL.rmv(e);
    return(newAVL);
}

void AVL::print2DotFile(char *filename){
    ofstream fout(filename);
    fout << "diagraph AVL{\n";
    fout << "\tnode [fontname=\"Arial\"];\n";

    if(root == NULL){
        fout << "\n";
    }
    else if(root->getLeft() == NULL && root->getRight() == NULL){
        fout << "\t" << root->getElement() << ";\n";
    }
    else{
        printDot(root, fout);
    }
    fout << "}\n";
}

void AVL::printDotNull(string key, int nullcount, ofstream& fout){
    fout << "\tnull"<< nullcount<< " [shape=point];\n";
    fout << "\t" << key << " -> null" << nullcount << ";\n";
}

void AVL::printDot(Node *node, ofstream &fout){
    static int nullcount = 0;

    if(node->getLeft() != NULL){
        fout << "\t" << node->getElement() << " -> " << node->getLeft()->getElement() << ";\n";
        printDot(node->getLeft(), fout);
    }
    else{
        printDotNull(node->getElement(), ++nullcount, fout);
    }
    if(node->getRight() != NULL){
        fout << "\t" << node->getElement() << " -> " << node->getRight()->getElement() << ";\n";
        printDot(node->getRight(), fout);
    }
    else{
        printDotNull(node->getElement(), ++nullcount, fout);
    }
}
