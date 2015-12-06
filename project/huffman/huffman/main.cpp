//
//  main.cpp
//  huffman
//
//  Created by MichaelChen on 15/10/22.
//  Copyright © 2015年 MichaelChen. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string.h>

using namespace std;
#define UINT8 unsigned char

#include "heap.h"
#include "huffnode.h"
#include "hufftree.h"
#include "encode.h"
#include "decode.h"


//帮助输出
void help() {
    cout    << "usage: [option] inputFileName\n"
    << "\t-E --encode\n"
    << "\t-D --decode\n"
    << "\t-O --output\t[outputFileName]\n"
    << "\t-H --help\n";
}

int main(int argc, char *argv[]) {
    FILE* fileIn = NULL;
    FILE* fileOut = NULL;
    int isEncode = 0;   //标记，-1解码，0表示还没输入，1表示解码
    char temp[256];     //用于保存频率表
    
    //需要帮助
    for (int i=1; i<argc; i++) {
        //cout << argv[i] << "\n";
        if (!strcmp(argv[i], "-H") | !strcmp(argv[i], "--help")) {
            help();
            return 1;
        }
    }
    
    //检查参数
    for (int i=1; i<argc; i++) {
        //是否压缩（编码）
        if (!strcmp(argv[i], "-E") | !strcmp(argv[i], "--encode")) {
            //冲突检测
            if (isEncode) {
                cout << "WRONG USAGE, require --encode or --decode\n";
                help();
                return 1;
            } else {
                isEncode = 1;
            }
        //是否解压（解码）
        } else if (!strcmp(argv[i], "-D") | !strcmp(argv[i], "--decode")) {
            //冲突检测
            if (isEncode) {
                cout << "WRONG USAGE, require --encode or --decode\n";
                help();
                return 1;
            } else {
                isEncode = -1;
            }
        //输出文件参数检测
        } else if (!strcmp(argv[i], "-O") | !strcmp(argv[i], "--output")) {
            if (fileOut) {
                cout << "WRONG USAGE\n";
                help();
                return 1;
            }
            i++;
            if ((fileOut = fopen(argv[i], "rb"))) {
                fclose(fileOut);
                cout << "ERROR, file already exist: " << argv[i] << "\n";
                help();
                return 1;
            }
            fileOut = fopen(argv[i], "wb");
        //输入文件参数监测
        } else if (i == argc-1) {
            if (!(fileIn = fopen(argv[i], "rb"))) {
                cout << "ERROR, file not exist: " << argv[i] << "\n";
                help();
                return 1;
            }
        }
    }
    //检查输入文件参数是否缺失
    if (!fileIn) {
        cout << "WRONG USAGE, require inputFileName\n";
        help();
        return 1;
    }
    //检查编码/解码参数是否缺失
    if (!isEncode) {
        cout << "WRONG USAGE, --encode or --decode\n";
        help();
        return 1;
    }
    //没有设置输出文件则程序自动取名
    for (int i=0; !fileOut; i++) {
        sprintf(temp, "%s.huffman.%d", argv[argc-1], i);
        if (!(fileOut = fopen(temp, "rb"))) {
            fileOut = fopen(temp, "wb");
        } else {
            fclose(fileOut);
            fileOut = NULL;
        }
    }
    //开始执行编码/解码
    if (isEncode == 1) {
        encode(fileIn, fileOut);
        cout << "SUCCESS, encode file output\n";
    } else {
        //解码中途错误检测
        if (!decode(fileIn, fileOut)) {
            cout << "ERROR, file is damaged\n";
            help();
            fclose(fileIn);
            fclose(fileOut);
            return 1;
        } else {
            cout << "SUCCESS, decode file output\n";
        }
    }
    
    fclose(fileIn);
    fclose(fileOut);
    return 0;
    
}