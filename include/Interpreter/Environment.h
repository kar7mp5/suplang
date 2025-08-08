#ifndef SUPLANG_INTERPRETER_ENVIRONMENT_H_
#define SUPLANG_INTERPRETER_ENVIRONMENT_H_

#include <map>
#include <memory>
#include <string>

namespace suplang {

// Forward declaration to break the circular dependency with Object.h.
class Object;

// Represents a scope for storing variables and their values during runtime.
// Supports nesting to create local scopes for functions.
class Environment {
  public:
    Environment() = default;
    // Creates a new, enclosed environment for a function call.
    explicit Environment(std::shared_ptr<Environment> outer) : outer_(outer) {}

    // Retrieves an object by name. If not found in the current scope, it
    // recursively searches in the outer scope.
    std::shared_ptr<Object> get(const std::string &name);

    // Stores an object with a given name in the current scope.
    void set(const std::string &name, std::shared_ptr<Object> value);

  private:
    std::map<std::string, std::shared_ptr<Object>> store_;
    std::shared_ptr<Environment> outer_ = nullptr;
};

} // namespace suplang

#endif // SUPLANG_INTERPRETER_ENVIRONMENT_H_