#include "RayTracer.h"
#include "Vec3.h" 
#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
RayTracer* initTracer(int width, int height)
{
   RayTracer* rayTracer = new RayTracer(width, height);

   // Red Sphere
   const Material* redMaterial = new Material(
       Vec3f(0.1f, 0.0f, 0.0f),   // ambient
       Vec3f(0.6f, 0.0f, 0.0f),   // diffuse
       Vec3f(0.6f, 0.6f, 0.6f),   // specular
       64.0f                      // shininess
   );
   rayTracer->addSphere(Vec3f(0.0f, 0.0f, -10.0f), 1.0f, redMaterial);

   // Green Sphere
   const Material* greenMaterial = new Material(
       Vec3f(0.0f, 0.1f, 0.0f),   // ambient
       Vec3f(0.0f, 0.6f, 0.0f),   // diffuse
       Vec3f(0.6f, 0.6f, 0.6f),   // specular
       128.0f                      // shininess
   );
   rayTracer->addSphere(Vec3f(2.20f, 0.0f, -11.0f), 0.9f, greenMaterial);
   rayTracer->addPlane(
	   Vec3f(0.0f, -1.0f, 0.0f),   // normal
	   Vec3f(0.0f, 1.0f, 0.0f),  // point on plane
	   new Material(
		   Vec3f(0.0f, 0.0f, 0.3f), // ambient
		   Vec3f(0.0f, 0.0f, 0.4f), // diffuse
		   Vec3f(0.5f, 0.5f, 0.5f), // specular
		   32.0f              // shininess
	   )
   );
   rayTracer->addLight(
	   Vec3f(-10.0f, 6.0f, 10.0f), // light position
	   Vec3f(1.0f, 1.0f, 1.0f), // ambient light
	   Vec3f(1.0f, 1.0f, 1.0f), // diffuse light
	   Vec3f(1.0f, 1.0f, 1.0f)  // specular light
   );
   return rayTracer;
}

void runTracer(RayTracer* tracer)
{
	tracer->fireRays();
	tracer->toPPM("DVA338_Raytracer_output.ppm");
	tracer->toBMP("DVA338_Raytracer_output.bmp");
}

void destroyTracer(RayTracer* tracer)
{
	delete tracer;
}

int main(int argc, char **argv) {
	RayTracer* tracer = initTracer(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	runTracer(tracer);
	destroyTracer(tracer);
	return 0;
}
