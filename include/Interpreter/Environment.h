#pragma once
#include "Object/Object.h"
#include <string>
#include <map>
#include <memory>

// Represents a scope for storing variables
class Environment {
public:
    std::shared_ptr<Object> get(const std::string& name);
    void set(const std::string& name, std::shared_ptr<Object> value);

private:
    // Symbol table to store variable names and their corresponding objects
    std::map<std::string, std::shared_ptr<Object>> store;
};