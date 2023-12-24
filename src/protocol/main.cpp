#include <iostream>
#include <memory>
#include <optional>
#include <utility>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "../animation.h"
#include "point.h"

int main()
{
    std::unique_ptr<Animation> animation;

    std::string line;
    while (std::getline(std::cin, line))
    {
        rapidjson::Document d;
        d.Parse(line.c_str());

        std::string_view method = d["method"].GetString();
        std::optional<std::uint32_t> id = std::nullopt;
        auto const &idValue = d.FindMember("id");
        if (idValue != d.MemberEnd() && idValue->value.IsUint())
        {
            id = idValue->value.GetUint();
        }

        if (method == "Initialize")
        {
            if (!id)
            {
                continue;
            }

            std::vector<Point> points;
            for (auto const &point : d["params"]["points"].GetArray())
            {
                auto const &v = point.GetArray();
                points.push_back(Point{v[0].GetDouble(), v[1].GetDouble(), v[2].GetDouble()});
            }
            animation = nullptr;
            animation = std::make_unique<Animation>(std::move(points));
            std::cout << "{\"id\":" << *id << ",\"result\":{}}" << std::endl;
        }
        else if (method == "AnimationName")
        {
            if (!id)
            {
                continue;
            }
            else if (!animation)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Animation was not initialized\"}" << std::endl;
            }
            else
            {
                auto animationName = animation->animationName();
                std::cout << "{\"id\":" << *id << ",\"result\":\"" << animationName << "\"}" << std::endl;
            }
        }
        else if (method == "ParameterSchema")
        {
            if (!id)
            {
                continue;
            }
            else if (!animation)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Animation was not initialized\"}" << std::endl;
            }
            else
            {
                auto parameterSchema = animation->parameterSchema();
                std::cout << "{\"id\":" << *id << ",\"result\":{\"parameters\":" << parameterSchema << "}}" << std::endl;
            }
        }
        else if (method == "SetParameters")
        {
            if (!id)
            {
                continue;
            }
            else if (!animation)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Animation was not initialized\"}" << std::endl;
            }
            else
            {
                animation->setParameters(Parameters(d["params"]["params"]));
                std::cout << "{\"id\":" << *id << ",\"result\":{}}" << std::endl;
            }
        }
        else if (method == "GetParameters")
        {
            if (!id)
            {
                continue;
            }
            else if (!animation)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Animation was not initialized\"}" << std::endl;
            }
            else
            {
                auto parameters = animation->getParameters().toString();
                std::cout << "{\"id\":" << *id << ",\"result\":" << parameters << "}" << std::endl;
            }
        }
        else if (method == "GetFps")
        {
            if (!id)
            {
                continue;
            }
            else if (!animation)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Animation was not initialized\"}" << std::endl;
            }
            else
            {
                auto fps = animation->fps();
                std::cout << "{\"id\":" << *id << ",\"result\":" << fps << "}" << std::endl;
            }
        }
        else if (method == "Update")
        {
            if (animation)
            {
                animation->update(d["params"]["time_delta"].GetDouble());
            }
        }
        else if (method == "OnEvent")
        {
            // Method OnEvent not supported, do nothing
            if (id)
            {
                std::cout << "{\"id\":" << *id << ",\"result\":{}}" << std::endl;
            }
        }
        else if (method == "Render")
        {
            if (!id)
            {
                continue;
            }
            else if (!animation)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Animation was not initialized\"}" << std::endl;
            }
            else
            {
                auto frame = animation->render();
                rapidjson::MemoryPoolAllocator<> a;
                rapidjson::Value frameValue(rapidjson::Type::kArrayType);
                for (auto &pixel : frame)
                {
                    rapidjson::Value pixelValue(rapidjson::Type::kObjectType);
                    pixelValue.AddMember(rapidjson::Value("r", a), rapidjson::Value(pixel.r), a);
                    pixelValue.AddMember(rapidjson::Value("g", a), rapidjson::Value(pixel.g), a);
                    pixelValue.AddMember(rapidjson::Value("b", a), rapidjson::Value(pixel.b), a);
                    frameValue.PushBack(pixelValue, a);
                }
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                frameValue.Accept(writer);

                std::cout << "{\"id\":" << *id << ",\"result\":{\"pixels\":" << buffer.GetString() << "}}" << std::endl;
            }
        }
        else
        {
            if (id)
            {
                std::cout << "{\"id\":" << *id << ",\"error\":\"Unknown method: " << method << "\"}" << std::endl;
            }
        }
    }
}