#include "Application.h"

void Application::DoGLFWLoop()
{
	if (glfwWindowShouldClose(window.GetWindowHandle()))
		this->Halt(); // exit with code 0 (success).
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cout << "OpenGL Debug: SRC: " << source << " TYPE: " << type << " ID: " << id << " SEVERITY: " << severity << " MESSAGE: " << message << std::endl;
}

glm::mat4 CalculateTransformMatrix(std::shared_ptr<TransformComponent> com)
{
	glm::mat4 RotationMatrix = glm::mat4(1.0f);
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(com->Rotation.x), { 1,0,0 });
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(com->Rotation.y), { 0,1,0 });
	RotationMatrix = glm::rotate(RotationMatrix, glm::radians(com->Rotation.z), { 0,0,1 });

	return glm::translate(glm::mat4(1), com->Position.ToGLMVec3()) * RotationMatrix * glm::scale(glm::mat4(1.0f), com->Scale.ToGLMVec3());
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Application::Run()
{
	std::shared_ptr<AppStateEventArgs> args = std::make_shared<AppStateEventArgs>();
	args->app = this;
	EventHandler::FireEvent(EventType::APP_PROCESS_START, args);

	SetStyle();

	Input::GetInstance().RegisterInput();

	Input& in = Input::GetInstance();
	Renderer& renderer = Renderer::GetInstance();

	GLuint ViewVAO = renderer.CreateVAO();
	renderer.BindVAO(ViewVAO);
	GLuint ViewVBO = renderer.CreateVBO();
	renderer.BindVBO(ViewVBO);

	ShaderProgram prog = renderer.CreateProgram(renderer.CreateShader(AssetManager::LoadAsset("shaders/main/editor/view.frag").data, ShaderType::FRAGMENT), renderer.CreateShader(AssetManager::LoadAsset("shaders/main/editor/view.vert").data, ShaderType::VERTEX));
	ShaderProgram df = renderer.CreateProgram(renderer.CreateShader(AssetManager::LoadAsset("shaders/deferred/dflighting.frag").data, ShaderType::FRAGMENT), renderer.CreateShader(AssetManager::LoadAsset("shaders/deferred/dflighting.vert").data, ShaderType::VERTEX));

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	renderer.BindVAO(ViewVAO);
	renderer.BindVBO(ViewVBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(quadVertices) * sizeof(float), quadVertices, GL_STATIC_DRAW);

	const unsigned int NR_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
		float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

	// main loop

	int frameCount = 0, previousTime = 0, ElapsedTicks = 0;
	while (!this->shouldHalt) {
		World& _world = World::GetInstance();
		double currentTime = glfwGetTime();
		frameCount++;

		_world.UpdateScene(currentTime-previousTime);

		if (currentTime - previousTime >= 1.0)
		{
			// Display the frame count here any way you want.
			std::cout << "FPS: " << frameCount << std::endl;

			frameCount = 0;
			
			previousTime = currentTime;
		}
		ElapsedTicks += 1;

		this->DoGLFWLoop();
		printf("\n\n\n\nX: %d; Y: %d\n\n\n\n", in.MousePosition.x, in.MousePosition.y);
		glfwPollEvents();

		renderer.Clear();

		// Get all entities that have a mesh renderer component attached to them. (building the render queue)
		
		std::vector<std::shared_ptr<Entity>> meshToBeRendered;

		for (std::shared_ptr<Entity>& _entity : _world.WorldEntities)
		{
			if (_entity->HasComponentOfType<MeshRendererComponent>())
			{
				meshToBeRendered.emplace_back(_entity);
			}
		}

		printf("SIZE: %d", meshToBeRendered.size());

		// Now, draw all of the entities.

		for (std::shared_ptr<Entity>& _entity : meshToBeRendered)
		{
			std::shared_ptr<EComponent> com = _entity->GetComponentOfType<MeshRendererComponent>();
			std::shared_ptr<EComponent> com2 = _entity->GetComponentOfType<TransformComponent>();
			std::shared_ptr<MeshRendererComponent> mshr = std::dynamic_pointer_cast<MeshRendererComponent>(com);
			std::shared_ptr<TransformComponent> trnf = std::dynamic_pointer_cast<TransformComponent>(com2);
			if (mshr)
			{
				if (mshr->Meshes.size() != mshr->Materials.size())
				{
					printf("[!]: Mesh and material count mismatch!");
				}
				else
				{
					glBindFramebuffer(GL_FRAMEBUFFER, renderer.gBuffer);
					renderer.Clear();
					for (int i = 0; i < mshr->Meshes.size(); i++)
					{
						Mesh m = mshr->Meshes[i];
						Material mtl = mshr->Materials[i];

						renderer.CreateProjection();
						renderer.Position = { 0,0,0 };
						renderer.Target = { 0,0,0 };
						renderer.Up = { 0,1,0 };
						renderer.CreateView(renderer.Position, renderer.Target, renderer.Up);

						mtl.ViewPosition = renderer.Position;
						
						renderer.CurrentShader = mtl.MaterialShader;

						mtl.MaterialShader.Use();

						renderer.RenderMesh(m, CalculateTransformMatrix(trnf));
					}

					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					renderer.Clear();
					df.Use();


					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, renderer.gPos);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, renderer.gNorm);
					glActiveTexture(GL_TEXTURE2);
					glBindTexture(GL_TEXTURE_2D, renderer.gColorSpec);

					

					for (unsigned int i = 0; i < lightPositions.size(); i++)
					{
						df.SetVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
						df.SetVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
					}

					df.SetVec3("viewPos", renderer.Position);

					prog.Use();
					renderQuad();

					glBindFramebuffer(GL_READ_FRAMEBUFFER, renderer.gBuffer);
					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
					glBlitFramebuffer(0, 0, this->window.Size.x, this->window.Size.y, 0, 0, this->window.Size.x, this->window.Size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);

				}
			}
			else
			{
				printf("[!]: Could not get shared_ptr of MeshRendererComponent.");
			}
		}
		
		// Draw the skybox.
		//renderer.RenderSkyboxCubemap(renderer.SkyboxCubemap);

		glfwSwapInterval(1);
		glfwSwapBuffers(window.GetWindowHandle());
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	window.~Window();
	glfwTerminate();

	AssetManager::Destruct();
	EventHandler::Destruct();
}

int Application::Init(bool initGL)
{
	window.Init("Tennessine Application", { 800,600 });

	EventHandler::CreateThread(); // init the eventhandler
	
	if(initGL)
	{
		window.MakeContextCurrent();
		return this->InitGL();
	}

	return 0;
}

int Application::InitGL()
{
	if (!glInitialized)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			printf("[-]: Couldn't initialize glad.");
			return -1;
		}

		window.MakeContextCurrent();
		Renderer& renderer = Renderer::GetInstance();

		renderer.Init(this->window.Size.x, this->window.Size.y);
		renderer.SetViewport(0, 0, 800, 600);
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(DebugCallback, nullptr);

		// beautiful Dear, Imgui initialization(copied by yours truly!)
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window.GetWindowHandle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
		ImGui_ImplOpenGL3_Init();
	}
	else
	{
		printf("[*]: Cannot initalize glad twice.");
		return 1;
	}

	this->glInitialized = true;
	return 0;
}

void Application::Halt(int nExitCode)
{
	this->shouldHalt = true;

	std::shared_ptr<AppStateEventArgs> args = std::make_shared<AppStateEventArgs>();
	args->app = this;
	EventHandler::FireEvent(EventType::APP_PROCESS_HALT, args);

	printf("[*]: Application halted with exit code: %d.", nExitCode);
}