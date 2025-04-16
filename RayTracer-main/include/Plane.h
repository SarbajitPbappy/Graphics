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
    bool hit(const Ray& ray, HitRec& rec) const override {
		// Check if the ray intersects the plane
		float denom = normal.dot(ray.d); // denominator
        // If denom is close to 0, ray is parallel to the plane
		if (fabs(denom) > Ray::rayEps) { // ensure denom is not zero and not parallel to the plane
            // Plane equation: n • p = -D
            float D = -(normal.dot(point));
            // t= -(D+n.o)/n.d
            float t = -(D + normal.dot(ray.o)) / denom;
            if (t > 0 && t < rec.tHit && t < ray.tClip) {
                rec.tHit = t;
                rec.p = ray.o + ray.d * t;
                rec.n = normal;
                rec.material = material;
                rec.anyHit = true;
                return true;
            }
        }
        return false;
    }
};
#endif
