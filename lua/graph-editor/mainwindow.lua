local Graph = flat.require 'graph/graph'
local NodeWidget = flat.require 'graph-editor/nodewidget'

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

function MainWindow:close()
    self.window:removeFromParent()
end

function MainWindow:build()
    local window = Widget.makeColumnFlow()
    window:setMargin(25)
    window:setSizePolicy(Widget.SizePolicy.EXPAND)
    window:setBackgroundColor(0x2C3E50FF)

    local titleText
    do
        local titleContainer = Widget.makeLineFlow()
        titleContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

        do
            titleText = Widget.makeText('Node Graph Editor', table.unpack(flat.ui.settings.defaultFont))
            titleText:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
            titleText:setTextColor(0xECF0F1FF)
            titleText:setMargin(3, 0, 0, 3)
            titleText:mouseDown(function()
                window:drag()
            end)
            titleText:mouseUp(function()
                window:drop()
            end)

            titleContainer:addChild(titleText)
        end

        do
            local closeWindowButton = Widget.makeText('X', table.unpack(flat.ui.settings.defaultFont))
            closeWindowButton:setTextColor(0xFFFFFFFF)
            closeWindowButton:setMargin(3, 3, 0, 0)
            titleContainer:addChild(closeWindowButton)
            closeWindowButton:click(function()
                self:close()
            end)
        end

        window:addChild(titleContainer)
    end

    local content = Widget.makeCanvas(1, 1)
    content:setSizePolicy(Widget.SizePolicy.EXPAND)
    content:setMargin(3)
    content:setAllowScroll(true, true)
    content:setRestrictScroll(false, false)
    local lx, ly
    content:draw(function()
        content:clear(0xECF0F1FF)
        lx, ly = nil, nil
    end)
    content:mouseMove(function(w, x, y)
        if lx then
            w:drawLine(0x000000FF, 1, flat.Vector2(lx, ly), flat.Vector2(x, y))
        end
        lx, ly = x, y
    end)
    window:addChild(content)

    self.window = window
    self.editorContainer:addChild(window)
    self.titleText = titleText
    self.content = content
end

function MainWindow:setTitle(title)
    self.titleText:setText('Node Graph Editor - ' .. title)
end

function MainWindow:openGraph(graphPath)
    self:setTitle(graphPath)

    local graph = self:loadGraph(graphPath)
    local graphLayout = self:loadGraphLayout(graphPath)
    assert(#graph.nodeInstances == #graphLayout)
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        local nodePosition = graphLayout[i]
        local nodeWidget = self:makeNodeWidget(node)
        nodeWidget:setPosition(table.unpack(nodePosition))
        self.content:addChild(nodeWidget)
    end
end

function MainWindow:loadGraph(graphPath)
    local graph = Graph:new()
    graph:loadGraph(graphPath .. '.graph.lua')
    return graph
end

function MainWindow:loadGraphLayout(graphPath)
    local graphLayout = dofile(graphPath .. '.layout.lua')
    return graphLayout
end

function MainWindow:makeNodeWidget(node)
    local nodeWidget = NodeWidget:new(node)
    return nodeWidget.container
end

return MainWindow