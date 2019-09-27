#pragma once
#include "Game.h"
#include "Constants.h"
using namespace std;

class TextureManager
{
private:
	static TextureManager* instance;
	unordered_map<int, LPDIRECT3DTEXTURE9> textures;
public:
	TextureManager();
	~TextureManager();

	void add(int id, LPCWSTR filePath, D3DCOLOR transparentColor = D3DCOLOR_XRGB(0, 128, 128));

	LPDIRECT3DTEXTURE9 get(int i) {
		return textures[i];
	}

	static TextureManager* getInstance() {
		if (instance == NULL) instance = new TextureManager();
		return instance;
	}
};
