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
    self.path = path

    local directories = Asset.getDirectories(path)
    print 'directories='
    flat.dumpFlat(directories)

    local assets = Asset.getAssets(path)
    print 'assets='
    flat.dumpFlat(assets)

    print(self:getContent())
end

return AssetBrowser