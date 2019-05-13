#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum class EColorAttachment
{
	Color = 0,
	Brightness = 1,
	Position = 2,
	Normal = 3,
	Albedo = 4,
	MAX
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render(float dt);

	// SETUP

	bool UseSSAO = false;

	void HandleInput(struct GLFWwindow *window);

protected:

	void Render3D();
	void RenderShadowDepth();
	void RenderPBR();
	void RenderHDRtoCubemap();

	void RenderPostProcess();
	void RenderSSAO();
	void RenderBlur(EColorAttachment attachment);
	void RenderBloom();
	void RenderSimplePostProcess(unsigned int colorBuffer);
	void RenderDeferredShading();
	void RenderLinearizedDepth(unsigned int colorBuffer);

	//Shaders
	//3D
	class Shader* mShader_3D_Mesh;
	class Shader* mShader_3D_ShadowDepth;
	class Shader* mShader_3D_HDRtoCubemap;

	class Shader* mShader_3D_ReflectionShader;
	class Shader* mShader_3D_RefractionShader;
	class Shader* mShader_3D_SkyboxShader;
	class Shader* mShader_3D_ColoredShader;

	class Shader* mShader_3D_PBR;

	// PostProcess
	class Shader* mShader_PostProcess_Native;
	class Shader* mShader_PostProcess_SSAO;
	class Shader* mShader_PostProcess_Blur;
	class Shader* mShader_PostProcess_Bloom;
	class Shader* mShader_PostProcess_DeferredShading;
	class Shader* mShader_PostProcess_LinearizedDepth;
	class Shader* mShader_PostProcess_Negative;
	class Shader* mShader_PostProcess_Grayscale;
	class Shader* mShader_PostProcess_ExposureToneMapping;

	//
	float Lerp(float a, float b, float f);

	// This is a common OpenGL data, used for all possible render techniques
	unsigned int mMain3DFramebuffer;
	unsigned int mTextureColorbuffers[static_cast<int>(EColorAttachment::MAX)];

	//Post process
	bool mWasLastBlurPassHorisontal = true;

	std::vector<glm::vec3> mSSAOKernel;

	unsigned int mQuadVAO, mQuadVBO;
	unsigned int mSSAO_FBO;
	unsigned int mSSAO_ColorBuffer;
	unsigned int mSSAO_NoiseTexture;

	unsigned int mDeferredShading_FBO;
	
	//Buffers
	unsigned int mBlurPingpongFBO[2];
	unsigned int mBlurPingpongBuffer[2];

	unsigned int mShadowDepthMap_FBO;
	unsigned int mShadowDepthMap;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int mCaptureFBO, mCaptureRBO;

	// Setups
	void SetupCommon3D();
	void SetupShadow();

	void SetupCommonPostProcess();
	void SetupSSAO();
	void SetupBlur();
	void SetupBloom();
	void SetupDeferredShading();

	void SetupPBR();

	void SetupHDRtoCubemap();

	//DATA

    class Texture* mHdrTexture;
	class Cubemap* mHdrCubemap;

private:

	// This positions and colors should be moved to a specific container of Lights

	std::vector<glm::vec3> deferredLightPositions;
	std::vector<glm::vec3> deferredLightColors;

	// This positions and colors should be moved to a specific container of PBR Lights

	std::vector<glm::vec3> PBRLightPositions;
	std::vector<glm::vec3> PBRLightColors;
};

