#include "AssetManager.h"

int AssetManager::AssetLoadedCount;
std::map<std::string, Asset> AssetManager::AssetsLoaded;
Asset AssetManager::ErrorAsset{"error", "error", AssetType::UNKNOWN, "error"};

std::string GetFileExtension(const std::string& filePath) {
    size_t dotPosition = filePath.find_last_of(".");
    if (dotPosition != std::string::npos) {
        return filePath.substr(dotPosition + 1);
    }
    return ""; // If there is no extension
}

std::string GetFileNameWithoutExtension(const std::string& filePath) {
    size_t lastSlash = filePath.find_last_of("/\\");
    size_t lastDot = filePath.find_last_of(".");

    if (lastDot != std::string::npos && (lastSlash == std::string::npos || lastDot > lastSlash)) {
        return filePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
    }

    return filePath; // If there are no dots or slashes, the entire string is treated as the file name
}

std::map<std::string, AssetType> ExtToTyMap
{
    {"frag", AssetType::FRAG_SHADER},
    {"vert", AssetType::VERT_SHADER},
    {"txt", AssetType::TEXT},
    {"png", AssetType::TEXTURE},
    {"lua", AssetType::CODE}
};

AssetType TypeAutodecide(std::string path)
{
    std::string ext = GetFileExtension(path);

    if (!ext.empty())
    {
        return ExtToTyMap[ext];
    }
    else
    {
        SendErrorEvent("Could not automatically deduct type of " + path + ". (no extension given.)\nReverting to type text.");
        return AssetType::TEXT;
    }
}

std::string GenAssetName(std::string n)
{
    int cnt = AssetManager::GetAssetsLoadedFromSamePathCount(n);
    std::string name = GetFileNameWithoutExtension(n);

    if (cnt > 0)
    {
        return name + " (" + std::to_string(cnt) + ")";
    }
    
    return name;
}

std::vector<Asset> AssetManager::GetAssetsLoadedFromSamePath(std::string path)
{
    std::vector<Asset> found;

    for (const std::pair<std::string, Asset>& ast : AssetManager::AssetsLoaded)
    {
        if (ast.second.path == path)
        {
            found.push_back(ast.second);
        }
    }

    return found;
}

int AssetManager::GetAssetsLoadedFromSamePathCount(std::string path)
{
    int c = 0;

    for (const std::pair<std::string, Asset>& ast : AssetManager::AssetsLoaded)
    {
        if (ast.second.path == path)
        {
            c++;
        }
    }

    return c;
}

Asset AssetManager::LoadAsset(std::string path, bool autoType, AssetType ty)
{
    Asset asset;

    AssetType type;

    if (autoType)
        type = TypeAutodecide(path);
    else
        type = ty;

    std::string name = GenAssetName(path);

    if (type == AssetType::TEXTURE)
    {
        std::string text;

        std::ifstream readFile(path, std::ios::binary);

        while (getline(readFile, text)) {
            std::cout << text;
        }

        asset.data = text;

        readFile.close();
    }
    else
    {
        std::string text;

        std::ifstream readFile(path);

        readFile.unsetf(std::ios_base::skipws);

        if (readFile.is_open()) {
            while (getline(readFile, text)) {
                asset.data += text+"\n";
            }
        }
        else
        {
            std::cout << "Could not open file, missing or invalid.";
        }

        readFile.close();
    }
    //std::cout << asset.data << std::endl;
    asset.name = name;
    asset.path = path;
    asset.type = type;

    AssetLoadedCount++;
    AssetsLoaded.emplace(name, asset);

    std::shared_ptr<AssetEventArgs> args = std::make_shared<AssetEventArgs>();
    args->asset = asset;
    EventHandler::FireEvent(EventType::ASSET_LOADED, args);

    if (type == AssetType::FRAG_SHADER || type == AssetType::VERT_SHADER)
    {
        std::shared_ptr<AssetEventArgs> args = std::make_shared<AssetEventArgs>();
        args->asset = asset;
        EventHandler::FireEvent(EventType::ASSET_SHADER_LOAD, args);
    }

    return asset;
}

bool AssetManager::IsAssetLoaded(std::string name)
{
    bool eval = AssetManager::AssetsLoaded.find(name) != AssetManager::AssetsLoaded.end();
    return eval;
}

void AssetManager::Destruct()
{
    for (auto& i : AssetsLoaded)
    {
        DeleteAsset(i.first);
    }
}

void AssetManager::DeleteAsset(std::string name)
{
    Asset ast = GetLoadedAsset(name);


    if (ast.data != "error")
    {
        AssetsLoaded.erase(name);

        std::shared_ptr<AssetEventArgs> args = std::make_shared<AssetEventArgs>();
        args->asset = ast;
        EventHandler::FireEvent(EventType::ASSET_DESTROYED, args);
    }
    else
    {
        SendErrorEvent("Failed to delete asset because it is not loaded.");
    }
}

void AssetManager::DeleteAsset(Asset asset)
{
    if (IsAssetLoaded(asset.name))
    {
        AssetsLoaded.erase(asset.name);
        std::shared_ptr<AssetEventArgs> args = std::make_shared<AssetEventArgs>();
        args->asset = asset;
        EventHandler::FireEvent(EventType::ASSET_DESTROYED, args);
    }
    else
    {
        SendErrorEvent("Failed to delete asset because it is not loaded.");
    }
}

Asset AssetManager::GetLoadedAsset(std::string name)
{
    if (IsAssetLoaded(name))
        return AssetManager::AssetsLoaded[name];

    SendErrorEvent("Cannot get asset, since it isn't loaded.");
    return ErrorAsset;
}
