#include "RayTracer.h"
#include "cmath"
#include "stdio.h"
#include "Features.h"
using namespace std;

void RayTracer::searchClosestHit(const Ray& ray, HitRec& hitRec) {
    hitRec.anyHit = false;
    hitRec.tHit = ray.tClip;
    int x = 0;
    bool hit = false;

    for (int i = 0; i < this->scene->objects.size(); i++) {
        hit = this->scene->objects[i]->hit(ray, hitRec);
        if (hit) {
            x = i;
        }
    }

    hitRec.primIndex = x;
    hitRec.material = this->scene->objects[hitRec.primIndex]->getMaterial();
    this->scene->objects[hitRec.primIndex]->computeSurfaceHitFields(ray, hitRec);
}

Vec3f RayTracer::getEyeRayDirection(int x, int y) {
    static float z = -5.0f;
    static float sizeX = 4.0f;
    static float sizeY = 3.0f;
    static float left = -sizeX * 0.5f;
    static float top = sizeY * 0.5f;
    static float dx = sizeX / float(this->image->getWidth());
    static float dy = sizeY / float(this->image->getHeight());

    return Vec3f((left + x * dx), (top - y * dy), z).normalize();
}

void RayTracer::fireRays() {
    Ray ray;

    for (int x = 0; x < this->image->getWidth(); x++) {
        for (int y = 0; y < this->image->getHeight(); y++) {
            ray.o = Vec3f(0.0f, 0.0f, 0.0f); // Assuming eye at origin
            ray.d = getEyeRayDirection(x, y);

            Vec3f color = traceRay(ray, 5);
            this->image->setPixel(x, y, color);
        }
    }
}

Vec3f RayTracer::traceRay(const Ray& ray, int depth) {
    if (depth <= 0) return bgColor;

    HitRec hitRec;
    searchClosestHit(ray, hitRec);

    if (hitRec.anyHit) {
        Vec3f localColor = Vec3f(0.0f, 0.0f, 0.0f);
        Vec3f reflectedColor = Vec3f(0.0f, 0.0f, 0.0f);
        bool inShadow = false;

        for (const Light* light : scene->lights) {
            localColor += colorComputation(ray, hitRec, light, inShadow);
        }

#if defined(REFLECTIONS)
        reflectedColor = calculateReflection(ray, hitRec, depth);
#endif

        Vec3f finalColor = localColor * (1.0f - hitRec.material->reflectionColor)
            + reflectedColor * hitRec.material->reflectionColor;

        finalColor.x = min(finalColor.x, 1.0f);
        finalColor.y = min(finalColor.y, 1.0f);
        finalColor.z = min(finalColor.z, 1.0f);

        return finalColor;
    }
    else {
        return bgColor;
    }
}

Vec3f RayTracer::calculateReflection(const Ray& ray, HitRec& hitRec, int depth) {
    if (hitRec.material->reflectionColor > 0.0f) {
        Vec3f N = hitRec.n;
        Vec3f reflectionDir = ray.d - N * (2.0f * ray.d.dot(N));

#ifdef FUZZY_NORMALS
        reflectionDir = randomlyPerturbNormals(reflectionDir, hitRec.material);
#endif

        Ray reflectionRay(hitRec.p + N * Ray::rayEps, reflectionDir);
        return traceRay(reflectionRay, depth - 1);
    }
    else {
        return Vec3f(0.0f, 0.0f, 0.0f);
    }
}

Vec3f RayTracer::randomlyPerturbNormals(Vec3f reflectionDir, const Material* mat) {
    float fuzz = mat->fuzziness;
    Vec3f randomVec(0.0f, 0.0f, 0.0f);

    if (fuzz > 0.0f) {
        randomVec = RayTracer::randomInUnitSphere();
    }

    return (reflectionDir + randomVec * fuzz).normalize();
}

Vec3f RayTracer::colorComputation(const Ray& ray, HitRec& hitRec, const Light* light, bool& shadow) {
    Vec3f N = hitRec.n;
    Vec3f L = (light->position - hitRec.p).normalize();

    Vec3f ambient(0.0f, 0.0f, 0.0f);
    Vec3f diffuse(0.0f, 0.0f, 0.0f);
    Vec3f specular(0.0f, 0.0f, 0.0f);
    Vec3f localColor(0.0f, 0.0f, 0.0f);

#if defined(AMBIENT_LIGHTING)
    ambient = hitRec.material->ambientColor.multCoordwise(light->ambient);
    localColor += ambient;
#endif

    bool inShadow = false;

#if defined(SHADOWS_BLACK) || defined(SHADOWS_AMBIENT)
    inShadow = isInShadow(N, hitRec, light->position);
#endif

    shadow = inShadow;

#if defined(SHADOWS_BLACK)
    if (inShadow) return Vec3f(0.0f, 0.0f, 0.0f);
#elif defined(SHADOWS_AMBIENT)
    if (inShadow) return ambient;
#endif

#if defined(DIFFUSE_LIGHTING)
    float diff = max(0.0f, N.dot(L));
    diffuse = hitRec.material->diffuseColor.multCoordwise(light->diffuse) * diff;
    localColor += diffuse;
#endif

#if defined(SPECULAR_LIGHTING)
    Vec3f V = (ray.o - hitRec.p).normalize();
    Vec3f R = (N * (2.0f * N.dot(L)) - L).normalize();
    float spec = pow(max(0.0f, R.dot(V)), hitRec.material->shineness);
    specular = hitRec.material->specularColor.multCoordwise(light->specular) * spec;
    localColor += specular;
#endif

    return localColor;
}

bool RayTracer::isInShadow(Vec3f N, HitRec& hitRec, Vec3f lightPosition) {
    Vec3f shadowOrigin = hitRec.p + N * Ray::rayEps;
    Vec3f L = (lightPosition - hitRec.p).normalize();
    Ray shadowRay(shadowOrigin, L);
    shadowRay.tClip = (lightPosition - shadowOrigin).len();

    HitRec shadowHitRec;
    searchClosestHit(shadowRay, shadowHitRec);

    return shadowHitRec.anyHit && shadowHitRec.tHit < shadowRay.tClip;
}
