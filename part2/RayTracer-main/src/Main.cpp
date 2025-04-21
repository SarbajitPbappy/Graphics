#include "RayTracer.h"
#include "Vec3.h"
#include "Features.h"

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480

RayTracer* initTracer(int width, int height)
{
	RayTracer* rayTracer = new RayTracer(width, height);

	// Green Sphere
	rayTracer->addSphere(Vec3f(2.25f, -0.05f, -10.0f), 0.95f, new Material(
		Vec3f(0.0f, 0.1f, 0.0f),   // ambient
		Vec3f(0.1f, 0.6f, 0.1f),   // diffuse
		Vec3f(0.6f, 0.6f, 0.6f),   // specular
		5000.0f,                   // shininess
		0.5f,                      // reflectivity
		0.05f                      // fuzziness
	));

	// Red Sphere
	rayTracer->addSphere(Vec3f(0.0f, -0.05f, -10.0f), 1.0f, new Material(
		Vec3f(0.1f, 0.0f, 0.0f),   // ambient
		Vec3f(0.6f, 0.1f, 0.1f),   // diffuse
		Vec3f(0.06f, 0.06f, 0.06f),// specular
		4000.0f,                   // shininess
		0.5f                       // reflectivity
	));

	// Yellow Light Source Sphere
	rayTracer->addSphere(Vec3f(-20.0f, 20.0f, -7.0f), 7.0f, new Material(
		Vec3f(0.6f, 0.6f, 0.0f),   // ambient
		Vec3f(0.9f, 0.9f, 0.0f),   // diffuse
		Vec3f(0.5f, 0.5f, 0.5f),   // specular
		5000.0f,                   // shininess
		0.5f                       // reflectivity
	));

#ifdef TRIANGLES
	// Red Triangle
	rayTracer->addTriangle(
		Vec3f(-0.8f, 1.0f, -11.0f),
		Vec3f(-2.5f, 3.4f, -11.0f),
		Vec3f(-0.8f, 3.4f, -11.0f),
		new Material(
			Vec3f(0.2f, 0.0f, 0.0f),
			Vec3f(0.5f, 0.0f, 0.0f),
			Vec3f(0.2f, 0.2f, 0.2f),
			4000.0f,
			0.5f
		)
	);

	// Green Triangle
	rayTracer->addTriangle(
		Vec3f(-0.8f, 1.0f, -11.0f),
		Vec3f(0.9f, 3.4f, -11.0f),
		Vec3f(-0.8f, 3.4f, -11.0f),
		new Material(
			Vec3f(0.0f, 0.2f, 0.0f),
			Vec3f(0.0f, 0.5f, 0.0f),
			Vec3f(0.2f, 0.2f, 0.2f),
			4000.0f,
			0.5f
		)
	);
#endif

	// Ground Sphere
	const Material* blueGround = new Material(
		Vec3f(0.02f, 0.02f, 0.05f), // ambient
		Vec3f(0.1f, 0.1f, 0.6f),    // diffuse
		Vec3f(0.0f, 0.0f, 0.0f),    // specular
		10.0f,                      // shininess
		0.75f                       // reflectivity (slight)
	);
	rayTracer->addSphere(Vec3f(0.0f, -1001.0f, -10.0f), 1000.0f, blueGround);

	// Light Source
	rayTracer->addLight(
		Vec3f(-10.0f, 5.0f, 10.0f),
		Vec3f(1.0f, 1.0f, 1.0f), // ambient
		Vec3f(1.0f, 1.0f, 1.0f), // diffuse
		Vec3f(1.0f, 1.0f, 1.0f)  // specular
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

int main(int argc, char** argv)
{
	RayTracer* tracer = initTracer(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	runTracer(tracer);
	destroyTracer(tracer);
	return 0;
}
