#include "Mesh.h"

Mesh Mesh::GenMeshCube()
{
	Mesh msh;

	std::vector<Vertex> cubeVertices =
	{
		{{ -0.5, 0.5, -0.5 },{ 0, 1, 0 },{ 0, 0 }},
		{{ 0.5, 0.5, 0.5 },{ 0, 1, 0 },{ 1, 1 }},
		{{ 0.5, 0.5, -0.5 },{ 0, 1, 0 },{ 1, 0 }},
		{{ 0.5, 0.5, 0.5 },{ 0, 0, 1 },{ 1, 0 }},
		{{ -0.5, -0.5, 0.5 },{ 0, 0, 1 },{ 0, 1 }},
		{{ 0.5, -0.5, 0.5 },{ 0, 0, 1 },{ 1, 1 }},
		{{ -0.5, 0.5, 0.5 },{ -1, 0, 0 },{ 0, 0 }},
		{{ -0.5, -0.5, -0.5 },{ -1, 0, 0 },{ 1, 1 }},
		{{ -0.5, -0.5, 0.5 },{ -1, 0, 0 },{ 0, 1 }},
		{{ 0.5, -0.5, -0.5 },{ 0, -1, 0 },{ 1, 0 }},
		{{ -0.5, -0.5, 0.5 },{ 0, -1, 0 },{ 0, 1 }},
		{{ -0.5, -0.5, -0.5 },{ 0, -1, 0 },{ 0, 0 }},
		{{ 0.5, 0.5, -0.5 },{ 1, 0, 0 },{ 1, 0 }},
		{{ 0.5, -0.5, 0.5 },{ 1, 0, 0 },{ 0, 1 }},
		{{ 0.5, -0.5, -0.5 },{ 1, 0, 0 },{ 1, 1 }},
		{{ -0.5, 0.5, -0.5 },{ 0, 0, -1 },{ 0, 0 }},
		{{ 0.5, -0.5, -0.5 },{ 0, 0, -1 },{ 1, 1 }},
		{{ -0.5, -0.5, -0.5 },{ 0, 0, -1 },{ 0, 1 }},
		{{ -0.5, 0.5, -0.5 },{ 0, 1, 0 },{ 0, 0 }},
		{{ -0.5, 0.5, 0.5 },{ 0, 1, 0 },{ 0, 1 }},
		{{ 0.5, 0.5, 0.5 },{ 0, 1, 0 },{ 1, 1 }},
		{{ 0.5, 0.5, 0.5 },{ 0, 0, 1 },{ 1, 0 }},
		{{ -0.5, 0.5, 0.5 },{ 0, 0, 1 },{ 0, 0 }},
		{{ -0.5, -0.5, 0.5 },{ 0, 0, 1 },{ 0, 1 }},
		{{ -0.5, 0.5, 0.5 },{ -1, 0, 0 },{ 0, 0 }},
		{{ -0.5, 0.5, -0.5 },{ -1, 0, 0 },{ 1, 0 }},
		{{ -0.5, -0.5, -0.5 },{ -1, 0, 0 },{ 1, 1 }},
		{{ 0.5, -0.5, -0.5 },{ 0, -1, 0 },{ 1, 0 }},
		{{ 0.5, -0.5, 0.5 },{ 0, -1, 0 },{ 1, 1 }},
		{{ -0.5, -0.5, 0.5 },{ 0, -1, 0 },{ 0, 1 }},
		{{ 0.5, 0.5, -0.5 },{ 1, 0, 0 },{ 1, 0 }},
		{{ 0.5, 0.5, 0.5 },{ 1, 0, 0 },{ 0, 0 }},
		{{ 0.5, -0.5, 0.5 },{ 1, 0, 0 },{ 0, 1 }},
		{{ -0.5, 0.5, -0.5 },{ 0, 0, -1 },{ 0, 0 }},
		{{ 0.5, 0.5, -0.5 },{ 0, 0, -1 },{ 1, 0 }},
		{{ 0.5, -0.5, -0.5 },{ 0, 0, -1 },{ 1, 1 }},
	};

	// Generated Indices:
	std::vector<GLuint> cubeIndices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, };

	msh.VertexData = cubeVertices;
	msh.IndexData = cubeIndices;

	glCreateVertexArrays(1, &msh.VAO);
	glCreateBuffers(1, &msh.VBO);
	std::cout << "VDS: " << msh.VertexData.size() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, msh.VBO);
	glBufferData(GL_ARRAY_BUFFER, msh.VertexData.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glCreateBuffers(1, &msh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, msh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, msh.IndexData.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

	return msh;
}

Mesh Mesh::CreateMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	Mesh mesh;

	mesh.VertexData = vertices;
	mesh.IndexData = indices;

	glCreateVertexArrays(1, &mesh.VAO);
	glCreateBuffers(1, &mesh.VBO);
	std::cout << "VDS: " << mesh.VertexData.size() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.VertexData.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glCreateBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.IndexData.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

	return mesh;
}