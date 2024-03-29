#include "ASTParser.hpp"
#include <iostream>
#include "Logger.hpp"

using namespace std;

CXChildVisitResult visitor(CXCursor c, CXCursor parent, CXClientData clientData);

ostream &operator<<(ostream &stream, const CXString &str)
{
    stream << clang_getCString(str);
    clang_disposeString(str);
    return stream;
}


void ASTParser::parseTheAST()
{
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        this->fileName.c_str(),
        nullptr,
        0,
        nullptr,
        0,
        CXTranslationUnit_None);

    if (unit == nullptr)
    {
        Logger::error("Unable to parse translation unit. Aborting.\n");
        exit(-1);
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(
        cursor,
        visitor,
        this);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}

// 把ASTParser当作ClientData传入
CXChildVisitResult visitor(CXCursor c, CXCursor parent, CXClientData clientData)
{ 
    // cout << cursor2String(c) << endl;
    ASTNode *newNode = new ASTNode(clang_getCursorKind(c));
    ASTParser* parser = (ASTParser*) clientData;
    // exclude the headers
    if(clang_Location_isFromMainFile(clang_getCursorLocation(c)) == 0)
    {
        return CXChildVisit_Continue;
    }

    if (clang_getCursorKind(parent) == CXCursor_TranslationUnit)
    {
        Logger::info("Add an edge: Parent:", clang_getCursorKindSpelling(clang_getCursorKind(parent)), "|","Children:",clang_getCursorKindSpelling(clang_getCursorKind(c)), "\n");
        
        parser->cursorNodeMap.insert(pair<unsigned, ASTNode *>(clang_hashCursor(c), newNode));
        newNode->setDepth(parser->root->getDepth() + 1);
        parser->root->addChild(newNode);
    }
    else
    {
        ASTNode *parentNode = parser->cursorNodeMap.at(clang_hashCursor(parent));
        newNode->setDepth(parentNode->getDepth() + 1);
        parser->cursorNodeMap.insert(pair<unsigned, ASTNode *>(clang_hashCursor(c), newNode));
        parentNode->addChild(newNode);
        Logger::info("Add an edge: Parent:", clang_getCursorKindSpelling(clang_getCursorKind(parent)), "|","Children:",clang_getCursorKindSpelling(clang_getCursorKind(c)), "\n");
    }
    return CXChildVisit_Recurse;
}