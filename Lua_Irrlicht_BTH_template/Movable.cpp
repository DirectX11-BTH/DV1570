
#include "Movable.h"
#include <assert.h>
#include <aabbox3d.h>
IrrlichtDevice* Movable::device = nullptr;
unordered_map<std::string, irr::scene::IMesh*>* Movable::meshDictionary = new unordered_map<std::string, irr::scene::IMesh*>;

void DumpStack(lua_State* L)
{
	std::cout << "------- STACK DUMP -------\n";
	for (int i = lua_gettop(L); i > 0; i--)
	{
		std::cout << "Index " << i << ": " << lua_typename(L, lua_type(L, i)) << "\n";
	}
	std::cout << "--------------------------\n";
}

Movable::Movable()
{

}

int Movable::Movable_New(lua_State* state)
{
	//Constructor in lua: Movable(name)
	if (lua_isnumber(state, -2) && lua_isnumber(state, -3) && lua_isnumber(state, -4) && lua_isstring(state, -1))
	{
		//x y z, Sx, Sy, Sz, s standing for scaling
		std::string path = lua_tostring(state, -1);
		float Sx = lua_tonumber(state, -2);
		float Sy = lua_tonumber(state, -3);
		float Sz = lua_tonumber(state, -4);

		lua_pop(state, 4);

		Movable** movable = reinterpret_cast<Movable**>(lua_newuserdata(state, sizeof(Movable*)));
		*movable = new Movable();

		
		io::path filename(path.data());
		auto found = meshDictionary->find(path);
		if (found == meshDictionary->end()) //If not found in the table
		{
			auto mesh = device->getSceneManager()->getMesh(filename); //Add it
			meshDictionary->insert(make_pair(path, mesh));
			(*movable)->modelNode = device->getSceneManager()->addMeshSceneNode(mesh);
		}
		else
		{
			(*movable)->modelNode = device->getSceneManager()->addMeshSceneNode(found->second); //Use already existing value (mesh)
		}
		assert((*movable)->modelNode != nullptr);


		(*movable)->modelNode->setScale(core::vector3df(Sx, Sy, Sz));
		(*movable)->modelNode->setPosition(core::vector3df(100, 100, 100));
		(*movable)->modelNode->setMaterialFlag(video::EMF_LIGHTING, true);
		luaL_getmetatable(state, "MetaMovable"); // Pushes onto the stack, the metatable associat the name in the registry
		lua_setmetatable(state, -2); // Pops a table from the stack and sets it as the new metatable for the value at the given index
	}
	else
	{
		std::cout << "Error in movable, not correct amount of numbers in init." << std::endl;
		std::cout << "Stack count: " << lua_gettop(state) << "\tExpected is 2." << std::endl;
		DumpStack(state);
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
int Movable::Movable_SetBB(lua_State* state)
{
	Movable* movable = checkMovable(state, 1);
	
	
	float bb = 0.f;
	string text = "";
	if (lua_isnumber(state, -2))
		
		text = lua_tostring(state, -1);
		bb = lua_tonumber(state, -2);

		io::path filename(text.data());
		
		auto mesh = device->getSceneManager()->getMesh(filename);
		mesh->setBoundingBox(core::aabbox3df(-bb, -bb, -bb, bb, bb, bb));

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
		{"setBB", Movable::Movable_SetBB},
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
}
