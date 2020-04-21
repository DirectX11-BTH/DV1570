#include "Projectile.h"

IrrlichtDevice* Projectile::device = nullptr;
scene::IAnimatedMesh* Projectile::mesh = nullptr;

Projectile::Projectile()
{
	modelNode = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
	modelNode->setScale(core::vector3df(0.2f, 0.2f, 0.2f));
}


int Projectile::Projectile_New(lua_State* state)
{
		//Constructor in lua: projectile(name)
	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3) && lua_isnumber(state, -4) && lua_isnumber(state, -5))
	{
		//const char* name = lua_tolstring(state, -1, nullptr);
		//assert(name != nullptr);

		float zSpeed = lua_tonumber(state, -1);
		float xSpeed = lua_tonumber(state, -2);

		//std::cout << xSpeed << " " << zSpeed << std::endl;

		float x = lua_tonumber(state, -3);
		float y = lua_tonumber(state, -4);
		float z = lua_tonumber(state, -5);

		lua_pop(state, 5);

		Projectile** projectile = reinterpret_cast<Projectile**>(lua_newuserdata(state, sizeof(Projectile*)));
		*projectile = new Projectile();

		(*projectile)->modelNode->setPosition(core::vector3df(x, y, z));
		(*projectile)->xSpeed = xSpeed;
		(*projectile)->zSpeed = zSpeed;


		luaL_getmetatable(state, "MetaProjectile"); // Pushes onto the stack, the metatable associat the name in the registry
		lua_setmetatable(state, -2); // Pops a table from the stack and sets it as the new metatable for the value at the given index


	}
	else
	{
		exit(1);
	}
	return 1; //Nr of args pushed to the stack
}

Projectile* Projectile::checkProjectile(lua_State* state, int stackIndex)
{
	Projectile* projectilePtr = nullptr;

	void* checkPtr = luaL_testudata(state, stackIndex, "MetaProjectile");  //Check if it is a userdata with the metatable metamonster.

	if (checkPtr != nullptr) // it's a Projectile if not null
		projectilePtr = *(Projectile**)checkPtr;

	assert(projectilePtr != nullptr);

	return projectilePtr; //If this is null, it is not an Projectile.
}

int Projectile::Projectile_Print(lua_State* state)
{
	Projectile* projectile = checkProjectile(state, 1);

	cout << "Projectile at: " << projectile->modelNode->getPosition().X << " " << projectile->modelNode->getPosition().Y << " " << projectile->modelNode->getPosition().Z << endl;

	return 0;
}

int Projectile::Projectile_SetPosition(lua_State* state)
{
		Projectile* projectile = checkProjectile(state, 1);

	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3))
	{
		float x = lua_tonumber(state, -1);
		float y = lua_tonumber(state, -2);
		float z = lua_tonumber(state, -3);
		lua_pop(state, 3);

		projectile->modelNode->setPosition(core::vector3df(x, y, z));
	}
	return 0;
}

int Projectile::Projectile_Delete(lua_State* state)
{
	Projectile* projectile = checkProjectile(state, 1);
	projectile->modelNode->remove();

	delete projectile;

	cout << "Deleted projectile" << endl;

	return 0;
}

int Projectile::Projectile_Move(lua_State* state)
{
	float x = lua_tonumber(state, -1);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -3);
	
	lua_pop(state, 3);

	Projectile* projectile = checkProjectile(state, 1);
	projectile->modelNode->setPosition(projectile->modelNode->getPosition() + core::vector3df(x, y, z));

	return 0;
}


int Projectile::Projectile_GetPosition(lua_State* state)
{
	Projectile* projectile = checkProjectile(state, 1);
	lua_pushnumber(state, projectile->modelNode->getPosition().Z);
	lua_pushnumber(state, projectile->modelNode->getPosition().Y);
	lua_pushnumber(state, projectile->modelNode->getPosition().X);

	return 3;
}

int Projectile::Projectile_GetVelocity(lua_State* state) //X then Z
{
	Projectile* projectile = checkProjectile(state, 1);
	lua_pushnumber(state, projectile->xSpeed);
	lua_pushnumber(state, projectile->zSpeed);


	return 2;
}

void Projectile::registerLuaCFunctions(lua_State* state)
{
	luaL_newmetatable(state, "MetaProjectile"); //Our metatable for lua
	luaL_Reg playerFuncs[] =
	{
		{"new", Projectile_New},
		{"print", Projectile::Projectile_Print},
		{"move", Projectile::Projectile_Move},
		{"getPosition", Projectile::Projectile_GetPosition},
		{"getVelocity", Projectile::Projectile_GetVelocity},
		{"__gc", Projectile::Projectile_Delete}, //Garbage collect function on lua's side
		{NULL, NULL}
	};

	luaL_setfuncs(state, playerFuncs, 0);

	lua_pushvalue(state, -1); //Push table to top, pushes a copy of the element at given index onto stack

	lua_setfield(state, -1, "__index"); //Point __index field of meta table to itself. Same as t[k] = v

	lua_setglobal(state, "Projectile"); //Expose 'Projectile' to lua
}

void Projectile::initClass(IrrlichtDevice* device, lua_State* state)
{
	Projectile::registerLuaCFunctions(state); //Registers functions for Projectile
	Projectile::device = device;
	io::path filename("actualCube.obj");
	Projectile::mesh = device->getSceneManager()->getMesh(filename);
}
