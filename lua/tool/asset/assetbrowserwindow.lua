local AssetBrowser = flat.require 'tool/asset/assetbrowser'

local AssetBrowserWindow = {}
AssetBrowserWindow.__index = AssetBrowserWindow
setmetatable(AssetBrowserWindow, { __index = flat.ui.Window })

function AssetBrowserWindow:new(parent, path, options)
    local o = flat.ui.Window:new(parent)
    setmetatable(o, self)
    o:build()
    o.assetBrowser = AssetBrowser:new(o:getContent(), path, options)
    return o
end

function AssetBrowserWindow:build()
    self:setTitle 'Asset Browser'

    local content = Widget.makeColumnFlow()
    content:setSizePolicy(Widget.SizePolicy.EXPAND)
    content:setAllowScrollY(true)
    self:setContent(content)
end

return AssetBrowserWindow