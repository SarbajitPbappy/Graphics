#include<Plane.h>

void Plane::computeSurfaceHitFields(const Ray& r, HitRec& rec) const {
	// Compute the hit point
	rec.p = r.o + r.d * rec.tHit;
	// Compute the normal at the hit point
	rec.n = normal;
}
bool Plane::hit(const Ray& r, HitRec& rec) const {
	// Compute the denominator
	float denom = normal.dot(r.d);
	if (fabs(denom) > Ray::rayEps) {
		Vec3f p0l0 = point - r.o;
		rec.tHit = p0l0.dot(normal) / denom;
		if (rec.tHit >= Ray::rayEps && rec.tHit < r.tClip) {
			rec.anyHit = true;
			rec.material = getMaterial();
			computeSurfaceHitFields(r, rec);
			return true;
		}
	}
	return false;
}

const Material* Plane::getMaterial() const {
	return this->material;
}