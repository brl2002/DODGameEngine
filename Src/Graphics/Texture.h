#pragma once

#pragma comment(lib, "DirectXTK.lib")

#include <d3d11.h>
#include <DDSTextureLoader.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};
