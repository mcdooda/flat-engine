flat.tool = {}
flat.tool.asset = {}

local assetTypes = {}

function flat.tool.asset.addAssetType(assetType, newAsset, click, actions)
    assert(not assetTypes[assetType], 'Asset type ' .. tostring(assetType) .. ' is already defined')
    assetTypes[assetType] = {
        newAsset = newAsset,
        click = click,
        actions = actions
    }
end

function flat.tool.asset.browser(browserContainer, path)
    local AssetBrowser = flat.require 'tool/asset/assetbrowser'
    AssetBrowser:new(browserContainer, path)
end