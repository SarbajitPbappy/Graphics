#ifndef _HITREC_H_
#define _HITREC_H_

#include "Vec3.h"

class Material; // Forward declaration

struct HitRec {
	float tHit; // distance to the hit point
	int primIndex; // index of the primitive that was hit
	Vec3f p; // hit point
	Vec3f n; // normal at the hit point
	Vec3f color; // color at the hit point
	bool anyHit; // true if a hit occurred
	const Material* material; // material of the hit object
	Vec3f normal; // normal at the hit point
};

#endif
