#pragma once
#pragma comment(lib, "Irrlicht.lib")
#ifdef _DEBUG
#pragma comment(lib, "LuaLibd.lib")
#else
#pragma comment(lib, "Lualib.lib")
#endif
#include <irrlicht.h>
#include <iostream>
#include <lua.hpp>
#include "lua.hpp"
#include <string>
#include <unordered_map>
//using namespace scene;
using namespace irr;
using namespace std;
using namespace scene;

class Movable
{

private:

public:
	irr::scene::IAnimatedMeshSceneNode* modelNode;
	irr::scene::IAnimatedMesh* mesh;
	static IrrlichtDevice* device;
	static unordered_map<std::string, irr::scene::IAnimatedMesh*>* meshDictionary;

	Movable();

	static int Movable_New(lua_State* state);
	static int Movable_SetPosition(lua_State* state);
	static int Movable_Delete(lua_State* state);
	static int Movable_SetRotation(lua_State* state);
	static int Movable_SetScale(lua_State* state);
	static int Movable_GetPosition(lua_State* state);
	static int Movable_GetScale(lua_State* state);


	static Movable* checkMovable(lua_State* state, int stackIndex);

	static void registerLuaCFunctions(lua_State* state);
	static void initClass(IrrlichtDevice* device, lua_State* state);
};

