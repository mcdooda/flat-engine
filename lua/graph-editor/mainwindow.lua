local Graph = flat.require 'graph/graph'
local NodeWidget = flat.require 'graph-editor/nodewidget'

local MainWindow = {}
MainWindow.__index = MainWindow

function MainWindow:open(editorContainer)
    assert(editorContainer)
    local o = setmetatable({
        editorContainer = editorContainer,
        nodeWidgets = {}
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
    local function draw()
        self:drawLinks()
    end
    content:scroll(draw)
    content:draw(draw)
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
    local content = self.content
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        local nodePosition = graphLayout[i]
        local nodeWidget = self:makeNodeWidget(node)
        nodeWidget:setPosition(table.unpack(nodePosition))
        nodeWidget:dragged(function()
            self:drawLinks()
        end)
        content:addChild(nodeWidget)
    end

    self.graph = graph
    self.graphLayout = graphLayout

    content:redraw()
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
    self.nodeWidgets[node] = nodeWidget
    return nodeWidget.container
end

function MainWindow:drawLinks()
    local graph = self.graph

    self.content:clear(0xECF0F1FF)

    for i = 1, #graph.nodeInstances do
        local inputNode = graph.nodeInstances[i]
        for j = 1, #inputNode.inputPins do
            local inputPin = inputNode.inputPins[j]
            if inputPin.pluggedOutputPin then
                local outputNode = inputPin.pluggedOutputPin.node
                local outputPin = inputPin.pluggedOutputPin.outputPin

                local inputPinX, inputPinY = self:getInputPinPosition(inputNode, inputPin)
                local outputPinX, outputPinY = self:getOutputPinPosition(outputNode, outputPin)
                local linkColor = self:getPinColor(inputNode, inputPin)

                self:drawLink(linkColor, inputPinX, inputPinY, outputPinX, outputPinY)
            end
        end
    end
end

function MainWindow:drawLink(linkColor, inputPinX, inputPinY, outputPinX, outputPinY)
    local dx = math.min(50, math.abs(inputPinX - outputPinX) / 2)
    local dy = math.min(50, (inputPinY - outputPinY) / 2)
    local bezier = {
        flat.Vector2(outputPinX, outputPinY),
        flat.Vector2(outputPinX + dx, outputPinY),
        flat.Vector2(outputPinX + dx, outputPinY + dy),
        flat.Vector2(inputPinX - dx, inputPinY - dy),
        flat.Vector2(inputPinX - dx, inputPinY),
        flat.Vector2(inputPinX, inputPinY)
    }
    self.content:drawBezier(linkColor, 2, bezier)
end

function MainWindow:getPinColor(inputNode, inputPin)
    local nodeWidget = self.nodeWidgets[inputNode]
    return nodeWidget:getPinColorByType(inputPin.pinType)
end

function MainWindow:getInputPinPosition(inputNode, inputPin)
    local nodeWidget = self.nodeWidgets[inputNode]
    local inputPinPlugWidget = nodeWidget:getInputPinPlugWidget(inputPin)
    local x, y = self.content:getRelativePosition(inputPinPlugWidget)
    local sx, sy = inputPinPlugWidget:getSize()
    return x, y + sy / 2
end

function MainWindow:getOutputPinPosition(outputNode, outputPin)
    local nodeWidget = self.nodeWidgets[outputNode]
    local outputPinPlugWidget = nodeWidget:getOutputPinPlugWidget(outputPin)
    local x, y = self.content:getRelativePosition(outputPinPlugWidget)
    local sx, sy = outputPinPlugWidget:getSize()
    return x + sx, y + sy / 2
end

return MainWindow