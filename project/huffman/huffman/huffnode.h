//
//  huffnode.h
//  huffman
//
//  Created by MichaelChen on 15/10/22.
//  Copyright © 2015年 MichaelChen. All rights reserved.
//

// Huffman tree node abstract base class
template <typename E> class HuffNode {
public:
    virtual ~HuffNode() {}                // Base destructor
    virtual int weight() = 0;             // Return frequency
    virtual bool isLeaf() = 0;            // Determine type
};

template <typename E>   // Internal node subclass
class IntlNode : public HuffNode<E> {
private:
    HuffNode<E>* lc;   // Left child
    HuffNode<E>* rc;   // Right child
    int wgt;              // Subtree weight
public:
    IntlNode() {}
    IntlNode(HuffNode<E>* l, HuffNode<E>* r)
    { wgt = l->weight() + r->weight(); lc = l; rc = r; }
    int weight() { return wgt; }
    bool isLeaf() { return false; }
    HuffNode<E>* left() const { return lc; }
    void setLeft(HuffNode<E>* b)
    { lc = (HuffNode<E>*)b; }
    HuffNode<E>* right() const { return rc; }
    void setRight(HuffNode<E>* b)
    { rc = (HuffNode<E>*)b; }
};

template <typename E>   // Leaf node subclass
class LeafNode : public HuffNode<E> {
private:
    E it;			// Value
    int wgt;		// Weight
public:
    LeafNode(const E& val, int freq)   // Constructor
    { it = val; wgt = freq; }
    int weight() { return wgt; }
    E val() { return it; }
    bool isLeaf() { return true; }
};

