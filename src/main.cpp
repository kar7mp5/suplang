#include <iostream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

// AST를 출력하여 구조를 확인하는 함수
void printAST(ASTNode* node, int indent = 0) {
    if (!node) return;

    for (int i = 0; i < indent; ++i) std::cout << "  ";

    if (auto p = dynamic_cast<ProgramNode*>(node)) {
        std::cout << "[Program]\n";
        for (const auto& stmt : p->statements) {
            printAST(stmt.get(), indent + 1);
        }
    } else if (auto vd = dynamic_cast<VarDeclNode*>(node)) {
        std::cout << "[VarDecl] Type: " << vd->varType << ", Name: " << vd->varName << "\n";
        for (int i = 0; i < indent + 1; ++i) std::cout << "  ";
        std::cout << "Value: \n";
        printAST(vd->initialValue.get(), indent + 2);
    } else if (auto nl = dynamic_cast<NumberLiteralNode*>(node)) {
        std::cout << "[Number] " << nl->value << "\n";
    }
}


int main() {
    std::string code = "int32 x = 100;";
    
    Lexer lexer(code);
    Parser parser(lexer);

    auto ast = parser.parseProgram();

    std::cout << "--- Abstract Syntax Tree ---\n";
    printAST(ast.get());
    std::cout << "--------------------------\n";

    return 0;
}