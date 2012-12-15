#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include <SFML/System/Vector2.hpp>

/// Pixels to units ratio
const float PTU = 32.f;

/// Constant for PI.
const float PI		= 3.14159265359f;

/// Constant for reciprocal of PI.
const float RECIPROCAL_PI	= 1.0f/PI;

/// Constant for half of PI.
const float HALF_PI	= PI/2.0f;

/// 32bit Constant for converting from degrees to radians
const float DEGTORAD = PI / 180.0f;

/// 32bit constant for converting from radians to degrees (formally known as GRAD_PI)
const float RADTODEG   = 180.0f / PI;

/// Utility function to convert pixel coordinates to world coordinates
inline sf::Vector2f screenToWorld(sf::Vector2i screen)
{
    return sf::Vector2f(screen.x,-screen.y)/PTU;
}

/// Utility function to convert world coordinates to screen coordinates
inline sf::Vector2f worldToScreen(sf::Vector2i screen)
{
    return sf::Vector2f(screen.x,-screen.y)*PTU;
}

/// Utility function to convert a radian value to degrees
/** Provided as it can be clearer to write radToDeg(X) than RADTODEG * X
\param radians	The radians value to convert to degrees.
*/
inline float radToDeg(float radians)
{
    return RADTODEG * radians;
}

/// Utility function to convert a degrees value to radians
/** Provided as it can be clearer to write degToRad(X) than DEGTORAD * X
\param degrees	The degrees value to convert to radians.
*/
inline float degToRad(float degrees)
{
    return DEGTORAD * degrees;
}

#endif // MATH_H_INCLUDED
