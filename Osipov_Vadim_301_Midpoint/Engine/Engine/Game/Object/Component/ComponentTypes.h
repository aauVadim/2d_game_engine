#pragma once
//Basically all possible components there can be
enum class ComponentTypes
{
	//Used by Script Components
	g_kScriptComponent = 0,

	//Used if object has "is_movable" boolean set to true  
	g_kMovementComponent, 

	//Will be used by future sound components 
	g_kSoundComponent,
    g_kComponentCount
};
