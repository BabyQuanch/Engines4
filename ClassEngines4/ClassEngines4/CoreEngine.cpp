#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine() : window(nullptr), isRunning(false), fps(60), gameInterface(nullptr), currentSceneNum(0)
{
}

CoreEngine::~CoreEngine()
{
}

CoreEngine* CoreEngine::GetInstance()
{
	if (engineInstance.get() == nullptr) {
		engineInstance.reset(new CoreEngine);
	}
	return engineInstance.get();
}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_)
{
	Debug::OnCreate();
	window = new Window();
	if (!window->OnCreate(name_ , width_ ,height_ )) {
		Debug::FatalError("Failed to initialize window", "CoreEngine.cpp", __LINE__);
		OnDestroy();
		return isRunning = false;
	}
	Debug::Info("everything worked", "CoreEngine.cpp", __LINE__);
	if (gameInterface) {
		Debug::Info("everything worked", "CoreEngine.cpp", __LINE__);
		if (!gameInterface->OnCreate()) {
			Debug::FatalError("Failed to initialize Game Interface", "CoreEngine.cpp", __LINE__);
			OnDestroy();
			return isRunning = false;
		}
	}

	
	timer.Start();
	return isRunning = true;
}

void CoreEngine::run()
{
	while (isRunning) {
		timer.UpdateFrameTicks();
		Update(timer.GetDeltaTime());
		Render();
		SDL_Delay(timer.GetSleepTime(fps));
	}
	if (!isRunning) {
		OnDestroy();
	}
}

void CoreEngine::Exit()
{
	isRunning = false;
}

bool CoreEngine::GetIsRunning() const
{
	return isRunning;
}

int CoreEngine::GetCurrentScene() const
{
	return currentSceneNum;
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_)
{
	gameInterface = gameInterface_;
}

void CoreEngine::SetCurrentScene(int sceneNum_)
{
	currentSceneNum = sceneNum_;
}

void CoreEngine::Update(const float deltaTime_)
{
	if (gameInterface) {
		gameInterface->Update(deltaTime_);
		std::cout << deltaTime_ << std::endl;
	}
	
}

void CoreEngine::Render()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gameInterface) {
		gameInterface->Render();
	}
	SDL_GL_SwapWindow(window->GetWindow());
}

void CoreEngine::OnDestroy()
{
	delete gameInterface;
	gameInterface = nullptr;
	delete window;
	window = nullptr;
	SDL_Quit();
	exit(0);
}
