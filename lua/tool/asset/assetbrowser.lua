local UiSettings = require 'data/scripts/ui/uisettings'

local AssetBrowser = {}
AssetBrowser.__index = AssetBrowser
setmetatable(AssetBrowser, { __index = flat.ui.Window })

local iconSize = 98
local iconPerLine = 8

function AssetBrowser:new(parent, path, options)
    local o = flat.ui.Window:new(parent)
    setmetatable(o, self)
    o.path = path
    o.options = options or {}
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
    content:setSizePolicy(Widget.SizePolicy.EXPAND)
    content:setAllowScrollY(true)

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
        local directoryName = directory:gsub('^.+[/\\](.+)$', '%1')
        local directoryLabel = Widget.makeText('[D] ' .. directoryName, table.unpack(flat.ui.settings.defaultFont))
        directoryLabel:setMargin(2)
        directoryLabel:setTextColor(0x111111FF)
        directoryLabel:click(function()
            self:openDirectory(directory)
        end)
        content:addChild(directoryLabel)
    end

    -- assets
    local contentLine

    for i = 1, #assets do
        local asset = assets[i]
        local assetIcon = flat.tool.asset.getIcon(asset, iconSize, self.options.allowSelection)
        if not self.options.allowSelection then
            assetIcon.container:click(function()
                print('Opening asset ' .. asset:getPath())
                local assetData = flat.tool.asset.getAssetTypeData(asset:getType())
                assetData.click(asset)
                return true
            end)
        end

        if not contentLine or contentLine:getChildrenCount() == iconPerLine then
            contentLine = Widget.makeLineFlow()
            content:addChild(contentLine)
        end
        contentLine:addChild(assetIcon.container)
    end

    self:setContent(content)
end

return AssetBrowser