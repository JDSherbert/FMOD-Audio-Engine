// ©2023 JDSherbert. All rights reserved.

#pragma once

struct Vector3
{
public:

    Vector3
    (
        float newX = 0.0f, 
        float newY = 0.0f, 
        float newZ = 0.0f
    )
        : x(newX)
        , y(newY)
        , z(newZ)
    {
    };

    ~Vector3()
    {
    };

    float x;
    float y;
    float z;
};