#include "Renderer.h"

int Shader::CompileShader(Shader shd)
{
	glCompileShader(shd.ShaderId);

	int success;

	glGetShaderiv(shd.ShaderId, GL_COMPILE_STATUS, &success);

	return success;
}

void Shader::DestroyShader(Shader shd)
{
	glDeleteShader(shd.ShaderId);
}

void ShaderProgram::Use()
{
	glUseProgram(this->ProgramId);
}

void ShaderProgram::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->ProgramId, name.c_str()), value);
}

void ShaderProgram::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->ProgramId, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->ProgramId, name.c_str()), value);
}

void ShaderProgram::SetMat4(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(this->ProgramId, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::DestroyProgram()
{
	glDeleteProgram(this->ProgramId);
}

int ShaderProgram::Link()
{
	glLinkProgram(this->ProgramId);

	int success;
	glGetProgramiv(this->ProgramId, GL_LINK_STATUS, &success);

	return success;
}

Shader Renderer::CreateShader(std::string code, ShaderType ty)
{
	Shader shader{ 0 };
	if (ty == ShaderType::VERTEX)
	{
		shader.ShaderId = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (ty == ShaderType::FRAGMENT)
	{
		shader.ShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
	{
		SendErrorEvent("Invalid ShaderType given.");
		return shader;
	}

	const char* code_cc = code.c_str();

	glShaderSource(shader.ShaderId, 1, &code_cc, nullptr);

	if (!shader.CompileShader(shader))
	{
		CheckGLErrors("SHADER COMPILATION");
		
		char infoLog[512];

		glGetShaderInfoLog(shader.ShaderId, 512, NULL, infoLog);
		std::string errorMessage = "Shader compilation failed: " + std::string(infoLog);

		glDeleteShader(shader.ShaderId); // delete the shader, since we're not gonna be using it anymore.

		SendErrorEvent(errorMessage);
	}
	shader.Type = ty;

	return shader;
}

ShaderProgram Renderer::CreateProgram(Shader frag, Shader vert)
{
	ShaderProgram prog; // pog ikr

	prog.ProgramId = glCreateProgram();

	glAttachShader(prog.ProgramId, frag.ShaderId);
	glAttachShader(prog.ProgramId, vert.ShaderId);

	glLinkProgram(prog.ProgramId);

	char infoLog[512];
	int success;

	glGetProgramiv(prog.ProgramId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(prog.ProgramId, 512, NULL, infoLog);
		SendErrorEvent("Shader Program Linking Failed: " + std::string(infoLog));
	}

	frag.DestroyShader(frag);
	vert.DestroyShader(vert);

	return prog;
}

GLuint Renderer::CreateVAO()
{
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	return VAO;
}

GLuint Renderer::CreateVBO()
{
	GLuint VBO;
	glGenBuffers(1, &VBO);

	return VBO;
}

GLuint Renderer::CreateEBO()
{
	GLuint EBO;
	glGenBuffers(1, &EBO);

	return EBO;
}

void Renderer::BindVAO(GLuint id)
{
	this->BoundVAO = id;
	glBindVertexArray(id);
}

void Renderer::BindVBO(GLuint id)
{
	this->BoundVBO = id;
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void Renderer::BindEBO(GLuint id)
{
	this->BoundEBO = id;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void Renderer::UnbindVAO()
{
	this->BoundVAO = 0;
	glBindVertexArray(0);
}

void Renderer::UnbindVBO()
{
	this->BoundVBO = 0;
	glBindBuffer(0, GL_ARRAY_BUFFER);
}

void Renderer::UnbindEBO()
{
	this->BoundEBO = 0;
	glBindBuffer(0, GL_ELEMENT_ARRAY_BUFFER);
}

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	
}

void Renderer::Init(int width, int height)
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	this->SetDepthTesting(true);

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// POSITION CB:
	glGenTextures(1, &gPos);
	glBindTexture(GL_TEXTURE_2D, gPos);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPos, 0);

	// NORMAL CB:
	glGenTextures(1, &gNorm);
	glBindTexture(GL_TEXTURE_2D, gNorm);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNorm, 0);

	// COLOR-SPECULAR CB:
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	glDrawBuffers(3, Attachments);

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	SkyboxVAO = this->CreateVAO();
	SkyboxVBO = this->CreateVBO();
	this->BindVAO(SkyboxVAO);
	this->BindVBO(SkyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(skyboxVertices) * sizeof(float), skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	this->BindVAO(this->CreateVAO());

	this->BindVBO(this->CreateVBO());

	this->BindEBO(this->CreateEBO());

	Asset FragmentShader = AssetManager::LoadAsset("shaders/skybox.frag");
	Asset VertexShader = AssetManager::LoadAsset("shaders/skybox.vert");

	if (FragmentShader.data == "error")
	{
		SendErrorEvent("Could not load skybox fragment shader.");
		return;
	}
	if(VertexShader.data == "error")
	{
		SendErrorEvent("Could not load skybox vertex shader.");
		return;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_MULTISAMPLE);

	this->SkyboxShader = this->CreateProgram(this->CreateShader(FragmentShader.data, ShaderType::FRAGMENT), this->CreateShader(VertexShader.data, ShaderType::VERTEX));
}

void Renderer::RenderObject(ShaderProgram program, const GLuint& vao, GLuint numIndices, GLuint indices[], const GLuint& texture, const glm::mat4& modelMatrix)
{
	this->CurrentShader = program;
	

	program.Use();
	program.SetMat4("Projection", this->ProjectionMat);
	program.SetMat4("View", this->ViewMat);
	program.SetMat4("Model", modelMatrix);

	

	this->BindVAO(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Renderer::RenderMesh(Mesh mesh, const glm::mat4& modelMatrix)
{
	this->BindVAO(mesh.VAO);
	this->BindVBO(mesh.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.VertexData.size() * sizeof(Vertex), mesh.VertexData.data());
	// Specify the vertex attribute pointer AFTER binding the VAO
// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	// TexCoord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	std::cout << "UVDS: " << mesh.VertexData.size()*sizeof(Vertex) << std::endl;

	this->BindEBO(mesh.EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mesh.IndexData.size() * sizeof(GLuint), mesh.IndexData.data());
	

	CurrentShader.Use();
	CurrentShader.SetMat4("Projection", this->ProjectionMat);
	CurrentShader.SetMat4("View", this->ViewMat);
	CurrentShader.SetMat4("Model", modelMatrix);

	glDrawElements(GL_TRIANGLES, mesh.IndexData.size(), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}

void Renderer::RenderSkyboxCubemap(GLuint cubemap)
{
	glDepthFunc(GL_LEQUAL);
	this->SkyboxShader.Use();
	this->SkyboxShader.SetMat4("projection", this->ProjectionMat);
	this->SkyboxShader.SetMat4("view", glm::mat4(glm::mat3(this->ViewMat)));

	glBindVertexArray(this->SkyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Renderer::CreateProjection()
{
	this->ProjectionMat = glm::perspective((float)glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Renderer::CreateView(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	this->ViewMat = glm::lookAt(pos, pos + cameraFront, up);
}

void Renderer::CheckGLErrors(std::string ctx)
{
	GLenum error = glGetError();
	if (error != 0)
	{
		SendErrorEvent("OpenGL Error:" + ctx + " Error Code: " + std::to_string((int)error));
	}
}

void Renderer::SetDepthTesting(bool enable)
{
	if (enable)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
		glDisable(GL_DEPTH_TEST);
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Renderer::Clear()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

GLuint Renderer::CreateTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	GLuint texture;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		SendErrorEvent("Could not properly load texture from path " + path);
		return -1; // failed.
	}

	stbi_image_free(data);
	return texture;
}

GLuint Renderer::CreateCubemap(std::vector<std::string> paths)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

GLuint Renderer::CreateFBO()
{
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);

	return fbo;
}

void Renderer::BindFBO(GLuint id)
{
	this->BoundFBO = id;
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}


FBOTexture Renderer::CreateFBOTexture(GLuint id, int sizeX, int sizeY)
{
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);


	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sizeX, sizeY);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer is not yet finished!" << " " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FBOTexture rtv;
	rtv.FBOid = id;
	rtv.RBOid = rbo;
	rtv.TEXid = texture;

	return rtv;
}