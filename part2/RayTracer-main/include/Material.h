#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Vec3.h"

class Material {
public:
	Vec3f ambientColor;
	Vec3f diffuseColor;
	Vec3f specularColor;
	float shineness;
	float reflectionColor;
	float refractionIndex;
	float fuzziness;
	float transparency;

public:
	Material(const Vec3f& ambient, const Vec3f& diffuse, const Vec3f& specular, float shine, float reflection = 0.0f, float fuzzy = 0.0f, float transp = 0.0f, float refract = 1.5f)
		: ambientColor(ambient), diffuseColor(diffuse), specularColor(specular), shineness(shine),
		reflectionColor(reflection), fuzziness(fuzzy), transparency(transp), refractionIndex(refract)
	{
	}

};
#endif