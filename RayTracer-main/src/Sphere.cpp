
#include "Sphere.h"
bool Sphere::hit(const Ray& r, HitRec& rec) const {

	//Now we need to find out the t
	Vec3f oc = r.o - c;
	float a = r.d.dot(r.d);
	float b = 2.0f*oc.dot(r.d);
	float c = oc.dot(oc) - (this->r*this->r);

	float det = b * b - 4.0f * a * c;

	if (det < 0) {
		return false;
	}

	float sqrtDiscriminant = static_cast<float>(sqrt(det));
	float t1 = (-b + sqrtDiscriminant) / (2.0f * a);
	float t2 = (-b - sqrtDiscriminant) / (2.0f * a);
	float t = t1;
	if (t < 0) {
		t = t2;
		if (t < 0)
		{
			return false;
		}
	}
	if (t1 > 0 && t2 > 0) {
		t = t1 < t2 ? t1 : t2;
	}
	if (t > r.tClip || (rec.anyHit && t >rec.tHit)) {
		return false;
	}
	rec.tHit = t;
	rec.anyHit = true;

	rec.material=Sphere::getMaterial();

	Sphere::computeSurfaceHitFields(r, rec);

	return true;
}


void Sphere::computeSurfaceHitFields(const Ray & r, HitRec & rec) const {
	rec.p = r.o + r.d * rec.tHit;
	rec.n = (rec.p - c).normalize();
}

const Material* Sphere::getMaterial() const {
	return material;
}
