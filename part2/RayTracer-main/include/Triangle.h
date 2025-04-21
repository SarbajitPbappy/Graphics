#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object.h"
#include "Material.h"

class Triangle : public Object {
public:
    Vec3f vertex0, vertex1, vertex2;   // Triangle vertices
    Vec3f normal;                      // Surface normal
    const Material* matPtr;           // Pointer to the material

    // Constructor
    Triangle(const Vec3f& a, const Vec3f& b, const Vec3f& c, const Material* mat)
        : vertex0(a), vertex1(b), vertex2(c), matPtr(mat)
    {
        // Compute the normalized normal vector of the triangle
        normal = ((vertex1 - vertex0).cross(vertex2 - vertex0)).getNormalized();
    }

    // Intersection test with ray
    bool hit(const Ray& ray, HitRec& record) const override;

    // Return the material of the triangle
    const Material* getMaterial() const override { return matPtr; }

    // Compute fields of HitRec after intersection
    void computeSurfaceHitFields(const Ray& ray, HitRec& record) const override;
};

#endif // TRIANGLE_H
