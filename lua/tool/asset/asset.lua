flat.tool = {}
flat.tool.asset = {}

local assetTypes = {}

function flat.tool.asset.addAssetType(assetType, newAsset, click, actions)
    assert(not assetTypes[assetType], 'Asset type ' .. tostring(assetType) .. ' is already defined')
    print('Defining asset type ' .. assetType)
    assetTypes[assetType] = {
        newAsset = newAsset,
        click = click,
        actions = actions
    }
end

function flat.tool.asset.getAssetTypeData(assetType)
    return assert(assetTypes[assetType], 'Asset type ' .. tostring(assetType) .. ' is not defined')
end

function flat.tool.asset.browser(browserContainer, path)
    local AssetBrowser = flat.require 'tool/asset/assetbrowser'
    AssetBrowser:new(browserContainer, path)
end