#include "Interpreter/Environment.h"

namespace suplang {

std::shared_ptr<Object> Environment::get(const std::string &name) {
    if (store_.count(name)) {
        return store_[name];
    }
    return nullptr; // Return null if the variable is not found.
}

void Environment::set(const std::string &name, std::shared_ptr<Object> value) { store_[name] = value; }

} // namespace suplang