#include "parameters.h"

#include <string>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

Color Parameters::getColor(char const *name, Color orDefault) const
{
    auto parameterValue = this->value.FindMember(name);
    if (parameterValue == this->value.MemberEnd() || !parameterValue->value.IsObject())
    {
        return orDefault;
    }

    auto parameter = parameterValue->value.GetObject();

    auto r = parameter.FindMember("r");
    if (r == parameter.MemberEnd() || !r->value.IsUint())
    {
        return orDefault;
    }

    auto g = parameter.FindMember("g");
    if (g == parameter.MemberEnd() || !g->value.IsUint())
    {
        return orDefault;
    }

    auto b = parameter.FindMember("b");
    if (b == parameter.MemberEnd() || !b->value.IsUint())
    {
        return orDefault;
    }

    return Color{
        static_cast<std::uint8_t>(r->value.GetUint() % 256),
        static_cast<std::uint8_t>(g->value.GetUint() % 256),
        static_cast<std::uint8_t>(b->value.GetUint() % 256)};
}

double Parameters::getNumber(char const *name, double orDefault) const
{
    auto parameterValue = this->value.FindMember(name);
    if (parameterValue == this->value.MemberEnd() || !parameterValue->value.IsNumber())
    {
        return orDefault;
    }

    return parameterValue->value.GetDouble();
}

std::string_view Parameters::getEnumOption(char const *name, std::string_view orDefault) const
{
    auto parameterValue = this->value.FindMember(name);
    if (parameterValue == this->value.MemberEnd() || !parameterValue->value.IsString())
    {
        return orDefault;
    }

    return parameterValue->value.GetString();
}

Parameters &Parameters::withColor(char const *name, Color color)
{
    auto &a = this->allocator;
    rapidjson::Value value(rapidjson::Type::kObjectType);
    value.AddMember(rapidjson::Value("r", a), rapidjson::Value(color.r), a);
    value.AddMember(rapidjson::Value("g", a), rapidjson::Value(color.g), a);
    value.AddMember(rapidjson::Value("b", a), rapidjson::Value(color.b), a);

    auto parameterValue = this->value.FindMember(name);
    if (parameterValue == this->value.MemberEnd())
    {
        this->value.AddMember(rapidjson::Value(name, a), value, a);
    }
    else
    {
        parameterValue->value = value;
    }

    return *this;
}

Parameters &Parameters::withNumber(char const *name, double number)
{
    auto &a = this->allocator;
    auto parameterValue = this->value.FindMember(name);
    if (parameterValue == this->value.MemberEnd())
    {
        this->value.AddMember(rapidjson::Value(name, a), rapidjson::Value(number), a);
    }
    else
    {
        parameterValue->value = rapidjson::Value(number);
    }
    return *this;
}

Parameters &Parameters::withEnumOption(char const *name, char const *option)
{
    auto &a = this->allocator;
    auto parameterValue = this->value.FindMember(name);
    if (parameterValue == this->value.MemberEnd())
    {
        this->value.AddMember(rapidjson::Value(name, a), rapidjson::Value(option, a), a);
    }
    else
    {
        parameterValue->value = rapidjson::Value(option, a);
    }
    return *this;
}

std::string Parameters::toString() const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    this->value.Accept(writer);

    return buffer.GetString();
}

ParameterSchema &ParameterSchema::with(ColorSchema color)
{
    auto &a = this->allocator;

    rapidjson::Value schemaValue(rapidjson::Type::kObjectType);
    schemaValue.AddMember(rapidjson::Value("type", a), rapidjson::Value("color", a), a);
    schemaValue.AddMember(rapidjson::Value("id", a), rapidjson::Value(color.id, a), a);
    schemaValue.AddMember(rapidjson::Value("name", a), rapidjson::Value(color.name, a), a);
    if (color.description)
    {
        schemaValue.AddMember(rapidjson::Value("description", a), rapidjson::Value(color.description, a), a);
    }
    this->values.PushBack(schemaValue, a);
    return *this;
}

ParameterSchema &ParameterSchema::with(NumberSchema number)
{
    auto &a = this->allocator;
    rapidjson::Value schemaValue(rapidjson::Type::kObjectType);
    schemaValue.AddMember(rapidjson::Value("type", a), rapidjson::Value("number", a), a);
    schemaValue.AddMember(rapidjson::Value("id", a), rapidjson::Value(number.id, a), a);
    schemaValue.AddMember(rapidjson::Value("name", a), rapidjson::Value(number.name, a), a);
    if (number.description)
    {
        schemaValue.AddMember(rapidjson::Value("description", a), rapidjson::Value(number.description, a), a);
    }
    schemaValue.AddMember(rapidjson::Value("min", a), rapidjson::Value(number.min), a);
    schemaValue.AddMember(rapidjson::Value("max", a), rapidjson::Value(number.max), a);
    schemaValue.AddMember(rapidjson::Value("step", a), rapidjson::Value(number.step), a);
    this->values.PushBack(schemaValue, a);
    return *this;
}

ParameterSchema &ParameterSchema::with(EnumSchema enumSchema)
{
    auto &a = this->allocator;
    rapidjson::Value schemaValue(rapidjson::Type::kObjectType);
    schemaValue.AddMember(rapidjson::Value("type", a), rapidjson::Value("enum", a), a);
    schemaValue.AddMember(rapidjson::Value("id", a), rapidjson::Value(enumSchema.id, a), a);
    schemaValue.AddMember(rapidjson::Value("name", a), rapidjson::Value(enumSchema.name, a), a);
    if (enumSchema.description)
    {
        schemaValue.AddMember(rapidjson::Value("description", a), rapidjson::Value(enumSchema.description, a), a);
    }

    rapidjson::Value enumOptions(rapidjson::Type::kArrayType);
    for (auto &option : enumSchema.values)
    {
        rapidjson::Value option_value(rapidjson::Type::kObjectType);
        option_value.AddMember(rapidjson::Value("value", a), rapidjson::Value(option.id, a), a);
        option_value.AddMember(rapidjson::Value("name", a), rapidjson::Value(option.name, a), a);
        enumOptions.PushBack(option_value, a);
    }
    schemaValue.AddMember(rapidjson::Value("values", a), enumOptions, a);
    this->values.PushBack(schemaValue, a);
    return *this;
}

std::string ParameterSchema::toString() const
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    this->values.Accept(writer);

    return buffer.GetString();
}