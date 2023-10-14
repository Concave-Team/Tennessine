#include <iostream>
#include "app/EventHandler.h"
#include "app/Application.h"

int main(void)
{
	EventHandler::SetEventCallback(EventType::APP_PROCESS_START, [](EventArgs* args) -> void {
		printf("[*]: Application has started!");
		AppStateEventArgs* appStateArgs = dynamic_cast<AppStateEventArgs*>(args);
		if (appStateArgs) {
			//appStateArgs->app->InitGL();
		}
		else
		{
			printf("E013: Cannot cast EventArgs to AppStateEventArgs.");
		}
	});

	EventHandler::SetEventCallback(EventType::ERROR_GOT, [](EventArgs* args) -> void {
		printf("[!]: Error Caught!");
		ErrorEventArgs* eStateArgs = dynamic_cast<ErrorEventArgs*>(args);
		if (eStateArgs) {
			printf("ERROR: %s", eStateArgs->message.c_str());
		}
		else
		{
			printf("E013: Cannot cast EventArgs to ErrorEventArgs. (ironic how an error errored.)");
		}
	});

	EventHandler::SetEventCallback(EventType::ASSET_LOADED, [](EventArgs* args) -> void {
	AssetEventArgs* eStateArgs = dynamic_cast<AssetEventArgs*>(args);
		if (eStateArgs) {
			std::cout << "ASSET LOADED: " << eStateArgs->asset.name << " from path " << eStateArgs->asset.path << " of type " << (int)eStateArgs->asset.type << " with data " << eStateArgs->asset.data << std::endl;
		}
		else
		{
			printf("E013: Cannot cast EventArgs to AssetEventArgs.");
		}	
	});

	Application app;

	int err = app.Init();

	if (err < 0) {
		printf("Error experienced while loading application.");
		return -1;
	}

	Renderer& renderer = Renderer::GetInstance();

	GLuint texId = renderer.CreateTexture("laminate_floor_02_1k/textures/laminate_floor_02_diff_1k.jpg");

	std::shared_ptr<Entity> cube = std::make_shared<Entity>();
	cube->Name = "EPIC CUBE";
	cube->Tag = "Renderable Coob";
	World::GetInstance().AddEntity(cube);
	std::shared_ptr<MeshRendererComponent> MeshCubeRND = std::make_shared<MeshRendererComponent>();
	Mesh cubemesh = Mesh::GenMeshCube();
	Material cubemat = Material::LoadDefaultMaterial();
	cubemat.AlbedoTexture = texId;
	MeshCubeRND->AddMeshAndMaterial(cubemesh, cubemat);
	cube->RegisterComponent(MeshCubeRND);
	std::shared_ptr<TransformComponent> MeshCubeTf = std::make_shared<TransformComponent>();

	MeshCubeTf->Position = { -1,0,-2 };
	MeshCubeTf->Rotation = { 15,15,15 };
	MeshCubeTf->Scale = { 0.5f,0.5f,0.5f };

	cube->RegisterComponent(MeshCubeTf);

	std::shared_ptr<Entity> cube2 = std::make_shared<Entity>();
	cube2->Name = "EPIC CUBE #2";
	cube2->Tag = "Renderable Coob";
	World::GetInstance().AddEntity(cube2);
	std::shared_ptr<MeshRendererComponent> MeshCubeRND2 = std::make_shared<MeshRendererComponent>();
	MeshCubeRND2->AddMeshAndMaterial(cubemesh, cubemat);
	cube2->RegisterComponent(MeshCubeRND2);
	std::shared_ptr<TransformComponent> MeshCubeTf2 = std::make_shared<TransformComponent>();

	MeshCubeTf2->Position = { 1,0,-2 };
	MeshCubeTf2->Rotation = { 15,50,15 };
	MeshCubeTf2->Scale = { 0.5f,0.5f,0.5f };

	cube2->RegisterComponent(MeshCubeTf2);

	std::vector<std::string> faces
	{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};

	GLuint skyboxTex = renderer.CreateCubemap(faces);
	renderer.SkyboxCubemap = skyboxTex;

	app.Run();

	return 0;
}