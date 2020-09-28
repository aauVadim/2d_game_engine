#pragma once

#include "Component/Component.h"
#include "Component/ComponentTypes.h"
#include "../Object/Object.h"

#include <vector>

//TODO: 
//	- Add SDL_MessageBox to assign script file to object. 
//	- Add Play/Edit modes. 


class Component;
class Transform;

class Object
{

	//Reference to our transform
	Transform* m_pTransform;
	int m_imageIndex;

	int m_ID;
	std::string m_objName; 
	
	Component* m_components[(int)ComponentTypes::g_kComponentCount] = { nullptr };
	bool m_isInteractible; 
	bool m_isSelected;

	//Objects Children. 
	std::vector<Object*> m_childrenGameObjects;

	// Parent game object is set to null in constructor and only set by factory. 
	// If there is no parent, then this game object has been instantiated from json 
	Object* m_pParentGameObject; 
public:
	//Defaulted to none for now
	Object(const std::string& objName, Transform* pTransform, int ID, int imageIndex);

	//Copy constructor, have to have it. 
	Object(Object& left);

	~Object();

	//------------------------------------------------------------------------------------------------
	// Object Name - can be specified in Tiled/JSON file. 
	//------------------------------------------------------------------------------------------------
	const std::string& GetObjectName() const { return m_objName; }

	//------------------------------------------------------------------------------------------------
	// Object ID - Each object should have unique ID. 
	//------------------------------------------------------------------------------------------------
	const int GetObjectID() const { return m_ID; }

	//------------------------------------------------------------------------------------------------
	// Used to set new object ID, in case object was spawned after json load. Example: Prefabs.
	//------------------------------------------------------------------------------------------------
	void SetNewObjectID(int newID);

	//------------------------------------------------------------------------------------------------
	// Reference to Transform of this object 
	//------------------------------------------------------------------------------------------------
	Transform* GetTransform() const { return m_pTransform; }

	//------------------------------------------------------------------------------------------------
	// Adds a child to this object. Example: Prefabs.
	//------------------------------------------------------------------------------------------------
	void AddChildObject(Object* pGameObject) { m_childrenGameObjects.push_back(pGameObject); }
	
	//------------------------------------------------------------------------------------------------
	// Children of this object. Example: Prefabs.
	//------------------------------------------------------------------------------------------------
	const std::vector<Object*>& GetChildrenVector() const { return m_childrenGameObjects; }

	//------------------------------------------------------------------------------------------------
	// If object has a parent class, it was spawned after load time. Example: Prefabs.
	//------------------------------------------------------------------------------------------------
	const Object* GetParentGameObject() const { return m_pParentGameObject; }

	//------------------------------------------------------------------------------------------------
	// Sets object parent, if object is spawned after essential game load. Example: Prefabs.
	//------------------------------------------------------------------------------------------------
	void SetParentObject(Object* pParentGameObject) { m_pParentGameObject = pParentGameObject; }

	//------------------------------------------------------------------------------------------------
	// Adds component to this game object 
	//------------------------------------------------------------------------------------------------
	void AddComponent(Component* pComponent) { m_components[(int)pComponent->GetCompType()] = pComponent; }

	//------------------------------------------------------------------------------------------------
	// Get component by component type. Cost: O(1)
	//------------------------------------------------------------------------------------------------
	Component* GetComponent(ComponentTypes compType) const { return m_components[(int)compType]; }

	//------------------------------------------------------------------------------------------------
	// Main Tick Update, used to update components
	//------------------------------------------------------------------------------------------------
	void Update();
	
	//------------------------------------------------------------------------------------------------
	// Returns if object selected or not. 
	//------------------------------------------------------------------------------------------------
	bool IsSelected() const { return m_isSelected; }

	//------------------------------------------------------------------------------------------------
	// Set selected object state
	//------------------------------------------------------------------------------------------------
	void SetSelected(bool selected) { if(m_isInteractible) m_isSelected = selected; }

	//------------------------------------------------------------------------------------------------
	// Return Object Image Index - Used by Renderer
	//------------------------------------------------------------------------------------------------
	int GetImageIndex() const { return m_imageIndex; }

	//------------------------------------------------------------------------------------------------
	// Responds to the Mouse Click event on the object 
	//------------------------------------------------------------------------------------------------
	void OnMouseDown(int mousePosX, int mousePosY);

private:
	void ToggleObjectOutline(int mousePosX, int mousePosY);
};

