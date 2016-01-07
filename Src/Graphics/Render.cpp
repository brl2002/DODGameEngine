#include "Render.h"

RenderComponent::RenderComponent()
	:	m_Direct3D(0),
		m_Input(0),
		m_Camera(0),
		m_Model1(0),
		m_Model2(0),
		m_LightShader(0),
		m_Light(0),
		m_Grid(0),
		m_ColorShader(0),
		m_Position(0),
		m_PlayerInputDelta(0),
		m_Fps(0),
		m_Cpu(0),
		m_FontShader(0),
		m_Text(0)
{}

RenderComponent::RenderComponent(const RenderComponent& other)
{
}

RenderComponent::~RenderComponent()
{
}

bool RenderComponent::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	using namespace DirectX;

	bool result;
	float cameraX, cameraY, cameraZ;
	DirectX::XMMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new Input;
	if (!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(hinstance, hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new D3D;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	cameraX = 27.0f;
	cameraY = 40.0f;
	cameraZ = -32.0f;

	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	m_Camera->SetRotation(35, 0, 0);

	// Create the model object.
	m_Model1 = new Model;
	if (!m_Model1)
	{
		return false;
	}

	m_Model2 = new Model;
	if (!m_Model2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model1->Initialize(m_Direct3D->GetDevice(), "../data/cube.txt", L"../data/seafloor.dds", DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_Model2->Initialize(m_Direct3D->GetDevice(), "../data/cube.txt", L"../data/seafloor.dds", DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShader;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new Light;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-1.0f, -1.0f, 1.0f);

	// Create the terrain object.
	m_Grid = new Grid;
	if (!m_Grid)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Grid->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	m_ColorShader = new ColorShader;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new Position;
	if (!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer to the same as the initial camera position.
	m_Position->SetPosition(cameraX, cameraY, cameraZ);

	m_PlayerInputDelta = new Position;
	if (!m_PlayerInputDelta)
	{
		return false;
	}

	// Create the fps object.
	m_Fps = new FPS;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CPU;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the font shader object.
	m_FontShader = new FontShader;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new Text;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void RenderComponent::Shutdown()
{
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the position object.
	if (m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	if (m_PlayerInputDelta)
	{
		delete m_PlayerInputDelta;
		m_PlayerInputDelta = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the terrain object.
	if (m_Grid)
	{
		m_Grid->Shutdown();
		delete m_Grid;
		m_Grid = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = 0;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}
}

bool RenderComponent::Update( Entity** entities, int startIndex, int numEntities, double deltaTime )
{
	bool result;

	// Read the user input.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	// Update the system stats.
	m_Fps->Frame();

	m_Cpu->Frame();

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleInput(deltaTime, entities[startIndex]);
	if (!result)
	{
		return false;
	}

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// Render the grid buffers.
	m_Grid->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Grid->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	int lastIndex = startIndex + numEntities;
	DirectX::XMMATRIX worldMat;
	worldMat = DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * DirectX::XMMatrixTranslation(entities[startIndex]->position.x, 1, entities[startIndex]->position.y);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model1->Render(m_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMat, viewMatrix, projectionMatrix,
		m_Model1->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());

	for (int i = startIndex + 1; i < lastIndex; ++i)
	{
		worldMat = DirectX::XMMatrixScaling(0.3f, 0.3f, 0.3f) * DirectX::XMMatrixTranslation(entities[i]->position.x, 1, entities[i]->position.y);

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Model2->Render(m_Direct3D->GetDeviceContext());

		// Render the model using the light shader.
		result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMat, viewMatrix, projectionMatrix,
			m_Model2->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return result;
}

bool RenderComponent::HandleInput(float frameTime, Entity* entity)
{
	if (m_Input->IsEscapePressed())
	{
		return false;
	}

	if (m_Input->IsLeftPressed())
	{
		entity->position.x -= 10 * frameTime;
	}

	if (m_Input->IsRightPressed())
	{
		entity->position.x += 10 * frameTime;
	}

	if (m_Input->IsDownPressed())
	{
		entity->position.y -= 10 * frameTime;
	}

	if (m_Input->IsUpPressed())
	{
		entity->position.y += 10 * frameTime;
	}

	return true;
}