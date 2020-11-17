# iSula-libutils

#### 介绍
iSula-libutils为iSula平台提供日志、json解析以及通用的公共方法的工具类的函数库。

本项目停止维护
为了简化iSula相关项目，把isula-libutils和lcr两个仓库进行合并，因此，后续isula-libutils将停止维护。
相关代码都已经合入lcr项目中: https://gitee.com/openeuler/lcr


#### 安装教程

```
$ mkdir build
$ cd build/
$ cmake ../
$ make -j
$ sudo make install
```

LLT测试:

```
$ cd build/tests/
$ ctest
```

#### 使用说明

1. 提供动态库：libisula_libutils.so
2. 提供头文件：日志相关和json解析相关


#### 参与贡献

1.  Fork iSula-libutils
2.  新建分支
3.  提交代码
4.  新建 Pull Request

