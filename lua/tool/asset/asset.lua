flat.tool = {}
flat.tool.asset = {}

local assetTypes = {}

function flat.tool.asset.addAssetType(assetType, newAsset, assetIcon, click, actions)
    assert(not assetTypes[assetType], 'Asset type ' .. tostring(assetType) .. ' is already defined')
    print('Defining asset type ' .. assetType)
    assetTypes[assetType] = {
        newAsset = newAsset,
        assetIcon = assetIcon,
        click = click,
        actions = actions
    }
end

function flat.tool.asset.getAssetTypeData(assetType)
    return assert(assetTypes[assetType], 'Asset type ' .. tostring(assetType) .. ' is not defined')
end

function flat.tool.asset.getIcon(asset, size, allowSelection)
    local assetTypeData = flat.tool.asset.getAssetTypeData(asset:getType())
    return assetTypeData.assetIcon(asset, size, allowSelection)
end

function flat.tool.asset.browser(browserContainer, path, options)
    local AssetBrowserWindow = flat.require 'tool/asset/assetbrowserwindow'
    AssetBrowserWindow:new(browserContainer, path, options)
end