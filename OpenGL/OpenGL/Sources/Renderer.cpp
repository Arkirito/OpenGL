#include "Renderer.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <time.h>     
#include <random>

#include <iostream>
#include <vector>

// Custom

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "World.h"
#include "GlobalInstance.h"
#include "Model.h"
#include "Cubemap.h"
#include "PrimitiveManager.h"
#include "Settings.h"
#include "Lights/DirectionalLight.h"

Renderer::Renderer()
{
	SetupCommon3D();
	SetupCommonPostProcess();
	SetupPBR();
	SetupHDRtoCubemap();

	//3D
	SetupShadow();

	// Post process
	SetupSSAO();
	SetupBlur();
	SetupBloom();
}

Renderer::~Renderer()
{
	delete mShader_PostProcess_Native;
	delete mShader_PostProcess_SSAO;
	delete mShader_PostProcess_Blur;
	delete mShader_PostProcess_Bloom;
	delete mShader_PostProcess_DeferredShading;

	delete mShader_3D_Mesh;
	delete mShader_3D_ReflectionShader;
	delete mShader_3D_RefractionShader;
	delete mShader_3D_SkyboxShader;
	delete mShader_3D_ColoredShader;
	delete mShader_PostProcess_Negative;
	delete mShader_PostProcess_Grayscale;
	delete mShader_PostProcess_ExposureToneMapping;

	delete mShader_3D_PBR;
	delete mShader_3D_HDRtoCubemap;

	delete mHdrCubemap;
}

void Renderer::Render(float dt)
{
	Render3D();
	RenderPostProcess();
}

void Renderer::HandleInput(GLFWwindow * window)
{
}

void Renderer::Render3D()
{
	World* world = GlobalInstance::GetInstance()->GetWorld();
	Camera* camera = world->GetCurrentCamera();

	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	glBindFramebuffer(GL_FRAMEBUFFER, mMain3DFramebuffer);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, settings.ViewportWidth, settings.ViewportHeight);

	// rendering
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDepthFunc(GL_LESS);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Actual render and passing parameters to the shader

	//RenderShadowDepth();


	//TODO: Add all features. The following code is just for test.
	//PrimitiveManager::DrawCube(mShader_3D_ColoredShader, camera, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.7, 0.7, 0.7));
	//PrimitiveManager::DrawSphere(mShader_3D_ColoredShader, camera, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	RenderPBR();
	PrimitiveManager::DrawSkybox(mShader_3D_SkyboxShader, camera, mHdrCubemap);
}

void Renderer::RenderShadowDepth()
{
	World* world = GlobalInstance::GetInstance()->GetWorld();
	DirectionalLight* dirLight = world->GetDirectionalLight();

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowDepthMap_FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	world->Draw(mShader_3D_ShadowDepth);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, mMain3DFramebuffer);
}

void Renderer::RenderPBR()
{
	World* world = GlobalInstance::GetInstance()->GetWorld();
	Camera* camera = world->GetCurrentCamera();
	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	mShader_3D_PBR->Use();

	mShader_3D_PBR->SetVec3("albedo", glm::vec3(0.5, 0.1, 0.1));
	mShader_3D_PBR->SetFloat("metallic", 0.0);
	mShader_3D_PBR->SetFloat("roughness", 0.4);
	mShader_3D_PBR->SetFloat("ao", 0.1);

	for (int i = 0; i < 4; ++i)
	{
		mShader_3D_PBR->SetVec3(("lightPositions[" + std::to_string(i) + "]").c_str(), PBRLightPositions[i]);
		mShader_3D_PBR->SetVec3(("lightColors[" + std::to_string(i) + "]").c_str(), PBRLightColors[i]);
	}

	mShader_3D_PBR->SetInt("irradianceMap", 0);

	// bind pre-computed IBL data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mHdrCubemap->GetID());

	PrimitiveManager::DrawSphere(mShader_3D_PBR, camera, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
}

void Renderer::RenderHDRtoCubemap()
{
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	// convert HDR equirectangular environment map to cubemap equivalent

	mHdrTexture = Texture::LoadTexture("Content/Textures/HDR/Greenhouse3_Env.hdr");

	mShader_3D_HDRtoCubemap->Use();
	mShader_3D_HDRtoCubemap->SetInt("equirectangularMap", 0);
	mShader_3D_HDRtoCubemap->SetMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mHdrTexture->GetID());

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		mShader_3D_HDRtoCubemap->SetMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mHdrCubemap->GetID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		PrimitiveManager::JustDrawCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, mMain3DFramebuffer);
}

void Renderer::RenderPostProcess()
{
	// disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); 
							  
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	//RenderSSAO();
	//(EColorAttachment::Brightness);
	//RenderBloom();
	RenderSimplePostProcess(mTextureColorbuffers[static_cast<int>(EColorAttachment::Color)]);
}

float Renderer::Lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void Renderer::RenderSSAO()
{
	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	glBindFramebuffer(GL_FRAMEBUFFER, mSSAO_FBO);
	glClear(GL_COLOR_BUFFER_BIT);
	mShader_PostProcess_SSAO->Use();
	// Send kernel + rotation 
	for (unsigned int i = 0; i < 64; ++i)
		mShader_PostProcess_SSAO->SetVec3("samples[" + std::to_string(i) + "]", mSSAOKernel[i]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[static_cast<int>(EColorAttachment::Position)]);
	mShader_PostProcess_SSAO->SetInt("TEXTURE_POSITION", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[static_cast<int>(EColorAttachment::Normal)]);
	mShader_PostProcess_SSAO->SetInt("TEXTURE_NORMAL", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mSSAO_NoiseTexture);
	mShader_PostProcess_SSAO->SetInt("TEXTURE_NOISE", 2);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), settings.ViewportWidth / settings.ViewportHeight, 0.1f, 100.0f);
	mShader_PostProcess_SSAO->SetMat4("Matrix_Projection", projection);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderBlur(EColorAttachment attachment)
{
	bool horizontal = true, first_iteration = true;
	int amount = 10;
	mShader_PostProcess_Blur->Use();
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBlurPingpongFBO[horizontal]);
		mShader_PostProcess_Blur->SetInt("horizontal", horizontal);
		glBindTexture(
			GL_TEXTURE_2D, first_iteration ? mTextureColorbuffers[static_cast<int>(attachment)] : mBlurPingpongBuffer[!horizontal]
		);

		glBindVertexArray(mQuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	mWasLastBlurPassHorisontal = horizontal;
}

void Renderer::RenderBloom()
{
	mShader_PostProcess_Bloom->Use();
	mShader_PostProcess_Bloom->SetFloat("exposure", 0.2f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[static_cast<int>(EColorAttachment::Brightness)]);	// use the color attachment texture as the texture of the quad plane
	mShader_PostProcess_Bloom->SetInt("screenTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mBlurPingpongBuffer[!mWasLastBlurPassHorisontal]);
	mShader_PostProcess_Bloom->SetInt("brightnessTexture", 1);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderSimplePostProcess(unsigned int colorBuffer)
{
	mShader_PostProcess_Native->Use();
	mShader_PostProcess_Native->SetFloat("exposure", 0.2f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	mShader_PostProcess_Native->SetInt("screenTexture", 0);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderDeferredShading()
{
	Camera* currentCamera = GlobalInstance::GetInstance()->GetWorld()->GetCurrentCamera();

	mShader_PostProcess_DeferredShading->Use();

	mShader_PostProcess_DeferredShading->SetVec3("viewPos", currentCamera->GetPosition());
	mShader_PostProcess_DeferredShading->SetFloat("exposure", 0.1f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[static_cast<int>(EColorAttachment::Position)]);
	mShader_PostProcess_DeferredShading->SetInt("gPosition", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[static_cast<int>(EColorAttachment::Normal)]);
	mShader_PostProcess_DeferredShading->SetInt("gNormal", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[static_cast<int>(EColorAttachment::Albedo)]);
	mShader_PostProcess_DeferredShading->SetInt("gAlbedoSpec", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mSSAO_ColorBuffer);
	mShader_PostProcess_DeferredShading->SetInt("ssao", 3);

	for (int i = 0; i < 10; ++i)
	{
		mShader_PostProcess_DeferredShading->SetVec3(("lights[" + std::to_string(i) + "].Position").c_str(), deferredLightPositions[i]);
		mShader_PostProcess_DeferredShading->SetVec3(("lights[" + std::to_string(i) + "].Color").c_str(), deferredLightColors[i]);
	}

	mShader_PostProcess_DeferredShading->SetInt("useSSAO", UseSSAO);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::RenderLinearizedDepth(unsigned int colorBuffer)
{
	mShader_PostProcess_LinearizedDepth->Use();
	//debugShadowDepth.SetFloat("exposure", 0.1f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);	// use the color attachment texture as the texture of the quad plane
	mShader_PostProcess_LinearizedDepth->SetInt("screenTexture", 0);
	mShader_PostProcess_LinearizedDepth->SetFloat("near_plane", 1.f);
	mShader_PostProcess_LinearizedDepth->SetFloat("far_plane", 40.5f);

	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::SetupCommon3D()
{
	mShader_3D_Mesh = new Shader("Shaders/Mesh.vs", "Shaders/Mesh.fs");

	mShader_3D_ReflectionShader = new Shader("Shaders/reflection.vs", "Shaders/reflection.fs");
	mShader_3D_RefractionShader = new Shader("Shaders/reflection.vs", "Shaders/refraction.fs");
	mShader_3D_SkyboxShader = new Shader("Shaders/skybox.vs", "Shaders/skybox.fs");
	mShader_3D_ColoredShader = new Shader("Shaders/SingleColorMesh.vs", "Shaders/SingleColorMesh.fs");

	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	// framebuffer configuration
	// -------------------------

	glGenFramebuffers(1, &mMain3DFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mMain3DFramebuffer);
	// create a color attachment texture
	glGenTextures(5, mTextureColorbuffers);
	for (unsigned int i = 0; i < static_cast<int>(EColorAttachment::MAX); i++)
	{
		glBindTexture(GL_TEXTURE_2D, mTextureColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, settings.ViewportWidth, settings.ViewportHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mTextureColorbuffers[i], 0);
	}

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, settings.ViewportWidth, settings.ViewportHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::SetupShadow()
{
	mShader_3D_ShadowDepth = new Shader("Shaders/shadowDepth.vs", "Shaders/shadowDepth.fs");

	glGenFramebuffers(1, &mShadowDepthMap_FBO);

	glGenTextures(1, &mShadowDepthMap);
	glBindTexture(GL_TEXTURE_2D, mShadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowDepthMap_FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::SetupCommonPostProcess()
{
	mShader_PostProcess_Native = new Shader("Shaders/screenShader.vs", "Shaders/screenShader.fs");
	mShader_PostProcess_LinearizedDepth = new Shader("Shaders/screenShader.vs", "Shaders/pp_shadowDepth.fs");
	mShader_PostProcess_Negative = new Shader("Shaders/screenShader.vs", "Shaders/negative.fs");
	mShader_PostProcess_Grayscale = new Shader("Shaders/screenShader.vs", "Shaders/grayscale.fs");
	mShader_PostProcess_ExposureToneMapping = new Shader("Shaders/screenShader.vs", "Shaders/exposureToneMapping.fs");

	// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.

	float quadVertices[] = { 
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO

	glGenVertexArrays(1, &mQuadVAO);
	glGenBuffers(1, &mQuadVBO);
	glBindVertexArray(mQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Renderer::SetupSSAO()
{
	mShader_PostProcess_SSAO = new Shader("Shaders/screenShader.vs", "Shaders/shaderSSAO.fs");

	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	//SSAO Kernel

	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		float scale = (float)i / 64.0;
		scale = Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		mSSAOKernel.push_back(sample);
	}

	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	glGenTextures(1, &mSSAO_NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, mSSAO_NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &mSSAO_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mSSAO_FBO);
	
	glGenTextures(1, &mSSAO_ColorBuffer);
	glBindTexture(GL_TEXTURE_2D, mSSAO_ColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, settings.ViewportWidth, settings.ViewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mSSAO_ColorBuffer, 0);
}

void Renderer::SetupBlur()
{
	const Settings& settings = *GlobalInstance::GetInstance()->GetSettings();

	mShader_PostProcess_Blur = new Shader("Shaders/screenShader.vs", "Shaders/gaussianBlur.fs");

	glGenFramebuffers(2, mBlurPingpongFBO);
	glGenTextures(2, mBlurPingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mBlurPingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, mBlurPingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, settings.ViewportWidth, settings.ViewportHeight, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurPingpongBuffer[i], 0
		);
	}
}

void Renderer::SetupBloom()
{
	mShader_PostProcess_Bloom = new Shader("Shaders/screenShader.vs", "Shaders/gaussianBlur.fs");
}

void Renderer::SetupDeferredShading()
{
	mShader_PostProcess_DeferredShading = new Shader("Shaders/screenShader.vs", "Shaders/defferedShading.fs");

	srand(time(NULL));

	for (int i = 0; i < 10; ++i)
	{
		deferredLightPositions.push_back(glm::vec3(rand() % 20 - 10, rand() % 2, rand() % 20 - 10));
		deferredLightColors.push_back(glm::vec3(rand() % 10, rand() % 10, rand() % 10));
	}

}

void Renderer::SetupPBR()
{
	mShader_3D_PBR = new Shader("Shaders/PBR.vs", "Shaders/PBR.fs");

	for (int i = 0; i < 4; ++i)
	{
		PBRLightPositions.push_back(glm::vec3(rand() % 20 - 10, rand() % 2, rand() % 20 - 10));
		PBRLightColors.push_back(glm::vec3(rand() % 10, rand() % 10, rand() % 10));
	}
}

void Renderer::SetupHDRtoCubemap()
{
	mShader_3D_HDRtoCubemap = new Shader("Shaders/HDR_To_Cubemap.vs", "Shaders/HDR_To_Cubemap.fs");

	glGenFramebuffers(1, &mCaptureFBO);
	glGenRenderbuffers(1, &mCaptureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, mCaptureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, mCaptureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mCaptureRBO);

	mHdrCubemap = new Cubemap();

	RenderHDRtoCubemap();
}
