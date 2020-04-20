#pragma once
#ifdef _MSC_VER
// We'll define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif
#include <irrlicht.h>
#include <IEventReceiver.h>
using namespace irr;
using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class EventReceiver : public IEventReceiver
{


public:
	EventReceiver();
	//Struct to record the info on the mouse state
	struct EventMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		EventMouseState() : LeftButtonDown(false) { }
	} MouseState;

	virtual bool OnEvent(const SEvent& event)
	{
		//Different states of mouse
		if(event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			 case EMIE_LMOUSE_PRESSED_DOWN:

                MouseState.LeftButtonDown = true;
                break;

			case EMIE_LMOUSE_LEFT_UP:
                MouseState.LeftButtonDown = false;
                break;

            case EMIE_MOUSE_MOVED:
                MouseState.Position.X = event.MouseInput.X;
                MouseState.Position.Y = event.MouseInput.Y;
                break;

			 default:
				 break;
			}
		}
		return false;
	}
	const EventMouseState & GetMouseState(void) const
	{
		return MouseState;
	}
};