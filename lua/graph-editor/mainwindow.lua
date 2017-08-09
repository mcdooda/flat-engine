local MainWindow = {}
MainWindow.__index = MainWindow

function MainWindow:open(editorContainer)
    assert(editorContainer)
    local o = setmetatable({
        editorContainer = editorContainer
    }, self)
    o:build()
    return o
end

function MainWindow:build()
    local window = Widget.makeColumnFlow()
    window:setMargin(25)
    window:setSizePolicy(Widget.SizePolicy.EXPAND)
    window:setBackgroundColor(0xFF0000FF)
    local text = Widget.makeText('NODE GRAPH EDITOR', table.unpack(flat.ui.settings.defaultFont))
    window:addChild(text)
    self.editorContainer:addChild(window)
end

function MainWindow:openGraph(graphPath)
    print('OPEN GRAPH ' .. graphPath)
end

return MainWindow