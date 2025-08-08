#include <iostream>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"

int main() {
    // The source code to be interpreted
    std::string code = "int32 x = 10; int32 y = 5 + x * 2;";
    
    // 1. Lexing: Turn the code into tokens
    Lexer lexer(code);

    // 2. Parsing: Build an Abstract Syntax Tree (AST) from the tokens
    Parser parser(lexer);
    auto ast = parser.parseProgram();
    
    // 3. Interpreting: Evaluate the AST
    Interpreter interpreter;
    auto env = std::make_shared<Environment>(); // Create a new environment to store variables
    interpreter.eval(ast.get(), env);
    
    std::cout << code << std::endl;

    // --- Verification ---
    // Check if the variable 'y' was calculated and stored correctly
    auto result_obj = env->get("y");
    if (result_obj && result_obj->type == ObjectType::INTEGER) {
        auto int_obj = std::dynamic_pointer_cast<IntegerObject>(result_obj);
        // Should print 25 (5 + (10 * 2))
        std::cout << "Variable 'y' has value: " << int_obj->value << std::endl;
    } else {
        std::cout << "Variable 'y' not found or not an integer." << std::endl;
    }

    return 0;
}