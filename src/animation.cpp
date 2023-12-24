#include "animation.h"

#include <cmath>
#include <iostream>
#include <utility>

// TODO: Initialize animation with provided points and default parameter values.
// You can also include any other fields, but remember to add them to the class
// in `animation.h` file.
Animation::Animation(std::vector<Point> points_)
    : parameters(
          Parameters{}
              .withColor("primary", Color{255, 255, 255})
              .withNumber("peak", 1.0)
              .withEnumOption("function", "sin")),
      points(points_),
      time(0.0)
{
}

// TODO: Return animation name, which will show up in WebUI.
std::string Animation::animationName() const
{
    return "Animation Plugin Template";
}

// TODO: Return frames per second rate for your animation.
double Animation::fps() const
{
    return 30.0;
}

// TODO: Define parameter schema for your animation. These parameters will
// show up in WebUI and can be changed by the user.
std::string Animation::parameterSchema() const
{
    return ParameterSchema{}
        .with(ColorSchema{
            .id = "primary",
            .name = "Primary Color",
            .description = nullptr,
        })
        .with(NumberSchema{
            .id = "peak",
            .name = "Peak Brightness",
            .description = nullptr,
            .min = 0.0,
            .max = 1.0,
            .step = 0.05,
        })
        .with(EnumSchema{
            .id = "function",
            .name = "Function",
            .description = nullptr,
            .values = {
                EnumOptionSchema{
                    .id = "sin",
                    .name = "Sine"},
                EnumOptionSchema{
                    .id = "abs_sin",
                    .name = "Absolute Value of Sine"}}})
        .toString();
}

// You likely don't need to change this, unless you need to do any processing
// when parameters are updated. In any case, add your code after
// `this->parameters` is updated with new parameters.
void Animation::setParameters(Parameters parameters)
{
    this->parameters = std::move(parameters);
}

// You don't need to modify this function.
Parameters const &Animation::getParameters() const
{
    return this->parameters;
}

// TODO: Update your animation after `time_delta` seconds pass. Have in mind
// that `time_delta` can (and likely will) be a fractional value, since it's
// called `fps` times per second.
void Animation::update(double time_delta)
{
    this->time += time_delta;
}

// TODO: Produce a frame of your animation based on the current state of
// the animation. In general, you should not be modifying your animation
// in this function.
std::vector<Color> Animation::render() const
{
    std::vector<Color> result;
    result.reserve(this->points.size());

    Color color = this->parameters.getColor("primary");
    double peak = this->parameters.getNumber("peak");
    std::string_view function = this->parameters.getEnumOption("function");

    for (auto &p : points)
    {
        double value = std::sin((p.y + this->time) * 3.14);
        if (function == "sin")
        {
            value = (value + 1.0) / 2.0;
        }
        else
        {
            value = std::abs(value);
        }
        value *= peak;

        result.push_back(Color{
            static_cast<std::uint8_t>(color.r * value),
            static_cast<std::uint8_t>(color.g * value),
            static_cast<std::uint8_t>(color.b * value)});
    }

    return result;
}