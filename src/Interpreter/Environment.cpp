#include "Interpreter/Environment.h"

std::shared_ptr<Object> Environment::get(const std::string& name) {
    if (store.count(name)) {
        return store[name];
    }
    return nullptr; // Return null if variable is not found
}

void Environment::set(const std::string& name, std::shared_ptr<Object> value) {
    store[name] = value;
}