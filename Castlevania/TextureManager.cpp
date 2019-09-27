#include "TextureManager.h"

TextureManager* TextureManager::instance = NULL;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void TextureManager::add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO imageInfo;

	auto result = D3DXGetImageInfoFromFile(filePath, &imageInfo);

	if (result != D3D_OK) return;

	auto d3ddv = Game::getInstance()->getDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		imageInfo.Width,							// Texture width
		imageInfo.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&imageInfo,
		NULL,
		&texture
	);

	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textures[id] = texture;
	DebugOut(L"[INFO] Load texture successfully: id=%d, path=%s \n", id, filePath);
}