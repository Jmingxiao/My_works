#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include"Mesh.h"
#include<vector>
#include<iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

class Model
{
public:
    // model data 
    vector<TextureM> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    Model(string const& path, bool gamma = false);
    void Draw(Shader& shader);


private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<TextureM> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};