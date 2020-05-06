local UiSettings = require 'data/scripts/ui/uisettings'

local AssetBrowser = {}
AssetBrowser.__index = AssetBrowser
setmetatable(AssetBrowser, { __index = flat.ui.Window })

function AssetBrowser:new(parent, path)
    local o = flat.ui.Window:new(parent)
    setmetatable(o, self)
    o.path = path
    o:build()
    return o
end

function AssetBrowser:build()
    self:setTitle 'Asset Browser'
    self:openDirectory(self.path)
end

function AssetBrowser:openDirectory(path)
    print('Opening ' .. path)
    self.path = path

    local directories = Asset.getDirectories(path)
    local assets = Asset.getAssets(path)

    local content = Widget.makeColumnFlow()

    -- parent directory
    do
        local directory = Asset.getParentDirectory(path);
        if directory then
            local directoryLabel = Widget.makeText('[D] ..', table.unpack(flat.ui.settings.defaultFont))
            directoryLabel:setMargin(2)
            directoryLabel:setTextColor(0x111111FF)
            directoryLabel:click(function()
                self:openDirectory(directory)
            end)
            content:addChild(directoryLabel)
        end
    end

    -- sub directories
    for i = 1, #directories do
        local directory = directories[i]
        local directoryLabel = Widget.makeText('[D] ' .. directory, table.unpack(flat.ui.settings.defaultFont))
        directoryLabel:setMargin(2)
        directoryLabel:setTextColor(0x111111FF)
        directoryLabel:click(function()
            self:openDirectory(directory)
        end)
        content:addChild(directoryLabel)
    end

    -- assets
    for i = 1, #assets do
        local asset = assets[i]
        local assetLabel = Widget.makeText('[A-' .. asset:getType() .. '] ' .. asset:getName(), table.unpack(flat.ui.settings.defaultFont))
        assetLabel:setMargin(2)
        assetLabel:setTextColor(0x111111FF)
        assetLabel:click(function()
            print('Opening asset ' .. asset:getPath())
            local assetData = flat.tool.asset.getAssetTypeData(asset:getType())
            assetData.click(asset)
            return true
        end)
        content:addChild(assetLabel)
    end

    self:setContent(content)
end

return AssetBrowser