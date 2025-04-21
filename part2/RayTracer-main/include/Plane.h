#ifndef _PLANE_H_
#define _PLANE_H_

#include "Vec3.h"
#include "Ray.h"
#include "Object.h"
#include "Material.h"

class Plane : public Object {
public:
	Vec3f point; // point on the plane
	Vec3f normal; // normal vector of the plane and it should be normalized
    const Material* material;
    Plane(const Vec3f& pt, const Vec3f& n, const Material* mat)
        : point(pt), material(mat) {
        Vec3f norm = n;
        normal = norm.normalize();
    }
    bool hit(const Ray& r, HitRec& rec) const;
    const Material* getMaterial() const;
    void computeSurfaceHitFields(const Ray& r, HitRec& rec) const;
};
#endif
