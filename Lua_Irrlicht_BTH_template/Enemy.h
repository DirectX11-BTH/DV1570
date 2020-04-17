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

class Enemy
{

private:
	
	irr::scene::IAnimatedMeshSceneNode* modelNode;
	int health = 100;
	std::string name = "default";
public:
	static IrrlichtDevice* device;
	static irr::scene::IAnimatedMesh* mesh;
	static void loadMeshForClass();

	Enemy(const char* name);
	
	static int Enemy_New(lua_State* state);

	static int Enemy_Print(lua_State* state);
	static int Enemy_SetHealth(lua_State* state);
	static int Enemy_SetPosition(lua_State* state);
	static int Enemy_Delete(lua_State* state);
	
	static Enemy* checkEnemy(lua_State* state, int stackIndex);

	static void registerLuaCFunctions(lua_State* state);
	static void initClass(IrrlichtDevice* device, lua_State* state);
};

