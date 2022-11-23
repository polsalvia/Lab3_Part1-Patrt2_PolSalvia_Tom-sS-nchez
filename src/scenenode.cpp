#include "scenenode.h"
#include "application.h"
#include "texture.h"
#include "utils.h"
#include "volume.h"

unsigned int SceneNode::lastNameId = 0;
unsigned int mesh_selected = 3;

SceneNode::SceneNode()
{
	this->name = std::string("Node" + std::to_string(lastNameId++));
}


SceneNode::SceneNode(const char * name)
{
	this->name = name;
}

SceneNode::~SceneNode()
{

}

Light::Light()
{
	this->name = "Light";
	position_light.set(50, 50, 0); //where is the light
	intesity_light = 1;
	color_light.set(1, 1, 1);
	diffuse_light.set(0.6f, 0.6f, 0.6f);
	specular_light.set(0.6f, 0.6f, 0.6f);
	ambient_light.set(0, 0.1f, 0);


		
}
Light::~Light()
{

}

void Light::renderInMenu()
{
	ImGui::DragFloat3("Position", (float*)&this->position_light, 0.1f);


}


void SceneNode::render(Camera* camera)
{
	if (material)
		material->render(mesh, model, camera);
}

void SceneNode::renderWireframe(Camera* camera)
{
	WireframeMaterial mat = WireframeMaterial();
	mat.render(mesh, model, camera);
}

void SceneNode::renderInMenu()
{
	//Model edit
	if (ImGui::TreeNode("Model")) 
	{
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(model.m, matrixTranslation, matrixRotation, matrixScale);
		ImGui::DragFloat3("Position", matrixTranslation, 0.1f);
		ImGui::DragFloat3("Rotation", matrixRotation, 0.1f);
		ImGui::DragFloat3("Scale", matrixScale, 0.1f);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, model.m);
		
		ImGui::TreePop();
	}

	//Material
	if (material && ImGui::TreeNode("Material"))
	{
		material->renderInMenu();
		
		ImGui::TreePop();
	}

	//Geometry
	if (mesh && ImGui::TreeNode("Geometry"))
	{
		const char* meshtypes[4] = {"data/volumes/CT-Abdomen.pvm", "data/volumes/volumes/bonsai_16_16.png", "data/volumes/volumes/foot_16_16.png", "data/volumes/volumes/teapot_16_16.png" };
		const char* meshNames[4] = { "Abdomen","Bonsai","Foot","Teapot" };
		bool isChanged= ImGui::Combo("Mesh", (int*)&mesh_selected, "ABDOMEN\0BONSAI\0FOOT\0TEAPOT\0");
		if (isChanged) {
			Volume* volu = new Volume();
			if(mesh_selected==0)
			{
				volu->loadPVM(meshtypes[mesh_selected]);
				material->texture->create3DFromVolume(volu, GL_CLAMP_TO_EDGE);
				Texture* volu_text = material->texture;
				model.setScale(1.0*volu->widthSpacing, (volu_text->height) / volu_text->width*volu->heightSpacing, (volu_text->depth) / volu_text->width*volu->depthSpacing);
				material->LUT = Texture::Get("data/lut6.PNG");
			}

			else {
				volu->loadPNG(meshtypes[mesh_selected], 16.0, 16.0);
				material->texture->create3DFromVolume(volu, GL_CLAMP_TO_EDGE);
				Texture* volu_text = material->texture;
				model.setScale(1.0, volu_text->height / volu_text->width, volu_text->depth / volu_text->width);
				if (mesh_selected == 3) {
					material->LUT = Texture::Get("data/lut1.PNG");
				}
				else {
					material->LUT = Texture::Get("data/lut2.PNG");
				}
			}
		}
		ImGui::TreePop();
		ImGui::TreeNode(meshNames[mesh_selected]);
		
		if (mesh_selected == 3) {

			ImGui::Combo("Apply Clipping", (int*)&this->material->ifVC_teapot1, "Complete teapot\0Delete table\0See inside\0");
	
			if (this->material->ifVC_teapot1 == 1) {
				material->c = vec4(0.0, -0.185, 0.0, -0.1);
			}
			else if (this->material->ifVC_teapot1 == 2){
				material->c = vec4(0.0, 0.0, -0.9, 0.2);
			}
			else{
				material->c = vec4(1.0, 1.0, 1.0, -5.0);
			}

			ImGui::TreePop();
			
		}
		
		else if (mesh_selected == 2) {
			ImGui::Checkbox("Apply clipping", &this->material->ifVC);
			if (this->material->ifVC == true) {
				material->c = vec4(0.0, 0.0, -0.9, 0.2);
			}
			else {
				material->c = vec4(1.0, 1.0, 1.0, -5.0);
			}

		}
		else {
			ImGui::Checkbox("Apply clipping", &this->material->ifVC);
			if (this->material->ifVC == true) {
				material->c = vec4(0.0, 0.0, 0.7, 0.2);
			}
			else {
				material->c = vec4(1.0, 1.0, 1.0, -5.0);
			}
				
		}
		ImGui::TreePop();

	}
}
