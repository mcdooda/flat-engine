local Graph = flat.require 'graph/graph'
local NodeRepository = flat.require 'graph/noderepository'
local NodeWidget = flat.require 'graph-editor/nodewidget'

local MainWindow = {}
MainWindow.__index = MainWindow

function MainWindow:open(editorContainer)
    assert(editorContainer, 'no editor container')
    local o = setmetatable({
        editorContainer = editorContainer,
        nodeWidgets = {},
        currentLink = nil
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

    local function mouseMove(content, x, y)
        if self.currentLink then
            self:updateCurrentLink(x, y)
        end
    end

    local function mouseUp(content, x, y)
        if self.currentLink then
            self:clearCurrentLink()
        end
    end

    local function rightClick(content, x, y)
        self:openNodeListMenu(x, y)
    end

    content:scroll(draw)
    content:draw(draw)
    content:mouseMove(mouseMove)
    content:mouseUp(mouseUp)
    content:rightClick(rightClick)
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
    assert(#graph.nodeInstances == #graphLayout, 'graph and layout do not match')
    local content = self.content
    for i = 1, #graph.nodeInstances do
        local node = graph.nodeInstances[i]
        local nodePosition = graphLayout[i]
        local nodeWidget = self:makeNodeWidget(node)
        nodeWidget:setPosition(table.unpack(nodePosition))
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
    local nodeWidget = NodeWidget:new(node, self)
    nodeWidget.container:dragged(function()
        self:drawLinks()
    end)
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

                self:drawLink(
                    linkColor,
                    inputPinX, inputPinY,
                    outputPinX, outputPinY
                )
            end
        end
    end

    local currentLink = self.currentLink
    if currentLink then
        self:drawLink(
            currentLink.color,
            currentLink.inputPinX, currentLink.inputPinY,
            currentLink.outputPinX, currentLink.outputPinY
        )
    end
end

function MainWindow:updateCurrentLink(x, y)
    local currentLink = self.currentLink
    assert(currentLink, 'no current link')
    if currentLink.inputNode then
        currentLink.outputPinX = x
        currentLink.outputPinY = y
    else
        assert(currentLink.outputNode)
        currentLink.inputPinX = x
        currentLink.inputPinY = y
    end
    self:drawLinks()
end

function MainWindow:clearCurrentLink()
    local currentLink = self.currentLink
    assert(currentLink, 'no current link')
    if currentLink.inputNode then
        local nodeWidget = self.nodeWidgets[currentLink.inputNode]
        nodeWidget:setInputPinPlugged(currentLink.inputPin, false)
    else
        assert(currentLink.outputNode)
        local nodeWidget = self.nodeWidgets[currentLink.outputNode]
        nodeWidget:setOutputPinPlugged(currentLink.outputPin, #currentLink.outputPin.pluggedInputPins > 0)
    end
    self.currentLink = nil
    self:drawLinks()
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

function MainWindow:beginDragWireFromInputPin(inputNode, inputPin)
    assert(inputNode, 'no input node')
    assert(inputPin, 'no input pin')
    local color = self:getPinColor(inputNode, inputPin)
    local inputPinX, inputPinY = self:getInputPinPosition(inputNode, inputPin)
    local outputPinX, outputPinY = self:getMousePositionOnContent()
    self.currentLink = {
        inputNode = inputNode,
        inputPin = inputPin,
        color = color,
        inputPinX = inputPinX, inputPinY = inputPinY,
        outputPinX = outputPinX, outputPinY = outputPinY
    }
    local nodeWidget = self.nodeWidgets[inputNode]
    nodeWidget:setInputPinPlugged(inputPin, true)
    self:drawLinks()
end

function MainWindow:beginDragWireFromOutputPin(outputNode, outputPin)
    assert(outputNode, 'no output node')
    assert(outputPin, 'no input pin')
    local color = self:getPinColor(outputNode, outputPin)
    local inputPinX, inputPinY = self:getMousePositionOnContent()
    local outputPinX, outputPinY = self:getOutputPinPosition(outputNode, outputPin)
    self.currentLink = {
        outputNode = outputNode,
        outputPin = outputPin,
        color = color,
        inputPinX = inputPinX, inputPinY = inputPinY,
        outputPinX = outputPinX, outputPinY = outputPinY
    }
    local nodeWidget = self.nodeWidgets[outputNode]
    nodeWidget:setOutputPinPlugged(outputPin, true)
    self:drawLinks()
end

function MainWindow:linkReleasedOnInputPin(inputNode, inputPin)
    local currentLink = self.currentLink
    if currentLink and currentLink.outputNode then
        if inputPin.pinType == currentLink.outputPin.pinType and currentLink.outputNode ~= inputNode then
            if inputPin.pluggedOutputPin then
                local outputPin = inputPin.pluggedOutputPin.outputPin
                local previousOutputNode = inputPin.pluggedOutputPin.node
                inputNode:unplugInputPin(inputPin)
                local nodeWidget = self.nodeWidgets[previousOutputNode]
                nodeWidget:setOutputPinPlugged(outputPin, #outputPin.pluggedInputPins > 0)
            end
            currentLink.outputNode:plugPins(currentLink.outputPin, inputNode, inputPin)
            local nodeWidget = self.nodeWidgets[inputNode]
            nodeWidget:setInputPinPlugged(inputPin, true)
        else
            local nodeWidget = self.nodeWidgets[currentLink.outputNode]
            local outputPin = currentLink.outputPin
            nodeWidget:setOutputPinPlugged(outputPin, #outputPin.pluggedInputPins > 0)
        end
        self.currentLink = nil
        self:drawLinks()
    end
end

function MainWindow:linkReleasedOnOutputPin(outputNode, outputPin)
    local currentLink = self.currentLink
    if currentLink and currentLink.inputNode then
        if outputPin.pinType == currentLink.inputPin.pinType and currentLink.inputNode ~= outputNode then
            outputNode:plugPins(outputPin, currentLink.inputNode, currentLink.inputPin)
            local nodeWidget = self.nodeWidgets[outputNode]
            nodeWidget:setOutputPinPlugged(outputPin, true)
        else
            local nodeWidget = self.nodeWidgets[currentLink.inputNode]
            nodeWidget:setInputPinPlugged(currentLink.inputPin, false)
        end
        self.currentLink = nil
        self:drawLinks()
    end
end

function MainWindow:openNodeListMenu(x, y)
    self:closeNodeListMenu()

    local root = Widget.getRoot()

    local nodeListMenu = Widget.makeColumnFlow()
    nodeListMenu:setBackgroundColor(0xBDC3C7FF)
    nodeListMenu:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
    local mouseX, mouseY = Mouse.getPosition()
    local _, windowHeight = root:getSize()
    local nodeListMenuY = mouseY - windowHeight
    nodeListMenu:setPosition(mouseX, nodeListMenuY)

    local line = Widget.makeLineFlow()
    line:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

    local nodeRegistry = NodeRepository:getNodesForType(self.graph.nodeType)
    local nodesContainer
    local textInputWidget

    local searchNodes = {}
    local function updateNodeSearch()
        searchNodes = {}
        local search = textInputWidget:getText():lower()
        for nodeName, nodeClass in pairs(nodeRegistry) do
            local nodeVisualName = nodeClass:getName()
            if #search == 0 or nodeVisualName:lower():match(search) then
                searchNodes[#searchNodes + 1] = {
                    visualName = nodeVisualName,
                    name = nodeName
                }
            end
        end
        table.sort(searchNodes, function(a, b) return a.visualName < b.visualName end)
    end

    local function insertNode(nodeName)
        local node = self.graph:addNode(nodeRegistry[nodeName])
        local nodeWidget = self:makeNodeWidget(node)
        local contentSizeX, contentSizeY = self.content:getComputedSize()
        local nodeWidgetY = y - contentSizeY -- move the relative position from bottom left to top left
        nodeWidget:setPosition(x, nodeWidgetY)
        print(x, y, nodeWidgetY)
        self.content:addChild(nodeWidget)
        self:closeNodeListMenu()
    end

    local function submitInsertNode()
        if #searchNodes > 0 then
            local nodeName = searchNodes[1].name
            insertNode(nodeName)
        end
    end

    local function updateNodes()
        updateNodeSearch()
        nodesContainer:removeAllChildren()
        for i = 1, #searchNodes do
            local node = searchNodes[i]
            local nodeLabel = Widget.makeText(node.visualName, table.unpack(flat.ui.settings.defaultFont))
            nodeLabel:setTextColor(0x000000FF)
            nodeLabel:setMargin(2)
            nodeLabel:click(function()
                insertNode(node.name)
            end)
            nodesContainer:addChild(nodeLabel)
        end
    end

    local searchWidget = Widget.makeExpand()
    searchWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
    searchWidget:setMargin(2)
    searchWidget:setBackgroundColor(0xFFFFFFFF)

    textInputWidget = Widget.makeTextInput(table.unpack(flat.ui.settings.defaultFont))
    textInputWidget:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.FIXED_Y)
    textInputWidget:setTextColor(0x000000FF)
    textInputWidget:setMargin(1)
    textInputWidget:valueChanged(updateNodes)
    textInputWidget:submit(submitInsertNode)
    searchWidget:addChild(textInputWidget)

    line:addChild(searchWidget)

    local closeMenuButton = Widget.makeText('X', table.unpack(flat.ui.settings.defaultFont))
    closeMenuButton:setMargin(2)
    closeMenuButton:setTextColor(0x000000FF)
    closeMenuButton:click(function()
        self:closeNodeListMenu()
    end)
    line:addChild(closeMenuButton)

    nodeListMenu:addChild(line)

    local spacer = Widget.makeFixedSize(100, 1)
    nodeListMenu:addChild(spacer)

    nodesContainer = Widget.makeColumnFlow()
    updateNodes()
    nodeListMenu:addChild(nodesContainer)

    root:addChild(nodeListMenu)

    Widget.focus(textInputWidget)

    self.nodeListMenu = nodeListMenu
end

function MainWindow:closeNodeListMenu()
    if self.nodeListMenu then
        self.nodeListMenu:removeFromParent()
        self.nodeListMenu = nil
    end
end

function MainWindow:getMousePositionOnContent()
    return self.content:getRelativePosition(Mouse.getPosition())
end

return MainWindow