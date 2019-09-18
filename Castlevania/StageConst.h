#pragma once

auto constexpr ID_SCENE_1 = 1;
auto constexpr MAP_NAME_PREFIX= L"Scene";
auto constexpr SCENE_PREFIX_PATH = L"Scenes\\";
auto constexpr SCENE_OBJECTS_PATH = L"_objects.txt";

enum MapObjectsId
{
	boundaries,
	items
};

#pragma region object id from file
auto constexpr BOUNDARY = 0;
auto constexpr ITEM= 1;
#pragma endregion object id from file
