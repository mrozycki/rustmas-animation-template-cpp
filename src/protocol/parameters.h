#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string_view>

#include "rapidjson/document.h"

struct Color
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
};

class Parameters
{
private:
    rapidjson::MemoryPoolAllocator<> allocator;
    rapidjson::Value value;

public:
    Parameters() : value(rapidjson::Type::kObjectType) {}
    explicit Parameters(rapidjson::Value const &value_) : value(value_, this->allocator) {}
    Parameters(Parameters const &other) : value(other.value, this->allocator) {}
    Parameters &operator=(Parameters const &other)
    {
        this->value = rapidjson::Value(other.value, this->allocator);
        return *this;
    }

    Color getColor(char const *name, Color orDefault = Color{0, 0, 0}) const;
    double getNumber(char const *name, double orDefault = 0.0) const;
    std::string_view getEnumOption(char const *name, std::string_view orDefault = {}) const;

    Parameters &withColor(char const *name, Color color);
    Parameters &withNumber(char const *name, double number);
    Parameters &withEnumOption(char const *name, char const *option);

    std::string toString() const;
};

struct ColorSchema
{
    char const *id;
    char const *name;
    char const *description;
};

struct NumberSchema
{
    char const *id;
    char const *name;
    char const *description;
    double min;
    double max;
    double step;
};

struct EnumOptionSchema
{
    char const *id;
    char const *name;
};

struct EnumSchema
{
    char const *id;
    char const *name;
    char const *description;
    std::vector<EnumOptionSchema> values;
};

class ParameterSchema
{
private:
    rapidjson::MemoryPoolAllocator<> allocator;
    rapidjson::Value values;

public:
    ParameterSchema() : values(rapidjson::Type::kArrayType) {}
    ParameterSchema &with(ColorSchema colorSchema);
    ParameterSchema &with(NumberSchema numberSchema);
    ParameterSchema &with(EnumSchema enumSchema);

    std::string toString() const;
};
