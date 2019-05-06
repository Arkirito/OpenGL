#include "World.h"
#include "Camera.h"
#include "Lights/DirectionalLight.h"
#include "Shader.h"


World::World()
{
}


World::~World()
{
}

void World::RegisterObject(WorldObject * obj)
{
	mObjects.push_back(obj);

	// Maybe not the best way to handle this, but so easy to use)

	if (!mCurrentCamera)
	{
		Camera* camera = dynamic_cast<Camera*>(obj); //TODO: rtti is heavy, better implement custom cast for used classes

		if (camera != nullptr)
		{
			mCurrentCamera = camera;
		}
	}

	if (!mDirectionalLight)
	{
		DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(obj);

		if (dirLight != nullptr)
		{
			mDirectionalLight = dirLight;
		}
	}
}

void World::UnregisterObject(WorldObject * obj)
{
	std::vector<WorldObject*>::iterator iter;

	for (iter = mObjects.begin(); iter < mObjects.end(); ++iter)
	{
		if (*iter == obj)
		{
			mObjects.erase(iter);
			return;
		}
	}
}

void World::HandleInput(GLFWwindow * window)
{
	for (WorldObject* obj : mObjects)
	{
		obj->ProcessInput(window);
	}
}

Camera * World::GetCurrentCamera()
{
	return mCurrentCamera;
}

void World::Draw(Shader* shaderToDraw)
{
	shaderToDraw->Use();

	shaderToDraw->SetVec3("dirLight.ambient", mDirectionalLight->mMaterial.mAmbient);
	shaderToDraw->SetVec3("dirLight.diffuse", mDirectionalLight->mMaterial.mDiffuse); // darken the light a bit to fit the scene
	shaderToDraw->SetVec3("dirLight.specular", mDirectionalLight->mMaterial.mSpecular);
	shaderToDraw->SetVec3("dirLight.direction", mDirectionalLight->GetDirection());

	shaderToDraw->SetMat4("lightSpaceMatrix", mDirectionalLight->GetLightSpaceMatrix());

	//TODO
	/*shaderToDraw->SetVec3("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shaderToDraw->SetVec3("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken the light a bit to fit the scene
	shaderToDraw->SetVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	shaderToDraw->SetVec3("spotLight.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
	shaderToDraw->SetVec3("spotLight.position", mCurrentCamera->GetPosition());
	shaderToDraw->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shaderToDraw->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(22.5f)));

	for (GLuint i = 0; i < 4; i++)
	{
		std::string number = std::to_string(i);

		shaderToDraw->SetVec3(("pointLights[" + number + "].position").c_str(), glm::vec3(pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z));
		shaderToDraw->SetVec3(("pointLights[" + number + "].ambient").c_str(), glm::vec3(pointLightColors[i].r * 0.1f, pointLightColors[i].g * 0.1f, pointLightColors[i].b * 0.1f));
		shaderToDraw->SetVec3(("pointLights[" + number + "].diffuse").c_str(), glm::vec3(pointLightColors[i].r, pointLightColors[i].g, pointLightColors[i].b));
		shaderToDraw->SetVec3(("pointLights[" + number + "].specular").c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
		shaderToDraw->SetFloat(("pointLights[" + number + "].constant").c_str(), 1.0f);
		shaderToDraw->SetFloat(("pointLights[" + number + "].linear").c_str(), 0.09f);
		shaderToDraw->SetFloat(("pointLights[" + number + "].quadratic").c_str(), 0.032f);
	}*/
}

DirectionalLight * World::GetDirectionalLight()
{
	return mDirectionalLight;
}
