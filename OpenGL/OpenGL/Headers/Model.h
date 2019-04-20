#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	/*  Functions   */
	Model(const char *path)
	{
		LoadModel(path);
	}
	void Draw(Shader shader);
private:
	/*  Model Data  */
	std::vector<Mesh> meshes;
	std::string directory;
	/*  Functions   */
	void LoadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
};

