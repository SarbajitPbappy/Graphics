#ifndef _RayTracer_H_
#define _RayTracer_H_

#include "Scene.h"
#include "Image.h"
#include "Ray.h"
#include "Sphere.h"

class RayTracer {
private: 
	Scene * scene;
	Image * image;
    Vec3f bgColor;
public:
	// Constructor
    RayTracer(const int w, const int h) {
        this->scene = new Scene();
        this->image = new Image(w, h);
        this->bgColor = Vec3f(0.60f, 0.60f, 0.60f);
    }
    ~RayTracer() {
        delete scene;
        delete image;
    }
    void searchClosestHit(const Ray & ray, HitRec & hitRec);
	//Vec3f traceRay(const Ray& ray, int depth);
	Vec3f getEyeRayDirection(int x, int y);
    void fireRays();

    Vec3f colorCalculation(const Ray& ray, HitRec& hitRec, const Light* light);
    void toPPM(const char* path) {
        this->image->toPPM(path);
    };
    void toBMP(const char* path) {
        this->image->toBMP(path);
    };
    void addSphere(const Vec3f & c, const float r, const Material* mat) {
        if (r >= 0) {
            this->scene->addSphere(c, r, mat);
        }
    };
    void addLight(const Vec3f& pos, const Vec3f& ambient,const Vec3f& diffuse, const Vec3f& specular) {
        this->scene->addLight(pos, ambient, diffuse, specular);
    };
    void addPlane(const Vec3f& point, const Vec3f& normal, const Material* mat) {
        this->scene->addPlane(point, normal, mat);
    }
};

#endif