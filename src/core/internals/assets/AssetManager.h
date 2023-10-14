#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "app/EventHandler.h"
#include "helper/errorhelper.h"
using namespace std::filesystem;

enum class AssetType
{
	UNKNOWN = 0,
	TEXT,
	CODE,
	FRAG_SHADER,
	VERT_SHADER,
	TEXTURE
};

struct Asset
{
	std::string path;
	std::string name;
	AssetType type;
	std::string data;
};

class AssetEventArgs : public EventArgs
{
public:
	Asset asset;
};

static class AssetManager
{
public:
	static int AssetLoadedCount;
	static Asset ErrorAsset;

	static std::map<std::string, Asset> AssetsLoaded;
	static Asset LoadAsset(std::string path, bool autoType = true, AssetType ty = AssetType::UNKNOWN); // Load an asset with an option asset type, or let it be autodetected(only with file extensions).
	static void DeleteAsset(std::string name); // Deletes an asset from the loaded assets list based on the name.
	static void DeleteAsset(Asset asset); // Deletes an asset from the loaded assets list based on a given asset object.
	static Asset GetLoadedAsset(std::string name); // Gets loaded asset from the name.
	static bool IsAssetLoaded(std::string name); // Is the asset of such name loaded?
	static int GetAssetsLoadedFromSamePathCount(std::string path); // Gets the amount of assets loaded from that same path, might be useful at some point... That's a long function name.
	static std::vector<Asset> GetAssetsLoadedFromSamePath(std::string path); // might aswell have this too.
	static void Destruct();
};

