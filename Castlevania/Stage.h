#pragma once
#include <Windows.h>
#include "Simon.h"
#include "Boundary.h"

class Stage
{

public:
	Stage();
	~Stage();

	void init(int mapId,wstring mapName);
	void render();
	void update(DWORD dt);
	void loadContent();
	void onKeyDown(int keyCode) ;
	void onKeyUp(int keyCode) ;
	void keyState(BYTE *states) ;

	void updateCamera(DWORD dt) const;
	// add object to game objects list
	void add(GameObject* gameObject);
	void addObjectToList(GameObject* gameObject, vector<GameObject*> &container);
	// add object to item list
	void remove(GameObject* gameObject);
	void removeObjectFromList(GameObject* gameObject, vector<GameObject*>& container);

private:
	Simon* simon{};
	int mapId;
	wstring mapName;
	vector<GameObject*> listBoundary;

	void loadObjectFromFiles();
};

