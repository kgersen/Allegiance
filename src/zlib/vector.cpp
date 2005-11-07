#include "pch.h"

const Vector Vector::s_vectorZero(0, 0, 0);

Vector Vector::GetOrthogonalVector() const
{
    float ax = abs(x);
    float ay = abs(y);
    float az = abs(z);

    if (ax > ay) {
        if (ax > az) {
            //
            // x is biggest
            //

            return Vector(-z - y * z / x, z, x);
        }
    } else {
        if (ay > az) {
            //
            // y is biggest
            //

            return Vector(y, -x - z * x / y, x);
        }
    }

    //
    // z is biggest
    //

    return Vector(y, z, -y - x * y / z);
}

Vector Vector::RandomDirection(void)
{
    float sinPitch = random(-1.0f, 1.0f);
    float cosPitch = sqrt(1.0f - sinPitch * sinPitch);
    float yaw      = random(0.0f, 2.0f * pi);

    return 
        Vector(
            cos(yaw) * cosPitch,
            sin(yaw) * cosPitch,
            sinPitch
        );
}

Vector Vector::RandomPosition(float radius)
{
    float   rnd = random(0.0f, 1.0f);
    return  rnd == 0.0f
            ? Vector::GetZero()
            : Vector::RandomDirection() * (radius * pow(rnd, 1.0f/3.0f));
}
