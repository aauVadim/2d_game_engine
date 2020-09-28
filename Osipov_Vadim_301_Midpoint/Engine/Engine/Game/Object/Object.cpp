#include "Object.h"

#include "Transform/Transform.h"
#include "Component/Component.h"
#include "Component/Concrete/ScriptComponent.h"
#include "Component/Concrete/MovementComponent.h"
#include "../../../Engine/Core/OS/Console/Logger.h"

//Create
Object::Object(const std::string& objName, Transform* pTransform, int ID, int imageIndex)
	: m_ID(ID)
	, m_objName(objName)
	, m_isInteractible(imageIndex != NULL)
	, m_pParentGameObject(nullptr)
	, m_pTransform(pTransform)
	, m_imageIndex(imageIndex)
{

}
//Copy 
Object::Object(Object& left)
	: m_ID(left.GetObjectID())
	, m_objName(left.GetObjectName())
	, m_isInteractible(left.GetImageIndex() != NULL)
	, m_pParentGameObject(nullptr)
	, m_imageIndex(left.GetImageIndex())
	, m_pTransform(new Transform(*left.GetTransform()))
{
	//Copy components over
	for (int i = 0; i < (int)ComponentTypes::g_kComponentCount; ++i)
	{
		Component* pComp = left.GetComponent((ComponentTypes)i);
		if (pComp != nullptr)
		{
			switch (pComp->GetCompType())
			{
			case ComponentTypes::g_kScriptComponent:
				m_components[i] = new ScriptComponent(this, ((ScriptComponent*)pComp)->GetScriptName());
				break;
			case ComponentTypes::g_kMovementComponent:
				m_components[i] = new MovementComponent(this, GetTransform()->GetObjectRotation());
			}
		}
	}
}
//Nuke 
Object::~Object()
{
	//we have children objects, so lets delete them too. 
	if (!m_childrenGameObjects.empty())
	{
		for (Object* pChild : m_childrenGameObjects)
			delete pChild;
	}

	//Clearing components out
	for (int i = 0; i < (int)ComponentTypes::g_kComponentCount; ++i)
	{
		if (m_components[i] != nullptr)
			delete m_components[i];
	}

	if(m_pTransform)
		delete m_pTransform;
}

void Object::SetNewObjectID(int newID)
{
	std::string newName(m_objName + std::to_string(newID));
	m_objName = newName;
	m_ID = newID;
}

void Object::Update()
{
	//Updating components
	for (int i = 0; i < (int)ComponentTypes::g_kComponentCount; ++i)
	{
		if(m_components[i] != nullptr) m_components[i]->UpdateComponent();
	}
}

//-------------------------------------------------------------------------------------------------
// Selects/De-selects Object if player clicked on it. 
//-------------------------------------------------------------------------------------------------
void Object::OnMouseDown(int mousePosX, int mousePosY)
{
	//See if we should outline the object
	//ToggleObjectOutline(mousePosX, mousePosY);
	//Notifying children if we have any
	if (!m_childrenGameObjects.empty())
	{
		for (Object* pChild : m_childrenGameObjects)
			pChild->OnMouseDown(mousePosX, mousePosY);
	}
	//Update components
	for (int i = 0; i < (int)ComponentTypes::g_kComponentCount; ++i)
	{
		if (m_components[i] != nullptr)
			m_components[i]->OnMouseClick(mousePosX, mousePosY);
	}
}

void Object::ToggleObjectOutline(int mousePosX, int mousePosY)
{
	if (m_isInteractible)
	{
		float posX = m_pTransform->GetPositionX();
		float posY = m_pTransform->GetPositionY();
		int height, width;
		m_pTransform->GetObjectScale(width, height);
		if ((mousePosX >= posX) && (mousePosX < (posX + width)) && (mousePosY >= posY) && (mousePosY < (posY + height)))
			m_isSelected = !m_isSelected;
	}
}
