// EngineEventTypes.h
#ifndef ENGINEEVENTTYPES_H
#define ENGINEEVENTTYPES_H


enum class EventID
{
	k_NULL_EVENT = 0, 

	//Object Events
	g_kEventOnObjectCreated,			//Used for object creation.
	g_kEventOnObjectDestroyed,			//Used when object is destroyed.
	g_kEventOnPrefabCreated,			//When Prefab Object is read from json
	g_kEventOnSceneCreated,				//When Factory created/read scene
	g_kEventOnCloseGameRequested,		//When objects requesting to close game. 

	//Object Creation/Destruction
	g_kEventOnCreateGameObjectRequested,	//When LUA Requests object creation
	g_kEventOnDestroyGameObjectRequested,	//When LUA Requests object destruction
	//Window Events
	g_kEventOnMouseClick, 				//Used for Mouse-Down
	g_kEventOnMarqueeSelect, 
	g_kEventOnGameWindowClosed,
	g_kEventOnKeyboardInput,			//Keyboard

	//Number of overall events
	k_NUM_EVENTS
};

#endif //ENGINEEVENTTYPES_H
