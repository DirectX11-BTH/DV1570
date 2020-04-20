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
//using namespace scene;
using namespace irr;
using namespace std;
using namespace scene;

class Player
{

private:

	irr::scene::IAnimatedMeshSceneNode* modelNode;
	int health = 100;
	std::string name = "default player";
public:
	static IrrlichtDevice* device;
	static irr::scene::IAnimatedMesh* mesh;
	static void loadMeshForClass();

	Player(const char* name);

	static int Player_New(lua_State* state);

	static int Player_Print(lua_State* state);
	static int Player_SetHealth(lua_State* state);
	static int Player_SetPosition(lua_State* state);
	static int Player_Delete(lua_State* state);
	static int Player_Move(lua_State* state);
	static int Player_SetRotation(lua_State* state);
	static int Player_GetPosition(lua_State* state);

	static Player* checkPlayer(lua_State* state, int stackIndex);

	static void registerLuaCFunctions(lua_State* state);
	static void initClass(IrrlichtDevice* device, lua_State* state);
};

