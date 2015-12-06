# 哈夫曼压缩 Huffman
### 1. 文件夹结构
- project
	- huffman
		- huffman
			- decode.h 
			- encode.h
			- heap.h
			- huffnode.h
			- hufftree.h
			- main.cpp
		- huffman.xcodeproj
- test
	- httpd.conf
	- httpd.conf.huffman.0
	- httpd.conf.huffman.1
- huffman
- README.md

### 2. 使用环境
- MAC OS X
- zsh或bash
- 使用windows系统打开源文件，注释可能乱码或者有换行问题。实测VS2015和Sublime Text 2可以解决问题

### 3. 操作步骤
- 进入Terminal
- `$ cd <dir> #切换到程序所在的目录`
- `$ ./huffman -H #执行程序,查看帮助`
- `$ ./huffman -E test/httpd.conf #压缩httpd.conf文件`
- `$ ./huffman -D test/httpd.conf.huffman.0 #解压文件`
- `[-O <dir>]` 参数可以设置输出位置
