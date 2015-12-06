//
//  hufftree.h
//  huffman
//
//  Created by MichaelChen on 15/10/22.
//  Copyright © 2015年 MichaelChen. All rights reserved.
//

template <typename E>
class HuffTree {
private:
    HuffNode<E>* Root;         // Tree root
public:
    HuffTree(E& val, int freq) // Leaf constructor
    { Root = new LeafNode<E>(val, freq); }
    // Internal node constructor
    HuffTree(HuffTree<E>* l, HuffTree<E>* r)
    { Root = new IntlNode<E>(l->root(), r->root()); }
    ~HuffTree() {}                          // Destructor
    HuffNode<E>* root() { return Root; }    // Get root
    int weight() { return Root->weight(); } // Root weight
};
