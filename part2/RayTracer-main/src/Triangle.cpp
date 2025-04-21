#include "Triangle.h"

bool Triangle::hit(const Ray& ray, HitRec& rec) const {
   // First check if ray is parallel to triangle plane
   float denom = normal.dot(ray.d);
   if (fabs(denom) < 1e-6f) return false;

   // Compute intersection distance t
   float D = -normal.dot(vertex0); // Use vertex0 instead of undefined v0
   float t = -(D + normal.dot(ray.o)) / denom;

   // Check if intersection is within valid range
   if (t < 1e-4f || t >= ray.tClip || (rec.anyHit && t >= rec.tHit)) {
       return false;
   }

   // Compute intersection point
   Vec3f p = ray.o + ray.d * t;

   // Edge tests to determine if point is inside triangle
   Vec3f edge0 = vertex1 - vertex0;
   Vec3f edge1 = vertex2 - vertex1;
   Vec3f edge2 = vertex0 - vertex2;

   Vec3f c0 = edge0.cross(p - vertex0);
   Vec3f c1 = edge1.cross(p - vertex1);
   Vec3f c2 = edge2.cross(p - vertex2);

   // Check if point is inside all edges
   if (normal.dot(c0) >= 0 && normal.dot(c1) >= 0 && normal.dot(c2) >= 0) {
       // Only update hit record if this is the closest hit so far
       if (!rec.anyHit || t < rec.tHit) {
           rec.tHit = t;
           rec.anyHit = true;
           rec.material = getMaterial();
           computeSurfaceHitFields(ray, rec);
           return true;
       }
   }
   return false;
}