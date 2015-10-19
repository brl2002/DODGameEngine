//#include "Engine.h"
//
//#include <Windows.h>

#include "ResourceManager.h"
#include "Render.h"
#include "Physics.h"
#include "AIManager.h"
#include "Entity.h"
#include <iostream>

void main()
{
	//HWND hwnd = GetConsoleWindow();
	//HMENU hmenu = GetSystemMenu(hwnd, FALSE);
	//EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

	//Engine engine = Engine();
	//engine.Run();

	Graph graph = Graph();
	

	ResourceManager::getInstance().ReadMapFile("map.txt");
	bool* navBuffer = ResourceManager::getInstance().AllocateNavBuffer("x", 3, 3);

	RenderComponent* renderComponent = new RenderComponent(
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().AllocateMapBuffer(),
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight());

	PhysicsComponent* physicsComponent = new PhysicsComponent(
		navBuffer,
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight(),
		3, 3);

	AIManager* aiManager = new AIManager(
		navBuffer,
		ResourceManager::getInstance().GetMapBufferWidth(),
		ResourceManager::getInstance().GetMapBufferHeight());

	Entity* m_Entities = new Entity[4] { 'P', 'X', 'X', 'X' };
	m_Entities[0].position.x = 20;
	m_Entities[0].position.y = 10;
	m_Entities[1].position.x = 3;
	m_Entities[1].position.y = 2;
	m_Entities[2].position.x = 8;
	m_Entities[2].position.y = 2;
	m_Entities[3].position.x = 25;
	m_Entities[3].position.y = 2;

	AIManager::Update(aiManager, m_Entities, 0, 4);

	renderComponent->Clear();

	RenderComponent::Update(renderComponent, m_Entities, 0, 4);

	renderComponent->Render();

	delete[] m_Entities;
	delete[] navBuffer;
	delete aiManager;
	delete physicsComponent;
	delete renderComponent;

	system("pause");
}