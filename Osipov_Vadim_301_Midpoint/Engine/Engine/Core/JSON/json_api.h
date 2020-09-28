#pragma once

#ifndef __JSON_API__
#define __JSON_API__

#include <vector>
#include <unordered_map>

//This is the Serialization/Deserialization library 
//https://github.com/nlohmann/json
#include "json.hpp"

//Shortcut
using json = nlohmann::json;


class Scene;
class Object;
class GameField;

class Factory;
struct Config; 
//---------------------------------------------------------------------------------------------
// Used to load Tiled game map. WIP 
//---------------------------------------------------------------------------------------------
class json_api
{
	Factory* m_pFactory;
public:
	json_api(Factory* pFactory) : m_pFactory(pFactory) { }

	//---------------------------------------------------------------------------------------------
	// Reads Tiled .json File. Returns false if file had errors
	//---------------------------------------------------------------------------------------------
	bool ReadTiledFile(std::ifstream& file);
	
	//---------------------------------------------------------------------------------------------
	// Used to parse Scene out of given .json file 
	//---------------------------------------------------------------------------------------------
	bool MakeSceneFromJson(json& jsonFile);

	//---------------------------------------------------------------------------------------------
	// Used to read config.json
	//---------------------------------------------------------------------------------------------
	Config* ReadConfigFile(std::ifstream& fileRead);

private:
	void MakeTile(json obj, GameField* pGF);
	void LoadTileSetImage(json obj);
	void ConvertIndexToXAndY(int intdex, float& x, float& y, GameField* pGF);
	void MakeObjects(json obj);
	void MakeObjectComponents(json jsonObj, Object* pNewObject);
};

#endif // !__JSON_API__
