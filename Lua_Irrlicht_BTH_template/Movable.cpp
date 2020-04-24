
#include "Movable.h"
#include <assert.h>
#include <aabbox3d.h>
IrrlichtDevice* Movable::device = nullptr;
scene::IAnimatedMesh* Movable::mesh = nullptr;


Movable::Movable()
{

	modelNode = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
	//mesh->setBoundingBox(core::aabbox3df(-3.f,-3.f,-3.f,3.f,3.f,3.f));
}

int Movable::Movable_New(lua_State* state)
{
	//Constructor in lua: Movable(name)
	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3))
	{
		//x y z, Sx, Sy, Sz, s standing for scaling
		float Sx = lua_tonumber(state, -1);
		float Sy = lua_tonumber(state, -2);
		float Sz = lua_tonumber(state, -3);

		lua_pop(state, 3);

		Movable** movable = reinterpret_cast<Movable**>(lua_newuserdata(state, sizeof(Movable*)));
		*movable = new Movable();
		(*movable)->modelNode->setScale(core::vector3df(Sx, Sy, Sz));

		luaL_getmetatable(state, "MetaMovable"); // Pushes onto the stack, the metatable associat the name in the registry
		lua_setmetatable(state, -2); // Pops a table from the stack and sets it as the new metatable for the value at the given index

	}
	else
	{
		std::cout << "Error in movable, not correct amount of numbers in init." << std::endl;
	}
	return 1; //Nr of args pushed to the stack
}

Movable* Movable::checkMovable(lua_State* state, int stackIndex)
{
	Movable* movablePtr = nullptr;

	void* checkPtr = luaL_testudata(state, stackIndex, "MetaMovable");  //Check if it is a userdata with the metatable metamonster.

	if (checkPtr != nullptr) // it's a movable if not null
		movablePtr = *(Movable**)checkPtr;

	assert(movablePtr != nullptr);

	return movablePtr; //If this is null, it is not an movable.
}

int Movable::Movable_SetPosition(lua_State* state)
{
	Movable* movable = checkMovable(state, 1);

	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3))
	{
		float x = lua_tonumber(state, -1);
		float y = lua_tonumber(state, -2);
		float z = lua_tonumber(state, -3);
		lua_pop(state, 3);

		movable->modelNode->setPosition(core::vector3df(x, y, z));
	}
	return 0;
}

int Movable::Movable_Delete(lua_State* state)
{
	Movable* movable = checkMovable(state, 1);
	movable->modelNode->remove();
	delete movable;

	//cout << "Deleted Movable" << endl;

	return 0;
}

int Movable::Movable_SetRotation(lua_State* state)
{
	Movable* movable = checkMovable(state, 1);
	
	float rot = 0.f;
	if (lua_isnumber(state, -1))
		rot = lua_tonumber(state, -1);

	movable->modelNode->setRotation(core::vector3df(0, rot, 0));

	return 0;
}

int Movable::Movable_SetScale(lua_State* state)
{
	Movable* movable = checkMovable(state, 1);

	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3) && movable)
	{

		float x = lua_tonumber(state, -1);
		float y = lua_tonumber(state, -2);
		float z = lua_tonumber(state, -3);
		lua_pop(state, 3);

		movable->modelNode->setScale(core::vector3df(x, y, z));
	}
	return 0;
}

int Movable::Movable_GetPosition(lua_State* state)
{

	Movable* movable = checkMovable(state, 1);
	lua_pushnumber(state, movable->modelNode->getPosition().Z);
	lua_pushnumber(state, movable->modelNode->getPosition().Y);
	lua_pushnumber(state, movable->modelNode->getPosition().X);

	return 3;
}

int Movable::Movable_GetScale(lua_State* state)
{
	Movable* movable = checkMovable(state, 1);
	lua_pushnumber(state, movable->modelNode->getScale().Z);
	lua_pushnumber(state, movable->modelNode->getScale().Y);
	lua_pushnumber(state, movable->modelNode->getScale().X);

	return 3;
}

void Movable::registerLuaCFunctions(lua_State* state) //Called externally once
{
	luaL_newmetatable(state, "MetaMovable"); //Our metatable for lua
	luaL_Reg movableFuncs[] =
	{
		{"new", Movable_New},
		{"setPosition", Movable::Movable_SetPosition},
		{"setScale", Movable::Movable_SetScale},
		{"setRotation", Movable::Movable_SetRotation},
		{"getPosition", Movable::Movable_GetPosition},
		{"getScale", Movable::Movable_GetScale},
		{"__gc", Movable::Movable_Delete}, //Garbage collect function on lua's side
		{NULL, NULL}
	};

	luaL_setfuncs(state, movableFuncs, 0);

	lua_pushvalue(state, -1); //Push table to top, pushes a copy of the element at given index onto stack

	lua_setfield(state, -1, "__index"); //Point __index field of meta table to itself. Same as t[k] = v

	lua_setglobal(state, "Movable"); //Expose 'Movable' to lua, sets value of 'Movable' table.
}



void Movable::initClass(IrrlichtDevice* device, lua_State* state)
{
	Movable::registerLuaCFunctions(state); //Registers functions for movable
	Movable::device = device;
	io::path filename("actualCube.obj");
	Movable::mesh = device->getSceneManager()->getMesh(filename);
}
