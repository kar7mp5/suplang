#ifndef SUPLANG_OBJECT_OBJECT_H_
#define SUPLANG_OBJECT_OBJECT_H_

#include <cstdint>
#include <string>

namespace suplang {

// Enum for all possible object types in the language's runtime.
enum class ObjectType {
    INTEGER,
    BOOLEAN,
    // Add FLOAT, STRING etc. later.
};

// Base class for all runtime objects, enabling polymorphism.
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

} // namespace suplang

#endif // SUPLANG_OBJECT_OBJECT_H_