#include "json_api.h"
#include <iostream>
#include <fstream>

//Object 
#include "../../Game/Scene/Scene.h"
#include "../../Game/GameField/GameField.h"
#include "../../Game/Object/Object.h"
#include "../../Game/Object/Transform/Transform.h"
#include "../../Game/Object/Component/ComponentTypes.h"
#include "../../Game/Object/Component/Component.h"
#include "../../Game/Object/Component/Concrete/ScriptComponent.h"
#include "../../Game/Object/Component/Concrete/MovementComponent.h"
//Logger
#include "../OS/Console/Logger.h"
//Factory
#include "../Factory/Factory.h"
//Events 
#include "../Engine/Event System/EventSystem.h"
#include "../Engine/Event System/ConcreteEvents/EventOnObjectCreated.h"
#include "../Engine/Event System/ConcreteEvents/EventOnPrefabCreated.h"
#include "../Engine/Event System/ConcreteEvents/EventOnGameFieldCreated.h"
#include "../Engine/Event System/ConcreteEvents/EventOnSceneCreated.h"
//Physics 
#include "../Physics/PhysicsSystem.h"
#include "../Physics/Implementation/SDLCollisionSystem.h"
#include "../Physics/Implementation/Box2DPhysics.h"

#include "../Factory/Resource/ResourseLoader.h"
//#include "../Factory/Resource/Config/Config.h"


Config* json_api::ReadConfigFile(std::ifstream& fileRead)
{
	Config* pConf = new Config();
	json confJson(fileRead);

	//Media Root
	std::string mediaRoot = confJson["media_root"];
	pConf->m_medialRoot = mediaRoot;
	//Map files folder
	std::string mapFilesFolder = confJson["map_files_folder"];
	pConf->m_mapFilesFolder = mapFilesFolder;
	//Map file
	std::string tiledMapFile = confJson["tiled_map_file"];
	pConf->m_tiledMapFile = tiledMapFile;
	//Script folder
	std::string scriptFolder = confJson["scripts_files_folder"];
	pConf->m_scriptFolder = scriptFolder;

	//Images folder
	std::string imagesFolder = confJson["images_files_folder"];
	pConf->m_imagesFolder = imagesFolder;
	return pConf;
}

//-------------------------------------------------------------------------------------------------
// Main TileD reader function
//-------------------------------------------------------------------------------------------------
bool json_api::ReadTiledFile(std::ifstream& file)
{
	//Creating JSON object
	json jsonTiled(file);
	//We're just basically bailing out is the JSON is bad
	if (!MakeSceneFromJson(jsonTiled))
		return false;

	return true;
}

bool json_api::MakeSceneFromJson(json& jsonFile)
{
	// Creating Scene,
	Scene* pScene = new Scene();

	//Checking orientation. Going to bail if its not something 
	if (jsonFile["orientation"] != "orthogonal")
	{
		Logger::GetInstance()->LogError("JSON File: orientation is not orthogonal. Wont load the map.");
		return nullptr;
	}
	//Checking if we are dealing with right render order. 
	if (jsonFile["renderorder"] != "left-down")
	{
		Logger::GetInstance()->LogError("JSON File: Render order is not Left-Down, will not load the map");
		return nullptr;
	}

	//Creating Game Field
	GameField* pNewGameField = new GameField(jsonFile["height"], jsonFile["width"]);

	pNewGameField->m_tileWidth = jsonFile["tilewidth"];
	pNewGameField->m_tileHeight = jsonFile["tileheight"];
	
	//Game field layers, objects, tiles 
	for (auto itor = jsonFile["layers"].begin(); itor != jsonFile["layers"].end(); ++itor)
	{
		//Creating object for every element of array
		json obj(*itor);

		if (obj["type"] == "objectgroup")
		{
			for (auto objItor = obj["objects"].begin(); objItor != obj["objects"].end(); ++objItor)
			{
				json objJson(*objItor);
				MakeObjects(objJson);
			}
		}
		else if (obj["type"] == "tilelayer")
			MakeTile(obj, pNewGameField);
		else
			return false;
	}

	for (auto tileSetItor = jsonFile["tilesets"].begin(); tileSetItor != jsonFile["tilesets"].end(); ++tileSetItor)
	{
		json obj(*tileSetItor);
		LoadTileSetImage(obj);
	}

	//Making sure to give scene the game field. 
	pScene->SetGameField(pNewGameField);

	//FIXME! This is not right. 
	pScene->SetPhysicsSystem(new SDLCollisionSystem());
	//Calling event that Game Field was created
	EventSystem::AddEventForExecution(new EventOnSceneCreated(pScene));
	return true;
}

//-------------------------------------------------------------------------------------------------
// Makes a tiles for game field. 
// Basically whole lot of safety checks, and one function call. 
//-------------------------------------------------------------------------------------------------
void json_api::MakeTile(json obj, GameField* pGF)
{
	int index = 0;
	for (auto dataItor = obj["data"].begin(); dataItor != obj["data"].end(); ++dataItor)
	{
		// About local variables - I'd skip making them, 
		// but I json loader library likes to put it into a variable first, otherwise it will complain
		int imageIndex = *dataItor;
		//"0" - Means that tile is empty, so we shouldn't really worry about it. 
		if (imageIndex > 0)
		{
			//Is this tile walkable? 
			//Converting position out. 
			float x = 0.0f;
			float y = 0.0f;
			//Getting coordinate positions
			ConvertIndexToXAndY(index, x, y, pGF);
			//Calling event to let everyone know that new tile object is created.
			pGF->InserTile(new Tile(imageIndex, pGF->GetTileHeight(), pGF->GetTileWidth(), x, y, index), index);
		}
		++index; // Upping index
	}
}

//-------------------------------------------------------------------------------------------------
// Loads every tile set image that json has
//-------------------------------------------------------------------------------------------------
void json_api::LoadTileSetImage(json obj)
{
	int	columns = obj["columns"];
	int firstGID = obj["firstgid"];
	int	imageHeight = obj["imageheight"];
	int	imageWidth = obj["imagewidth"];
	int	margin = obj["margin"]; 
	int	spacing = obj["spacing"];
	int	tileCount = obj["tilecount"];
	int	tileHeight = obj["tileheight"];
	int	tileWidth = obj["tilewidth"];

	std::string	name = obj["name"];
	
	Logger::GetInstance()->Log("Created tile atlas: ( %s )", name.c_str());
	ResourseLoader::CreateTextureAtlas(m_pFactory->GetCurrentConfiguration(), name, columns, firstGID, imageHeight, imageWidth, margin, spacing, tileCount, tileHeight, tileWidth);
}

//-------------------------------------------------------------------------------------------------
// Little utility function, not using anywhere but here. 
//-------------------------------------------------------------------------------------------------
void json_api::ConvertIndexToXAndY(int index, float& x, float& y, GameField* pGF)
{
	x = static_cast<float>(index % pGF->GetFieldWidth());
	y = static_cast<float>((index - x) / pGF->GetFieldWidth());

	x *= static_cast<float>(pGF->GetTileWidth());
	y *= static_cast<float>(pGF->GetTileHeight());
}

void json_api::MakeObjects(json obj)
{
	//Basic variables, should be there by default
	//auto findIt = obj.find("gid");
	int objGID = obj.find("gid") != obj.end() ? obj["gid"] : NULL;
	//Scale
	int objHeight = obj["height"];
	int objWidth = obj["width"];
	//Tiled ID 
	int objID = obj["id"];
	//Position
	int objPosX = obj["x"];
	int objPosY = obj["y"];
	//Object Name
	std::string objName = obj.find("name") != obj.end() ? obj["name"] : "NONE" ;
	//Rotation 
	float objRotation = obj["rotation"];
	//Is it a prefab? 
	bool isPrefab = (obj.find("properties") != obj.end() && obj["properties"].find("prefab") != obj["properties"].end()) ? obj["properties"]["prefab"] : false;

	Object* pNewGO = new Object(objName, new Transform(static_cast<float>(objPosX), static_cast<float>(objPosY), objHeight, objWidth, objRotation), objID, objGID);

	//If we have something stored in properties, dig it out 
	if (obj.find("properties") != obj.end())
	{
		MakeObjectComponents(obj, pNewGO);
	}
	else
	{
		Logger::GetInstance()->Log("Dealing with Object with no properties");
	}

	//if this is a prefab - do not add it to anything, just store it with Factory
	if (isPrefab)
		EventSystem::AddEventForExecution(new EventOnPrefabCreated(pNewGO));
	//Else we just have Game Object from the scene
	else 
		EventSystem::AddEventForExecution(new EventOnObjectCreated(pNewGO));
}

void json_api::MakeObjectComponents(json jsonObj, Object* pNewObject)
{
	//We have a script component 
	if (jsonObj["properties"].find("script") != jsonObj["properties"].end())
	{
		//just a safety check
		if (jsonObj["properties"]["script"].is_string())
		{
			//I have to do it this way, for some reason library that I'm using doesn't like to pass value directly to constructors.
			//Maybe its one of them .NET type of "Lazy" implementations? [???]
			std::string scriptName = jsonObj["properties"]["script"];
			pNewObject->AddComponent(new ScriptComponent(pNewObject, scriptName));
		}
	}
	//We should make MovementComponent
	
	
	if (jsonObj["properties"].find("is_movable") != jsonObj["properties"].end())
	{
		if (jsonObj["properties"]["is_movable"].is_boolean() && jsonObj["properties"]["is_movable"])
		{
			float objRotation = jsonObj["rotation"];
			pNewObject->AddComponent(new MovementComponent(pNewObject, objRotation));
		}
	}
}


