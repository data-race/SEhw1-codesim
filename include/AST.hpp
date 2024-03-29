#pragma once
#include <iostream>
#include <clang-c/Index.h>
#include <vector>
using namespace std;

class ASTNode {
private:
    CXCursorKind nodeKind;
    string kindName;
    vector<ASTNode*> children;
    int depth;
public:
    CXCursorKind getNodeKind();
    string getKindName();
    int getDepth();
    void setDepth(int);
    void addChild(ASTNode* node);
    // 构造函数: 根据传入的kind来设置变量和名称
    ASTNode(CXCursorKind kind) {
        this->nodeKind = kind;
        this->kindName = clang_getCString(clang_getCursorKindSpelling(kind));
        this->children = vector<ASTNode*>();
        // cout << "new node: " << this->kindName << endl;
    }

    // 递归打印AST
    void outputASTNode();
    
    // 前序遍历生成Sequence
    vector<string> tranverse();
    
};

