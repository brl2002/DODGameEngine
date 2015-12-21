#pragma once

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "D3D.h"
#include "Camera.h"
#include "Grid.h"
#include "ColorShader.h"
#include "Position.h"
#include "FPS.h"
#include "CPU.h"
#include "FontShader.h"
#include "Text.h"
#include "Model.h"
#include "LightShader.h"
#include "Light.h"
#include "..\Entity.h"

class RenderComponent
{
public:
	RenderComponent();

	RenderComponent(const RenderComponent&);

	~RenderComponent();

	bool Initialize(HINSTANCE, HWND, int, int);

	void Shutdown();

	bool Update( Entity** entities, int startIndex, int numEntities, double deltaTime );

private:
	D3D* m_Direct3D;

	Camera* m_Camera;

	Model* m_Model;

	LightShader* m_LightShader;

	Light* m_Light;

	Grid* m_Grid;

	ColorShader* m_ColorShader;

	Position* m_Position;

	FPS* m_Fps;

	CPU* m_Cpu;

	FontShader* m_FontShader;

	Text* m_Text;
};