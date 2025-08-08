#include "Object/Object.h"

// This .cpp file needs the full definition of Environment to implement the
// FunctionObject constructor.
#include "Interpreter/Environment.h"

namespace suplang {

FunctionObject::FunctionObject(std::vector<Parameter> params, std::unique_ptr<BlockStatementNode> body,
                               std::shared_ptr<Environment> env)
    : parameters(std::move(params)), body(std::move(body)), env(env) {
    type = ObjectType::FUNCTION;
}

} // namespace suplang