#ifndef SUPLANG_INTERPRETER_ENVIRONMENT_H_
#define SUPLANG_INTERPRETER_ENVIRONMENT_H_

#include "Object/Object.h"

#include <map>
#include <memory>
#include <string>

namespace suplang {

// Represents a scope for storing variables and their values during runtime.
class Environment {
  public:
    // Retrieves an object by name from the environment. Returns nullptr if not found.
    std::shared_ptr<Object> get(const std::string &name);

    // Stores an object with a given name in the environment.
    void set(const std::string &name, std::shared_ptr<Object> value);

  private:
    // The symbol table that maps variable names to their corresponding objects.
    std::map<std::string, std::shared_ptr<Object>> store_;
};

} // namespace suplang

#endif // SUPLANG_INTERPRETER_ENVIRONMENT_H_