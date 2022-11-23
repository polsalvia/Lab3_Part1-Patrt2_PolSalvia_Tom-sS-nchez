#include "material.h"
#include "texture.h"
#include "application.h"
#include "extra/hdre.h"

StandardMaterial::StandardMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

StandardMaterial::~StandardMaterial()
{

}

void StandardMaterial::setUniforms(Camera* camera, Matrix44 model)
{
	//upload node uniforms
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_output", Application::instance->output);

	shader->setUniform("u_color", color);
	shader->setUniform("u_exposure", Application::instance->scene_exposure);

	if (texture)
		shader->setUniform("u_texture", texture);
}

void StandardMaterial::render(Mesh* mesh, Matrix44 model, Camera* camera)
{
	if (mesh && shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}

void StandardMaterial::renderInMenu()
{
	ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
}


VolumeMaterial::VolumeMaterial()
{
	//boundary in local coordinates is from [-1,-1,-1] to [1,1,1]
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/volume.fs");
	length = 0.01;
	brightness = 1.0;
	ifLut = false;
	ifPhong = false;
	ifVC_teapot1 = 0;
	ifVC = false;
	h = 0.01;
	threshold = 0.150;
	clip = vec4(0.0, 0.0, 0.5, 1.0);
	ambient.set(1, 0, 0); //reflected ambient light
	diffuse.set(1, 1, 1); //reflected diffuse light
	specular.set(1, 1, 1); //reflected specular light
	shininess = 30.0; //glosiness coefficient (plasticity)
}

VolumeMaterial::~VolumeMaterial()
{
	

}

void VolumeMaterial::renderInMenu()
{
	ImGui::ColorEdit3("Color", (float*)&color); // Edit 3 floats representing a color
	ImGui::DragFloat("Length step Vector", (float*)&length,0.001,0.0001,0.05);
	ImGui::DragFloat("Brightness", (float*)&brightness, 0.01, 0.1, 10);
	ImGui::Checkbox("Transfer function", &ifLut);
	ImGui::Checkbox("Apply phong", &ifPhong);
	if (ifPhong == true) {
		ImGui::DragFloat("h value", (float*)&h, 0.001, 0.001, 0.05);
		ImGui::DragFloat("threshold", (float*)&threshold, 0.01, 0.0, 1.0);
	}
	//ImGui::Checkbox("Transfer function?", &ifLut);
}

void VolumeMaterial::setUniforms(Camera* camera, Matrix44 model) {


	Matrix44 model_inv = model.m;
	model_inv.inverse();
	shader->setUniform("u_length", length);
	shader->setUniform("u_brie", brightness);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_camera_position", camera->eye);
	shader->setUniform("u_model", model);
	shader->setUniform("u_model_inv", model_inv);
	shader->setUniform("u_time", Application::instance->time);
	shader->setUniform("u_output", Application::instance->output);

	shader->setUniform("u_color", color);
	shader->setUniform("u_exposure", Application::instance->scene_exposure);


	shader->setUniform("u_sampler3D", texture,0);
	shader->setUniform("u_blueNoise", blueNoise,1);
	shader->setUniform("u_LUT", LUT,2);


	shader->setUniform("clip", clip);
	shader->setUniform("ifLut", ifLut);
	shader->setUniform("ifPhong", ifPhong);
	shader->setUniform("h", h);
	shader->setUniform("threshold", threshold);
	shader->setUniform("c", c);

	//phong
	//material
	shader->setUniform("ambient_material", ambient);
	shader->setUniform("diffuse_material", diffuse);
	shader->setUniform("specular_material", specular);
	shader->setUniform("shininess", shininess);

	//light

	shader->setUniform("ambient_light", Application::instance->light->ambient_light);
	shader->setUniform("diffuse_light", Application::instance->light->diffuse_light);
	shader->setUniform("specular_light", Application::instance->light->specular_light);
	shader->setUniform("position_light", Application::instance->light->position_light);

}



WireframeMaterial::WireframeMaterial()
{
	color = vec4(1.f, 1.f, 1.f, 1.f);
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
}

WireframeMaterial::~WireframeMaterial()
{

}

void WireframeMaterial::render(Mesh* mesh, Matrix44 model, Camera * camera)
{
	if (shader && mesh)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//enable shader
		shader->enable();

		//upload material specific uniforms
		setUniforms(camera, model);

		//do the draw call
		mesh->render(GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}