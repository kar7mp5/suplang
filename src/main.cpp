#include <iostream>
#include <memory>
#include <string>

#include "AST/ASTNode.h"
#include "Interpreter/Environment.h"
#include "Interpreter/Interpreter.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

// A utility function to recursively print the AST for debugging purposes.
// This helps verify that the parser is building the correct structure.
void PrintAST(const suplang::ASTNode *node, int indent = 0) {
    if (!node) {
        return;
    }

    // Use indentation to represent the tree structure.
    std::cout << std::string(indent * 2, '-');

    if (auto p = dynamic_cast<const suplang::ProgramNode *>(node)) {
        std::cout << "[Program]\n";
        for (const auto &stmt : p->statements) {
            PrintAST(stmt.get(), indent + 1);
        }
    } else if (auto vd = dynamic_cast<const suplang::VarDeclNode *>(node)) {
        std::cout << "[VarDecl] Type: " << vd->varType << ", Name: " << vd->varName << "\n";
        PrintAST(vd->initialValue.get(), indent + 2);
    } else if (auto es = dynamic_cast<const suplang::ExpressionStatementNode *>(node)) {
        std::cout << "[ExprStmt]\n";
        PrintAST(es->expression.get(), indent + 1);
    } else if (auto is = dynamic_cast<const suplang::IfStatementNode *>(node)) {
        std::cout << "[IfStmt]\n";
        std::cout << std::string((indent + 1) * 2, ' ') << "[Condition]\n";
        PrintAST(is->condition.get(), indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "[Consequence]\n";
        PrintAST(is->consequence.get(), indent + 2);
        if (is->alternative) {
            std::cout << std::string((indent + 1) * 2, ' ') << "[Alternative]\n";
            PrintAST(is->alternative.get(), indent + 2);
        }
    } else if (auto bs = dynamic_cast<const suplang::BlockStatementNode *>(node)) {
        std::cout << "[BlockStmt]\n";
        for (const auto &stmt : bs->statements) {
            PrintAST(stmt.get(), indent + 1);
        }
    } else if (auto ie = dynamic_cast<const suplang::InfixExpressionNode *>(node)) {
        std::cout << "[InfixExpr] Op: " << ie->op << "\n";
        PrintAST(ie->left.get(), indent + 1);
        PrintAST(ie->right.get(), indent + 1);
    } else if (auto id = dynamic_cast<const suplang::IdentifierNode *>(node)) {
        std::cout << "[Identifier] " << id->value << "\n";
    } else if (auto nl = dynamic_cast<const suplang::NumberLiteralNode *>(node)) {
        std::cout << "[Number] " << nl->value << "\n";
    } else if (auto bl = dynamic_cast<const suplang::BooleanLiteralNode *>(node)) {
        std::cout << "[Boolean] " << (bl->value ? "true" : "false") << "\n";
    }
}

int main() {
    // The source code to be interpreted.
    std::string code = R"(
      int32 score = 85;
      int32 grade = 0;

      if (score > 90) {
          grade = 1;
      } elif (score == 85) {
          grade = 2;
      } else {
          grade = 3;
      }
  )";

    // 1. Lexing: Turn the source code string into a stream of tokens.
    suplang::Lexer lexer(code);

    // 2. Parsing: Build an Abstract Syntax Tree (AST) from the tokens.
    suplang::Parser parser(lexer);
    auto ast = parser.parseProgram();

    // 3. (Optional) Print the AST for debugging and verification.
    std::cout << "--- Abstract Syntax Tree ---\n";
    PrintAST(ast.get());
    std::cout << "--------------------------\n\n";

    // 4. Interpreting: Evaluate the AST to execute the code.
    suplang::Interpreter interpreter;
    auto env = std::make_shared<suplang::Environment>();
    interpreter.eval(ast.get(), env);

    // --- Verification ---
    std::cout << "--- Execution Result ---\n";
    auto result_obj = env->get("grade");
    if (result_obj && result_obj->type == suplang::ObjectType::INTEGER) {
        auto int_obj = std::dynamic_pointer_cast<suplang::IntegerObject>(result_obj);
        // Should print 2, because the 'elif' condition is met.
        std::cout << "Final grade is: " << int_obj->value << std::endl;
    } else {
        std::cout << "Variable 'grade' not found or not an integer." << std::endl;
    }

    return 0;
}