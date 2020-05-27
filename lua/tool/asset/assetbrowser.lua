local AssetBrowser = {}
AssetBrowser.__index = AssetBrowser

local iconSize = 98
local iconPerLine = 3

function AssetBrowser:new(parent, path, options)
    local o = setmetatable({
        parent               = parent,
        path                 = path,
        options              = options or {},

        selectedAsset        = nil,
        selectedAssetIcon    = nil,

        selectionChanged     = flat.Slot:new(),
        selectionCleared     = flat.Slot:new()
    }, self)
    parent:setPadding(4)
    o:openDirectory(path)
    return o
end

function AssetBrowser:openDirectory(path)
    self.path = path

    local directories = Asset.getDirectories(path)
    local assets = Asset.getAssets(path)

    self.parent:removeAllChildren()

    local contentLine

    local directoryEntries = {}

    -- parent directory
    do
        local directory = Asset.getParentDirectory(path);
        if directory then
            directoryEntries[#directoryEntries + 1] = {
                name = '..',
                path = directory
            }
        end
    end
    for i = 1, #directories do
        local directory = directories[i]
        local directoryName = directory:gsub('^.+[/\\](.+)$', '%1')
        directoryEntries[#directoryEntries + 1] = {
            name = directoryName,
            path = directory
        }
    end

    for i = 1, #directoryEntries do
        local directoryEntry = directoryEntries[i]

        local directoryIcon = Widget.makeFixedSize(iconSize, 0)
        directoryIcon:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.COMPRESS_Y)

        local directoryLabel = Widget.makeText('[D] ' .. directoryEntry.name, table.unpack(flat.ui.settings.defaultFont))
        directoryLabel:setMargin(2)
        directoryLabel:click(function()
            self:openDirectory(directoryEntry.path)
        end)
        directoryIcon:addChild(directoryLabel)

        if not contentLine or contentLine:getChildrenCount() == iconPerLine then
            contentLine = Widget.makeLineFlow()
            self.parent:addChild(contentLine)
        end
        contentLine:addChild(directoryIcon)
    end

    -- assets
    contentLine = nil

    for i = 1, #assets do
        local asset = assets[i]
        local assetIcon = flat.tool.asset.getIcon(asset, iconSize, self.options.allowSelection)
        if self.options.allowSelection then
            assetIcon.container:click(function()
                if assetIcon:isSelected() then
                    self:clearSelection()
                else
                    self:setSelection(asset, assetIcon)
                end
            end)
        else
            assetIcon.container:click(function()
                print('Opening asset ' .. asset:getPath())
                local assetData = flat.tool.asset.getAssetTypeData(asset:getType())
                assetData.click(asset)
                return true
            end)
        end

        if not contentLine or contentLine:getChildrenCount() == iconPerLine then
            contentLine = Widget.makeLineFlow()
            self.parent:addChild(contentLine)
        end
        contentLine:addChild(assetIcon.container)
    end
end

function AssetBrowser:setSelection(selectedAsset, selectedAssetIcon)
    if selectedAsset ~= self.selectedAsset then
        if self.selectedAsset then
            self.selectedAssetIcon:setSelected(false)
        end
        self.selectedAsset = selectedAsset
        self.selectedAssetIcon = selectedAssetIcon
        selectedAssetIcon:setSelected(true)
        self.selectionChanged(selectedAsset)
    end
end

function AssetBrowser:clearSelection()
    if self.selectedAsset then
        self.selectedAssetIcon:setSelected(false)
        self.selectedAsset = nil
        self.selectedAssetIcon = nil
        self.selectionCleared()
    end
end

function AssetBrowser:onSelectionChanged(callback)
    self.selectionChanged:on(callback)
end

function AssetBrowser:onSelectedAssetOfType(assetType, callback)
    self:onSelectionChanged(function(asset)
        if asset:getType() == assetType then
            callback(asset)
        end
    end)
end

function AssetBrowser:onSelectionCleared(callback)
    self.selectionCleared:on(callback)
end

function AssetBrowser:getSelectedAsset()
    return self.selectedAsset
end

return AssetBrowser