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
    window:setBackgroundColor(0x333333FF)
    local titleText = Widget.makeText('Node Graph Editor', table.unpack(flat.ui.settings.defaultFont))
    titleText:setMargin(3, 0, 0, 3)
    window:addChild(titleText)
    local content = Widget.makeColumnFlow()
    content:setSizePolicy(Widget.SizePolicy.EXPAND)
    content:setMargin(3)
    content:setBackgroundColor(0xCCCCCCFF)
    window:addChild(content)
    self.editorContainer:addChild(window)
    self.titleText = titleText
end

function MainWindow:setTitle(title)
    self.titleText:setText('Node Graph Editor - ' .. title)
end

function MainWindow:openGraph(graphPath)
    self:setTitle(graphPath)
end

return MainWindow