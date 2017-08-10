local Graph = flat.require 'graph/graph'

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
    local content = Widget.makeExpand()
    content:setMargin(3)
    content:setBackgroundColor(0xCCCCCCFF)
    window:addChild(content)
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
    local nodeWidget = Widget.makeColumnFlow()
    nodeWidget:setBackgroundColor(0x888888FF)
    nodeWidget:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)

    do
        local nodeNameText = Widget.makeText(node:getName(), table.unpack(flat.ui.settings.defaultFont))
        nodeNameText:setTextColor(0x000000FF)
        nodeNameText:setPositionPolicy(Widget.PositionPolicy.CENTER)
        nodeNameText:setMargin(3)
        nodeWidget:addChild(nodeNameText)
    end

    do
        local pinsWidget = Widget.makeLineFlow()

        do
            local inputPinsWidget = Widget.makeColumnFlow()
            inputPinsWidget:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
            --inputPinsWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            for i = 1, #node.inputPins do
                local inputPinWidget = Widget.makeText('<-', table.unpack(flat.ui.settings.defaultFont))
                inputPinWidget:setTextColor(0x000000FF)
                inputPinsWidget:addChild(inputPinWidget)
            end
            pinsWidget:addChild(inputPinsWidget)
        end

        do
            local centerMargin = Widget.makeFixedSize(20, 20)
            pinsWidget:addChild(centerMargin)
        end

        do
            local outputPinsWidget = Widget.makeColumnFlow()
            outputPinsWidget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
            --outputPinsWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            for i = 1, #node.outputPins do
                local outputPinWidget = Widget.makeText('->', table.unpack(flat.ui.settings.defaultFont))
                outputPinWidget:setTextColor(0x000000FF)
                outputPinWidget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
                outputPinsWidget:addChild(outputPinWidget)
            end
            pinsWidget:addChild(outputPinsWidget)
        end

        nodeWidget:addChild(pinsWidget)
    end

    return nodeWidget
end

return MainWindow