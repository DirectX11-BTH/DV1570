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
#include <assert.h>

using namespace irr;
using namespace std;
using namespace scene;

class Projectile
{

private:
	irr::scene::IAnimatedMeshSceneNode* modelNode;

	float xSpeed = 0;
	float zSpeed = 0;
public:
	Projectile();

	static IrrlichtDevice* device;
	static irr::scene::IAnimatedMesh* mesh;

	static int Projectile_New(lua_State* state);
	static Projectile* checkProjectile(lua_State* state, int stackIndex);

	static int Projectile_Print(lua_State* state);
	static int Projectile_SetPosition(lua_State* state);
	static int Projectile_Delete(lua_State* state);
	static int Projectile_Move(lua_State* state);
	static int Projectile_GetPosition(lua_State* state);
	static int Projectile_GetVelocity(lua_State* state); //Returns x and z velocity

	static void registerLuaCFunctions(lua_State* state);
	static void initClass(IrrlichtDevice* device, lua_State* state);
};