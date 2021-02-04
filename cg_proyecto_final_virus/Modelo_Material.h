#pragma once
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh_Material.h"

using namespace std;

class Modelo_Material
{
public:
	//Esta clase carga un modelo a dibujar con shader de iluminación
	Modelo_Material(GLchar *path)
	{
		this->loadModel(path);
	}

    // Draws the model, and thus all its meshes
    void Draw(Shader shader)
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
        {
            this->meshes[i].Draw(shader);
        }
    }
private:
	vector<Mesh_Material> meshes;

	void loadModel(string path)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
        /*
		else {
			cout << "El modelo se cargó exitosamente" << endl;
			cout << "El modelo tiene texturas?"<< scene->HasTextures() << endl;
			cout << "El modelo tiene materiales?" << scene->HasMaterials() <<endl;
            cout << "El modelo tiene animacions?" << scene->HasAnimations() << endl;
            cout << "Numero de mallas" << scene->mNumMeshes << endl;
            cout << "Numero huesos " << scene->mMeshes[0]->mNumBones << endl;

		}
        */
		// Retrieve the directory path of the filepath
		//this->directory = path.substr(0, path.find_last_of('/'));
        //Testing aianimations
        if (scene->HasAnimations()) {
            aiAnimation *animacion;
            aiMeshAnim *malla_animada;
            aiNodeAnim *nodo_animado;
            aiVector3D posicion;
            aiQuaternion rotation;
            for (GLuint i = 0; i < scene->mNumAnimations; i++)
            {
                animacion = scene->mAnimations[i];
                cout << "Nombre de la animacion " << animacion->mName.C_Str() << endl;
                for (GLuint i = 0; i < animacion->mNumMeshChannels; i++) {
                    malla_animada = animacion->mMeshChannels[i];
                    cout << "Nombre de la malla animada " << malla_animada->mName.C_Str() << endl;
                }
                printf("Numero de nodos de animacion %i", animacion->mNumChannels);
                for (GLuint i = 0; i < animacion->mNumChannels; i++) {
                    nodo_animado = animacion->mChannels[i];
                    cout << "Nombre del nodo animado " << nodo_animado->mNodeName.C_Str() << endl;
                    for (GLuint i = 0; i < nodo_animado->mNumPositionKeys; i++) {
                        posicion = nodo_animado->mPositionKeys[i].mValue;
                        printf("Pos vec3(%.2f,%.2f,%.2f)\n", posicion.x, posicion.y, posicion.z);
                    }
                    for (GLuint i = 0; i < nodo_animado->mNumRotationKeys; i++) {
                        rotation = nodo_animado->mRotationKeys[i].mValue;
                        printf("Rot vec3(%.2f,%.2f,%.2f)\n", rotation.x, rotation.y, rotation.z);
                    }
                }
            }
        }
        
        //TESTING AIANIMATIONS
		// Process ASSIMP's root node recursively
		this->processNode(scene->mRootNode, scene);
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			// The node object only contains indices to index the actual objects in the scene.
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            //aiBone hueso=mesh->mBones[1][0];
            printf("Numero de mallas en este nodo %i\n",node->mNumMeshes);
            printf("Numero de huesos en esta malla %i\n", mesh->mNumBones);
            if (mesh->mNumBones > 0) {
                for (unsigned int i = 0; i < mesh->mNumBones; i++) {
                    cout << "Nombre del hueso " << mesh->mBones[i]->mName.data<< endl;
                }
                
            }
            
			this->meshes.push_back(this->processMesh(mesh, scene));
		}

		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}
	}

	Mesh_Material processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// Data to fill
		vector<Vertex_Material> vertices;
		vector<GLuint> indices;
        Material material_cargado;
		

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex_Material vertex;
			glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.

							  // Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			vertices.push_back(vertex);
		}

		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN
            std::cout << "Si se encontró un material" << std::endl;
			material->Get(AI_MATKEY_COLOR_DIFFUSE, material_cargado.diffuse);
            material->Get(AI_MATKEY_COLOR_SPECULAR, material_cargado.specular);
            material->Get(AI_MATKEY_COLOR_AMBIENT, material_cargado.ambient);
            material->Get(AI_MATKEY_SHININESS, material_cargado.shininess);
			//// 1. Diffuse maps
			//vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			//// 2. Specular maps
			//vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh_Material(vertices, indices, material_cargado);
	}
	//void loadMaterial(aiMaterial *mat, aiTextureType type, string typeName)
	//{
	//	vector<Texture> textures;

	//	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	//	{
	//		aiString str;
	//		mat->GetTexture(type, i, &str);

	//		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
	//		GLboolean skip = false;

	//		for (GLuint j = 0; j < textures_loaded.size(); j++)
	//		{
	//			if (textures_loaded[j].path == str)
	//			{
	//				textures.push_back(textures_loaded[j]);
	//				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

	//				break;
	//			}
	//		}

	//		if (!skip)
	//		{   // If texture hasn't been loaded already, load it
	//			Texture texture;
	//			texture.id = TextureFromFile(str.C_Str(), this->directory);
	//			texture.type = typeName;
	//			texture.path = str;
	//			textures.push_back(texture);

	//			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	//		}
	//	}

	//	return textures;
	//}

};

