//
//  encode.hpp
//  huffman
//
//  Created by MichaelChen on 15/12/3.
//  Copyright © 2015年 MichaelChen. All rights reserved.
//

// Comparator for the heap

//设置比较规则
class minTreeComp {
public:
    static bool prior(HuffTree<UINT8>* x, HuffTree<UINT8>* y)
    { return x->weight() < y->weight(); }
};

//通过建树
HuffTree<UINT8>* buildHuff(HuffTree<UINT8>** TreeArray, int count) {
    heap<HuffTree<UINT8>*,minTreeComp>* forest =
    new heap<HuffTree<UINT8>*, minTreeComp>(TreeArray, count, count);
    HuffTree<UINT8> *temp1, *temp2, *temp3 = NULL;
    while (forest->size() > 1) {
        temp1 = forest->removefirst();   // Pull first two trees
        temp2 = forest->removefirst();   //   off the list
        temp3 = new HuffTree<UINT8>(temp1, temp2);
        forest->insert(temp3);  // Put the new tree back on list
        delete temp1;        // Must delete the remnants
        delete temp2;        //   of the trees we created
    }
    return temp3;
}

//爬树建立编码表
void makeTable(HuffNode<UINT8> * root, char table[256][256], char code[256]) {
    if (root->isLeaf()) {
        strcpy(table[((LeafNode<UINT8>*)root)->val()], code);
    } else {
        code[strlen(code)] = '0';
        makeTable(((IntlNode<UINT8>*)root)->left(), table, code);
        code[strlen(code)-1] = '1';
        makeTable(((IntlNode<UINT8>*)root)->right(), table, code);
        code[strlen(code)-1] = '\0';
    }
}

//前序遍历输出树的编码
void preOrderEncode(IntlNode<UINT8>* root, FILE* file) {
    UINT8 out;
    if (root->left()->isLeaf()) {
        out = ((LeafNode<UINT8>*)(root->left()))->val();
        if (out == 0) {
            fwrite(&out, 1, 1, file);
        }
        fwrite(&out, 1, 1, file);
    } else {
        out = 0;
        fwrite(&out, 1, 1, file);
        out = 1;
        fwrite(&out, 1, 1, file);
        preOrderEncode((IntlNode<UINT8>*)(root->left()), file);
    }
    
    if (root->right()->isLeaf()) {
        out = ((LeafNode<UINT8>*)(root->right()))->val();
        if (out == 0) {
            fwrite(&out, 1, 1, file);
        }
        fwrite(&out, 1, 1, file);
    } else {
        out = 0;
        fwrite(&out, 1, 1, file);
        out = 1;
        fwrite(&out, 1, 1, file);
        preOrderEncode((IntlNode<UINT8>*)(root->right()), file);
    }
}

void encode(FILE* fileIn, FILE* fileOut) {
    int arr[256];
    HuffTree<UINT8> ** treeArray;
    treeArray = (HuffTree<UINT8>**)malloc(sizeof(HuffTree<UINT8>*)*256);
    char codeTable[256][256];
    char tempCode[256];
    UINT8 in, out;
    
    
    //数组初始化
    for (int i=0; i<256; i++)
        arr[i] = 0;
    
    while (!feof(fileIn)) {
        fread(&in, 1, 1, fileIn);
        if (feof(fileIn)) {
            break;
        }
        arr[in]++;
    }
    
    //第一次遍历建树
    for (UINT8 i=0; ; i++) {
        treeArray[i] = new HuffTree<UINT8>(i, arr[i]);
        if (i == 255) break;
    }
    HuffTree<UINT8> * huffTree = buildHuff(treeArray, 256);
    makeTable(huffTree->root(), codeTable, tempCode);
    
    /*
     //输出编码表
     for (UINT8 i=0; ; i++) {
     //cout << (int)i << ":" << codeTable[i] << "\n";
     if (i == 255) break;
     }*/
    
    //第二次遍历编码
    rewind(fileIn);
    out = 0;
    fwrite(&out, 1, 1, fileOut);
    
    preOrderEncode((IntlNode<UINT8>*)(huffTree->root()), fileOut);
    
    tempCode[0] = '\0';
    while (!feof(fileIn)) {
        fread(&in, 1, 1, fileIn);
        if (feof(fileIn)) {
            break;
        }
        //类似队列操作，缓存队列，满8输出。
        strcat(tempCode, codeTable[in]);
        while (strlen(tempCode)>8) {
            out = tempCode[0]-'0';
            out = (out << 1)|((tempCode[1]-'0') & 1);
            out = (out << 1)|((tempCode[2]-'0') & 1);
            out = (out << 1)|((tempCode[3]-'0') & 1);
            out = (out << 1)|((tempCode[4]-'0') & 1);
            out = (out << 1)|((tempCode[5]-'0') & 1);
            out = (out << 1)|((tempCode[6]-'0') & 1);
            out = (out << 1)|((tempCode[7]-'0') & 1);
            fwrite(&out, 1, 1, fileOut);
            char tempCode2[256];
            strcpy(tempCode2, tempCode+8);
            strcpy(tempCode, tempCode2);
        }
    }
    //小尾巴输出
    out = 0;
    for (int i=0; i<8; i++) {
        if (i<strlen(tempCode)) {
            out = (out << 1)|((tempCode[i]-'0') & 1);
        } else {
            out = out << 1;
        }
    }
    fwrite(&out, 1, 1, fileOut);
    //小尾巴标记输出
    out = (UINT8)strlen(tempCode);
    rewind(fileOut);
    fwrite(&out, 1, 1, fileOut);
}