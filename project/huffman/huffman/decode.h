//
//  decode.h
//  huffman
//
//  Created by MichaelChen on 15/12/3.
//  Copyright © 2015年 MichaelChen. All rights reserved.
//

//建树时字节出现统计
bool flag[256];

//字符已经存在树中检查
bool isExist(UINT8 input) {
    if (flag[input]) {
        return true;
    } else {
        flag[input] = true;
        return false;
    }
}

//先序遍历，递归建树
bool preOrderDecode(IntlNode<UINT8>* root, FILE * file) {
    UINT8 input;
    
    //左边判断
    fread(&input, 1, 1, file);
    //遇到0判断转义
    if (input == 0) {
        fread(&input, 1, 1, file);
        if (input == 1) {
            //建立左子树的根结点
            root->setLeft(new IntlNode<UINT8>());
            //带错误返回的递归
            if (!preOrderDecode((IntlNode<UINT8>*)(root->left()), file)) return false;
        } else {
            if (isExist(input)) return false;
            root->setLeft(new LeafNode<UINT8>(input, 0));
        }
    } else {
        if (isExist(input)) return false;
        root->setLeft(new LeafNode<UINT8>(input, 0));
    }
    
    //右边判断
    fread(&input, 1, 1, file);
    //遇到0判断转义
    if (input == 0) {
        fread(&input, 1, 1, file);
        if (input == 1) {
            //建立右子树的根结点
            root->setRight(new IntlNode<UINT8>());
            //带错误返回的递归
            if (!preOrderDecode((IntlNode<UINT8>*)(root->right()), file)) return false;
        } else {
            if (isExist(input)) return false;
            root->setRight(new LeafNode<UINT8>(input, 0));
        }
    } else {
        if (isExist(input)) return false;
        root->setRight(new LeafNode<UINT8>(input, 0));
    }
    return true;
}

bool decode(FILE* fileIn, FILE* fileOut) {
    UINT8 tail;
    UINT8 in, in_temp, out;
    
    for (int i=0; i<256; i++) {
        flag[i] = false;
    }
    //读取小尾巴
    fread(&tail, 1, 1, fileIn);
    
    IntlNode<UINT8>* tree = new IntlNode<UINT8>();
    HuffNode<UINT8>* curr;
    //建树，检查错误
    if (!preOrderDecode(tree, fileIn)) return false;
    for (int i=0; i<256; i++) {
        if (!flag[i]) return false;
    }
    
    //解码开始
    curr = tree;
    fread(&in, 1, 1, fileIn);
    while (1) {
        fread(&in_temp, 1, 1, fileIn);
        //正常解码
        if (!feof(fileIn)) {
            for (UINT8 i=128; i>0; i>>=1) {
                //爬树
                curr = (!(i & in))?(((IntlNode<UINT8>*)curr)->left()):(((IntlNode<UINT8>*)curr)->right());
                if (curr->isLeaf()) {
                    out = ((LeafNode<UINT8>*)curr)->val();
                    fwrite(&out, 1, 1, fileOut);
                    curr = tree;
                }
            }
        } else {
            //小尾巴解码
            for (UINT8 i=128; i>0; i>>=1) {
                //爬树
                curr = (!(i & in))?(((IntlNode<UINT8>*)curr)->left()):(((IntlNode<UINT8>*)curr)->right());
                if (curr->isLeaf()) {
                    out = ((LeafNode<UINT8>*)curr)->val();
                    fwrite(&out, 1, 1, fileOut);
                    curr = tree;
                }
                tail--;
                if (tail == 0) {
                    break;
                }
            }
            break;
        }
        in = in_temp;
    }
    return true;
}
