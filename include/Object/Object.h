#pragma once
#include <string>
#include <cstdint>

// Enum for all possible object types in the language
enum class ObjectType {
    INTEGER,
    // Add BOOL, FLOAT, STRING etc. later
};

// Base class for all objects
class Object {
public:
    ObjectType type;
    virtual ~Object() = default;
};

// Represents an integer object
class IntegerObject : public Object {
public:
    int32_t value;
    IntegerObject(int32_t val) : value(val) { type = ObjectType::INTEGER; }
};