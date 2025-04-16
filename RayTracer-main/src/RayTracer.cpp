#include "RayTracer.h"
#include "cmath"
#include "stdio.h"
#include "Features.h"
using namespace std;
void RayTracer::searchClosestHit(const Ray & ray, HitRec & hitRec) {
    hitRec.anyHit = false;
    hitRec.tHit = ray.tClip;
    for (int i = 0; i < this->scene->objects.size(); i++) {
       this->scene->objects[i]->hit(ray, hitRec);
    }
}
Vec3f RayTracer::getEyeRayDirection(int x, int y) {
    //Uses a fix camera looking along the negative z-axis
    static float z = -5.0f;		
    static float sizeX = 4.0f; 
    static float sizeY = 3.0f; 
    static float left = -sizeX * 0.5f;
    static float top = sizeY * 0.5f;
    static float dx =  sizeX / float(this->image->getWidth());  
    static float dy =  sizeY / float(this->image->getHeight());

    return Vec3f((left + x * dx), (top - y * dy), z).normalize();
}
void RayTracer::fireRays() {
    Ray ray;
    for(int x = 0; x < this->image->getWidth(); x++) {
        for(int y = 0; y < this->image->getHeight(); y++) {
            ray.d = RayTracer::getEyeRayDirection(x, y);
            RayTracer::searchClosestHit(ray, ray.hitRec);
            if (ray.hitRec.anyHit) {
				Vec3f color = Vec3f(0.0f, 0.0f, 0.0f); // hitRec color
                for (int i = 0;i < this->scene->lights.size();i++) {
					// compute light color
                    color += colorCalculation(ray, ray.hitRec, this->scene->lights[i]);
                }
				this->image->setPixel(x, y, color); // set pixel color
            }
            else {
					this->image->setPixel(x, y, this->bgColor); // set background color
                }

            }
        }
}

// Compute the color contribution from a light source
Vec3f RayTracer::colorCalculation(const Ray& ray, HitRec& hitRec, const Light* light) {
	Vec3f N = hitRec.n.normalize(); //surface normal at the hit point
	Vec3f L = (light->position - hitRec.p).normalize(); //light direction
    Vec3f color = Vec3f(0.0f, 0.0f, 0.0f); 
    // ambient lighting
#if defined(AMBIENT_LIGHTING)
    Vec3f ambient = hitRec.material->ambientColor.multCoordwise(light->ambient);
    color += ambient;
#endif
	// shadow calculation
    bool inShadow = false;
#if  defined(SHADOWS_BLACK) || defined(SHADOWS_AMBIENT)
	// Check if the hit point is in shadow
	// Create a shadow ray from the hit point to the light source
	// If the shadow ray hits any object before reaching the light, it's in shadow
	// shadow_black means no light contribution completely in shadow
	// shadow_ambient means ambient light contribution even in shadow
	Vec3f shadowOrigin = hitRec.p + N * Ray::rayEps; // avoiding self shadowing
    Ray shadowRay(shadowOrigin, L);
	shadowRay.tClip = (light->position - shadowOrigin).len(); // clip to light source (stop at light)

    HitRec shadowHitRec;
    searchClosestHit(shadowRay, shadowHitRec);
    if (shadowHitRec.anyHit && shadowHitRec.tHit < shadowRay.tClip) {
        inShadow = true;
    }
#endif
	// Handle shadows
#if  defined(SHADOWS_BLACK)
	if (inShadow) {
		// In shadow, no light contribution
		return Vec3f(0.0f, 0.0f, 0.0f);
#elif defined(SHADOWS_AMBIENT)
		// In shadow, only ambient light contribution
        return color;
#endif
	}
    // Only compute diffuse/specular if not in shadow (or if shadows are disabled)
    if (!inShadow) {
		// diffuse lighting
#if  defined(DIFFUSE_LIGHTING)
		float diff = max(0.0f, N.dot(L)); // cosine of angle between normal and light direction
        Vec3f diffuse = hitRec.material->diffuseColor.multCoordwise(light->diffuse) * diff;
        color += diffuse;
#endif

		// specular lighting
#if  defined(SPECULAR_LIGHTING)
		Vec3f V = (ray.o - hitRec.p).normalize(); //view direction
		Vec3f R = (N * (2.0f * N.dot(L)) - L).normalize(); //reflection direction
        float spec = pow(max(0.0f, R.dot(V)), hitRec.material->shineness);
        Vec3f specular = hitRec.material->specularColor.multCoordwise(light->specular) * spec;
        color += specular;
#endif
    }
    // Clamp color values to [0,1]
    color.x = min(color.x, 1.0f);
    color.y = min(color.y, 1.0f);
    color.z = min(color.z, 1.0f);

    return color;
}
