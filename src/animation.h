#pragma once

#include <vector>
#include <string>

#include "parameters.h"
#include "point.h"

class Animation
{
public:
    explicit Animation(std::vector<Point> points_);

    std::string animationName() const;
    double fps() const;

    std::string parameterSchema() const;
    void setParameters(Parameters parameters);
    Parameters const &getParameters() const;

    void update(double time_delta);
    std::vector<Color> render() const;

private:
    Parameters parameters;

    // TODO: Add any additional data you need to keep track of.
    // Remember to set them in the constructor.
    std::vector<Point> points;
    double time;
};