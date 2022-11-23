#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework.h"
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "extra/hdre.h"

class Material {
public:

	Shader* shader = NULL;
	Texture* texture = NULL;
	vec4 color;
	Texture* LUT = NULL;
	vec4 c; 
	float threshold;
	float h;
	int ifVC_teapot1;
	bool ifPhong;
	bool ifVC;

	virtual void setUniforms(Camera* camera, Matrix44 model) = 0;
	virtual void render(Mesh* mesh, Matrix44 model, Camera * camera) = 0;
	virtual void renderInMenu() = 0;
};

class StandardMaterial : public Material {
public:

	StandardMaterial();
	~StandardMaterial();

	void setUniforms(Camera* camera, Matrix44 model);
	void render(Mesh* mesh, Matrix44 model, Camera * camera);
	void renderInMenu();
};

class WireframeMaterial : public StandardMaterial {
public:

	WireframeMaterial();
	~WireframeMaterial();

	void render(Mesh* mesh, Matrix44 model, Camera * camera);
};

class VolumeMaterial : public StandardMaterial {
public:
	VolumeMaterial();
	~VolumeMaterial();
	float length;
	bool ifLut;
	
	float brightness;
	Texture* blueNoise = NULL;
	
	void setUniforms(Camera* camera, Matrix44 model);
	void renderInMenu();

	vec4 clip;

	Vector3 ambient; //reflected ambient light
	Vector3 diffuse; //reflected diffuse light
	Vector3 specular; //reflected specular light
	float shininess; //glosiness coefficient (plasticity)



};
#endif