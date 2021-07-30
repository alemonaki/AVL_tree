/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AVL.hpp
 * Author: alkisti
 *
 * Created on May 3, 2020, 1:15 PM
 */

#ifndef AVL_HPP
#define AVL_HPP

#include <iostream>
#include <fstream>
#include<stack>

using namespace std;

class AVL {
private:
  class Node {
    Node *parent, *left, *right;
    int height;
    string element;

  public:
    Node(const string& e, Node *parent, Node *left, Node *right);
    
    Node*  getParent() const;
    Node*  getLeft() const;
    Node*  getRight() const;
    string getElement() const;
    int    getHeight() const;
  
    void setLeft(Node *);
    void setRight(Node *);
    void setParent(Node *);
    void setElement(string e);
    bool isLeft() const;
    bool isRight() const;
    int  rightChildHeight() const;
    int  leftChildHeight() const;
    int  updateHeight();
    bool isBalanced();
    //mine
    void setHeight(int h);
    //mine
  };
private:
  
  int   size;
  Node* root;
  
public:
    
    class Iterator {
    private:    
        stack<Node *> st;
        Node* curr;
    private:
        bool hasNext(); 
    public:
      Iterator(Node *);  
      Iterator();
      Iterator& operator++();
      Iterator operator++(int a);      
      string operator*(); 
      bool operator!=(Iterator it);
      bool operator==(Iterator it);
    };
    
  Iterator begin() const;  
  Iterator end() const;
  
  static const int MAX_HEIGHT_DIFF = 1;
  AVL();
  AVL(const AVL& );
  bool contains(string e);
  //mine
  void deleteNode(Node *);
  void exchangeElements(Node * a, Node * b);
  Node * deleteItem(string e);
  void addLeaf(Node *, bool kindofson, string e);
  Node * contains(Node *, string e);
  Node * rebalanceSon(Node *);
  void rebalance(Node *);
  Node * reconstruct(Node *, Node *, Node *);
  //mine
  bool add(string e);
  bool rmv(string e);
  void print2DotFile(char *filename);
  void printDot(Node *, ofstream &fout);
  void printDotNull(string key, int nullcount, ofstream &fout);
  void pre_order(std::ostream& out);

  friend std::ostream& operator<<(std::ostream& out, const AVL& tree);  
  AVL& operator  =(const AVL& avl);
  AVL  operator  +(const AVL& avl);
  AVL& operator +=(const AVL& avl);
  AVL& operator +=(const string& e);
  AVL& operator -=(const string& e);
  AVL  operator  +(const string& e);
  AVL  operator  -(const string& e);
};

#endif /* AVL_HPP */

