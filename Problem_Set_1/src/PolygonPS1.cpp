#include "Polygon.h"

float Polygon::getSignedArea() const
{
    float area = 0.0f;

    // Shoelace formula
    for (size_t i = 0; i < fNumberOfVertices; ++i)
    {
        const Vector2D& current = fVertices[i];
        const Vector2D& next = fVertices[(i + 1) % fNumberOfVertices];

        area += current.getX() * next.getY() - next.getX() * current.getY();
    }

    return 0.5f * area;
}
