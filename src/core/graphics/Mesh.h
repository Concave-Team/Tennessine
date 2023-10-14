#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include "Vertex.h"
#include <vector>

// Container for storing mesh data(vertices, indices, etc.) 
class Mesh
{
public:
	std::vector<Vertex> VertexData; // Contains Vertex Positions, Vertex Normals, and Vertex Texcoords.
	std::vector<GLuint> IndexData; // List of indices belonging to the mesh.

	GLuint VBO; // the VBO used for the mesh.
	GLuint EBO; // the EBO used for the mesh.
	GLuint VAO; // the VAO used for the mesh.

	// Generates a cube mesh without any material loaded.
	static Mesh GenMeshCube();
	// Generates a mesh based on a set of vertices and indices.
	static Mesh CreateMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
};