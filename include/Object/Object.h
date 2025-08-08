#ifndef SUPLANG_OBJECT_OBJECT_H_
#define SUPLANG_OBJECT_OBJECT_H_

#include "AST/ASTNode.h" // Required for function body and parameters.

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace suplang {

// Forward declaration to break the circular dependency with Environment.h.
class Environment;

// Enum for all possible object types in the language's runtime.
enum class ObjectType {
    INTEGER,
    BOOLEAN,
    FUNCTION,
    RETURN_VALUE,
};

// Base class for all runtime objects.
class Object {
  public:
    ObjectType type;
    virtual ~Object() = default;
};

// Represents an integer object at runtime.
class IntegerObject : public Object {
  public:
    explicit IntegerObject(int32_t val) : value(val) { type = ObjectType::INTEGER; }
    int32_t value;
};

// Represents a boolean object at runtime.
class BooleanObject : public Object {
  public:
    explicit BooleanObject(bool val) : value(val) { type = ObjectType::BOOLEAN; }
    bool value;
};

// Represents a function object at runtime.
class FunctionObject : public Object {
  public:
    // The constructor is only declared here; its definition is in Object.cpp
    // to avoid needing the full definition of Environment in this header.
    FunctionObject(std::vector<Parameter> params, std::unique_ptr<BlockStatementNode> body,
                   std::shared_ptr<Environment> env);

    std::vector<Parameter> parameters;
    std::unique_ptr<BlockStatementNode> body;
    std::shared_ptr<Environment> env;
};

// A wrapper object used to signal a return from a function call.
class ReturnValueObject : public Object {
  public:
    explicit ReturnValueObject(std::shared_ptr<Object> val) : value(val) { type = ObjectType::RETURN_VALUE; }
    std::shared_ptr<Object> value;
};

} // namespace suplang

#endif // SUPLANG_OBJECT_OBJECT_H_