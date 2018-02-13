local Graph = flat.require 'graph/graph'
local NodeWidget = flat.require 'graph-editor/nodewidget'
local PinTypes = flat.require 'graph/pintypes'

local MainWindow = {}
MainWindow.__index = MainWindow
setmetatable(MainWindow, { __index = flat.ui.Window })

function MainWindow:new(parent, metadata, onSave)
    local o = flat.ui.Window:new(parent)
    setmetatable(o, self)
    o.graph = nil
    o.graphPath = nil
    o.editorContainer = editorContainer
    o.nodeWidgets = {}
    o.selectedNodeWidgets = {}
    o.selectionWidget = nil
    o.initialSelectionPosition = nil
    o.currentLink = nil
    o.nodeListMenu = nil
    o.nodeContextualMenu = nil
    o.metadata = metadata
    o.onSave = onSave
    o.isNew = false
    o:build()
    return o
end

function MainWindow:build()
    self:setTitle 'Node Graph Editor'

    -- toolbar
    do
        local toolbar = self:addToolbar()
        local saveButton = toolbar:addButton('Save Graph')
        saveButton:click(function()
            self:saveGraph()
            self:saveGraphLayout()
            self:saveLuaRunnerFile()
            self:updateCustomNodeEditors()
            if self.onSave then
                self.onSave(self.isNew)
                self.isNew = false
            end
        end)
    end

    -- graph canvas
    do
        local content = Widget.makeCanvas(1, 1)
        content:setSizePolicy(Widget.SizePolicy.EXPAND)
        content:setAllowScroll(true, true)
        content:setAllowDragScrolling(true)
        content:setRestrictScroll(false, false)

        local function scroll()
            self:closeAllRightClickMenus()
            self:drawLinks()
        end

        local function draw()
            self:drawLinks()
        end

        local function mouseMove(content, x, y)
            assert(not (self.currentLink and self.selectionWidget))
            if self.currentLink then
                self:updateCurrentLink(x, y)
            elseif self.selectionWidget then
                self:updateSelectionWidget()
            end
        end

        local function mouseUp(content, x, y)
            assert(not (self.currentLink and self.selectionWidget))
            if self.currentLink then
                self:clearCurrentLink()
            elseif self.selectionWidget then
                self:selectWidgets()
            end
        end

        local function leftClick(content, x, y)
            self:clearSelectedWidgets()
            self:closeNodeListMenu()
            self:closeNodeContextualMenu()
            self:addSelectionWidget()
        end

        local function rightClick(content, x, y)
            self:openNodeListMenu(x, y)
        end

        content:scroll(scroll)
        content:draw(draw)
        content:mouseMove(mouseMove)
        content:mouseUp(mouseUp)
        content:mouseDown(leftClick)
        content:rightClick(rightClick)
        self:setContent(content)
    end
end

function MainWindow:close()
    self:closeAllRightClickMenus()

    flat.ui.Window.close(self)
end

function MainWindow:closeAllRightClickMenus()
    self:closeNodeListMenu()
    self:closeNodeContextualMenu()
end

function MainWindow:openGraph(graphPath, nodeType)
    self.graphPath = graphPath

    self:setTitle('Node Graph Editor - ' .. graphPath)

    local graph = self:loadGraph()
    self.graph = graph
    if graph.nodeType then
        -- the graph has been loaded
        assert(
            not nodeType or graph.nodeType == nodeType,
            'unexpected graph type: ' .. tostring(nodeType) .. ' expected, got ' .. tostring(graph.nodeType)
        )

        local graphLayout = self:loadGraphLayout()
        assert(
            #graph.nodeInstances == #graphLayout,
            'graph and layout do not match (' .. #graph.nodeInstances .. ' and ' .. #graphLayout .. ' nodes)'
        )
        local content = self:getContent()
        for i = 1, #graph.nodeInstances do
            local node = graph.nodeInstances[i]
            local nodePosition = graphLayout[i]
            local nodeWidget = self:makeNodeWidget(node)
            nodeWidget:setPosition(table.unpack(nodePosition))
            content:addChild(nodeWidget)
        end

        self:updateCustomNodeEditors()
        content:redraw()
        return true
    else
        -- the graph has not been loaded
        graph.nodeType = nodeType
        self.isNew = true
        return false
    end
end

function MainWindow:loadGraph()
    local graph = Graph:new()
    pcall(function()
        -- if the file does not exist, we want to create a new graph
        graph:loadGraph(self.graphPath .. '.graph.lua')
    end)
    return graph
end

function MainWindow:saveGraph()
    self.graph:saveGraph(self.graphPath .. '.graph.lua')
end

function MainWindow:loadGraphLayout()
    local graphLayout
    if not pcall(function()
        graphLayout = dofile(self.graphPath .. '.layout.lua')
    end) then
        graphLayout = {}
    end
    return graphLayout
end

function MainWindow:saveGraphLayout()
    local graphLayout = {}
    local nodeInstances = self.graph.nodeInstances
    for i = 1, #nodeInstances do
        local nodeInstance = nodeInstances[i]
        local nodeWidget = self.nodeWidgets[nodeInstance]
        local nodePosition = { nodeWidget.container:getPosition() }
        graphLayout[i] = nodePosition
    end

    local f = io.open(self.graphPath .. '.layout.lua', 'w')
    f:write 'return '
    flat.dumpToOutput(f, graphLayout)
    f:close()
end

function MainWindow:saveLuaRunnerFile()
    local componentFilePath = self.graphPath .. '.lua'
    if not io.open(componentFilePath, 'r') then
        local f = io.open(componentFilePath, 'w')
        f:write(([[return flat.graph.script.run '%s']]):format(self.graphPath))
        f:close()
    end
end

function MainWindow:makeNodeWidget(node)
    local nodeWidget = NodeWidget:new(node, self)
    nodeWidget.container:dragged(function()
        self:drawLinks()
    end)
    self.nodeWidgets[node] = nodeWidget
    return nodeWidget.container
end

function MainWindow:drawLinks(delayToNextFrame)
    local function draw()
        local graph = self.graph

        self:getContent():clear(0xECF0F1FF)

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

    if delayToNextFrame then
        Timer.start(0.05, nil, draw) -- TODO: Timer comes from CG!
    else
        draw()
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
    self:getContent():drawBezier(linkColor, 2, bezier)
end

function MainWindow:getPinColor(inputNode, inputPin)
    local nodeWidget = self.nodeWidgets[inputNode]
    return nodeWidget:getPinColorByType(inputPin.pinType)
end

function MainWindow:getInputPinPosition(inputNode, inputPin)
    local nodeWidget = self.nodeWidgets[inputNode]
    local inputPinPlugWidget = nodeWidget:getInputPinPlugWidget(inputPin)
    local x, y = self:getContent():getRelativePosition(inputPinPlugWidget)
    local sx, sy = inputPinPlugWidget:getSize()
    return x, y + sy / 2
end

function MainWindow:getOutputPinPosition(outputNode, outputPin)
    local nodeWidget = self.nodeWidgets[outputNode]
    local outputPinPlugWidget = nodeWidget:getOutputPinPlugWidget(outputPin)
    local x, y = self:getContent():getRelativePosition(outputPinPlugWidget)
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

function MainWindow:canPlugPins(outputNode, outputPin, inputNode, inputPin)
    return (inputPin.pinType == outputPin.pinType
        or ((inputPin.pinType == PinTypes.ANY) ~= (outputPin.pinType == PinTypes.ANY)))
        and outputNode ~= inputNode
end

function MainWindow:linkReleasedOnInputPin(inputNode, inputPin)
    local currentLink = self.currentLink
    if currentLink and currentLink.outputNode then
        if self:canPlugPins(currentLink.outputNode, currentLink.outputPin, inputNode, inputPin) then
            local updateInputNodeWidget = false
            local unpluggedOutputPin = false
            if inputPin.pluggedOutputPin then
                unpluggedOutputPin = true
                local outputPin = inputPin.pluggedOutputPin.outputPin
                local previousOutputNode = inputPin.pluggedOutputPin.node
                local updateOutputNodeWidget, updateInputNodeWidgetUnplug = inputNode:unplugInputPin(inputPin, true)

                do
                    local outputNodeWidget = self.nodeWidgets[previousOutputNode]
                    if updateOutputNodeWidget then
                        outputNodeWidget:rebuild()
                    else
                        outputNodeWidget:setOutputPinPlugged(outputPin, #outputPin.pluggedInputPins > 0)
                    end
                    outputNodeWidget:updateCustomNodeEditor()
                end

                do
                    updateInputNodeWidget = updateInputNodeWidget or updateInputNodeWidgetUnplug
                end
            end

            local updateOutputNodeWidget, updateInputNodeWidgetPlug = currentLink.outputNode:plugPins(currentLink.outputPin, inputNode, inputPin, unpluggedOutputPin)
            self.currentLink = nil
            self:drawLinks()
            
            do
                local outputNodeWidget = self.nodeWidgets[currentLink.outputNode]
                if updateOutputNodeWidget then
                    outputNodeWidget:rebuild()
                end
                outputNodeWidget:updateCustomNodeEditor()
            end

            do
                updateInputNodeWidget = updateInputNodeWidget or updateInputNodeWidgetPlug
                local inputNodeWidget = self.nodeWidgets[inputNode]
                if updateInputNodeWidget then
                    inputNodeWidget:rebuild()
                else
                    inputNodeWidget:setInputPinPlugged(inputPin, true)
                end
                inputNodeWidget:updateCustomNodeEditor()
            end
        else
            local nodeWidget = self.nodeWidgets[currentLink.outputNode]
            local outputPin = currentLink.outputPin
            nodeWidget:setOutputPinPlugged(outputPin, #outputPin.pluggedInputPins > 0)
            self.currentLink = nil
            self:drawLinks()
        end
    end
end

function MainWindow:linkReleasedOnOutputPin(outputNode, outputPin)
    local currentLink = self.currentLink
    if currentLink and currentLink.inputNode then
        if self:canPlugPins(outputNode, outputPin, currentLink.inputNode, currentLink.inputPin) then
            local updateOutputNodeWidget, updateInputNodeWidget = outputNode:plugPins(outputPin, currentLink.inputNode, currentLink.inputPin)
            self.currentLink = nil
            self:drawLinks()

            do
                local outputNodeWidget = self.nodeWidgets[outputNode]
                if updateOutputNodeWidget then
                    outputNodeWidget:rebuild()
                else
                    outputNodeWidget:setOutputPinPlugged(outputPin, true)
                end
                outputNodeWidget:updateCustomNodeEditor()
            end

            do
                local inputNodeWidget = self.nodeWidgets[currentLink.inputNode]
                if updateInputNodeWidget then
                    inputNodeWidget:rebuild()
                end
                inputNodeWidget:updateCustomNodeEditor()
            end
        else
            local nodeWidget = self.nodeWidgets[currentLink.inputNode]
            nodeWidget:setInputPinPlugged(currentLink.inputPin, false)
            self.currentLink = nil
            self:drawLinks()
        end
    end
end

function MainWindow:openRightClickMenu()
    local root = Widget.getRoot()
    local rightClickMenu = Widget.makeColumnFlow()
    rightClickMenu:setBackgroundColor(0xBDC3C7FF)
    rightClickMenu:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
    local mouseX, mouseY = Mouse.getPosition()
    local _, windowHeight = root:getSize()
    local rightClickMenuY = mouseY - windowHeight
    rightClickMenu:setPosition(mouseX, rightClickMenuY)
    root:addChild(rightClickMenu)
    return rightClickMenu
end

function MainWindow:openNodeListMenu(x, y)
    self:closeAllRightClickMenus()

    local nodeListMenu = self:openRightClickMenu()
    nodeListMenu:setSizePolicy(Widget.SizePolicy.FIXED)
    nodeListMenu:setSize(150, 350)

    local line = Widget.makeLineFlow()
    line:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)

    local nodeClasses = flat.graph.getNodeClasses(self.graph.nodeType)
    local nodesContainer
    local textInputWidget

    local searchNodes = {}
    local function updateNodeSearch()
        searchNodes = {}
        local search = textInputWidget:getText():lower()
        for nodeName, nodeClass in pairs(nodeClasses) do
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
        local content = self:getContent()
        local node = self.graph:addNode(nodeClasses[nodeName])
        local nodeWidget = self:makeNodeWidget(node)
        local contentSizeX, contentSizeY = content:getComputedSize()
        local scrollX, scrollY = content:getScrollPosition()
        local nodeWidgetX = x + scrollX
        local nodeWidgetY = y + scrollY - contentSizeY -- move the relative position from bottom left to top left
        nodeWidget:setPosition(nodeWidgetX, nodeWidgetY)
        content:addChild(nodeWidget)
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

    local nodesContainerScroller = Widget.makeColumnFlow()
    nodesContainerScroller:setSizePolicy(Widget.SizePolicy.EXPAND)
    nodesContainerScroller:setAllowScrollY(true)
    nodesContainer = Widget.makeColumnFlow()
    updateNodes()
    nodesContainerScroller:addChild(nodesContainer)
    nodeListMenu:addChild(nodesContainerScroller)

    Widget.focus(textInputWidget)

    self.nodeListMenu = nodeListMenu
end

function MainWindow:closeNodeListMenu()
    if self.nodeListMenu then
        self.nodeListMenu:removeFromParent()
        self.nodeListMenu = nil
    end
end

function MainWindow:openNodeContextualMenu()
    self:closeAllRightClickMenus()

    local nodeContextualMenu = self:openRightClickMenu()

    local deleteSelectedNodesLabel = Widget.makeText('Delete selected nodes', table.unpack(flat.ui.settings.defaultFont))
    deleteSelectedNodesLabel:setTextColor(0x000000FF)
    deleteSelectedNodesLabel:setMargin(2)
    deleteSelectedNodesLabel:click(function()
        self:deleteSelectedNodes()
        self:closeNodeContextualMenu()
    end)
    nodeContextualMenu:addChild(deleteSelectedNodesLabel)

    self.nodeContextualMenu = nodeContextualMenu
end

function MainWindow:closeNodeContextualMenu()
    if self.nodeContextualMenu then
        self.nodeContextualMenu:removeFromParent()
        self.nodeContextualMenu = nil
    end
end

function MainWindow:getMousePositionOnContent()
    return self:getContent():getRelativePosition(Mouse.getPosition())
end

function MainWindow:getMousePositionOnContentWithScrolling()
    local mouseX, mouseY = self:getContent():getRelativePosition(Mouse.getPosition())
    local scrollX, scrollY = self:getContent():getScrollPosition()
    return mouseX + scrollX, mouseY + scrollY
end

function MainWindow:isNodeSelected(nodeWidget)
    return self.selectedNodeWidgets[nodeWidget]
end

function MainWindow:selectNode(nodeWidget)
    if not self.selectedNodeWidgets[nodeWidget] then
        self.selectedNodeWidgets[nodeWidget] = true
        nodeWidget:select()
        return true
    end
    return false
end

function MainWindow:deselectNode(nodeWidget)
    if self.selectedNodeWidgets[nodeWidget] then
        self.selectedNodeWidgets[nodeWidget] = nil
        nodeWidget:deselect()
        return true
    end
    return false
end

function MainWindow:clearSelectedWidgets()
    for selectedNodeWidget in pairs(self.selectedNodeWidgets) do
        selectedNodeWidget:deselect()
    end
    self.selectedNodeWidgets = {}
end

function MainWindow:dragSelectedNodeWidgets()
    for selectedNodeWidget in pairs(self.selectedNodeWidgets) do
        selectedNodeWidget.container:drag()
    end
end

function MainWindow:dropSelectedNodeWidgets()
    for selectedNodeWidget in pairs(self.selectedNodeWidgets) do
        selectedNodeWidget.container:drop()
    end
end

function MainWindow:deleteSelectedNodes()
    for selectedNodeWidget in pairs(self.selectedNodeWidgets) do
        selectedNodeWidget.container:removeFromParent()
        self.graph:removeNode(selectedNodeWidget.node)
    end
    self.selectedNodeWidgets = {}
    self:updateAllNodesPinSocketWidgets()
    self:drawLinks()
end

function MainWindow:addSelectionWidget()
    assert(not self.selectionWidget)
    local content = self:getContent()
    local selectionWidget = Widget.makeFixedSize(0, 0)
    selectionWidget:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
    selectionWidget:setBackgroundColor(0x44444466)
    local mouseX, mouseY = self:getMousePositionOnContentWithScrolling()
    selectionWidget:setPosition(mouseX, mouseY)
    content:addChild(selectionWidget)
    self.selectionWidget = selectionWidget
    self.initialSelectionPosition = { mouseX, mouseY }
end

function MainWindow:updateSelectionWidget()
    assert(self.selectionWidget)
    local content = self:getContent()
    local mouseX, mouseY = self:getMousePositionOnContentWithScrolling()
    local initialSelectionX, initialSelectionY = table.unpack(self.initialSelectionPosition)
    local selectionX, selectionY = math.min(mouseX, initialSelectionX), math.min(mouseY, initialSelectionY)
    self.selectionWidget:setPosition(selectionX, selectionY)
    local selectionWidth, selectionHeight = math.abs(mouseX - initialSelectionX), math.abs(mouseY - initialSelectionY)
    self.selectionWidget:setSize(selectionWidth, selectionHeight)
end

function MainWindow:removeSelectionWidget()
    assert(self.selectionWidget)
    self.selectionWidget:removeFromParent()
    self.selectionWidget = nil
end

function MainWindow:selectWidgets()
    local selectionX, selectionY = self:getContent():getRelativePosition(self.selectionWidget)
    local selectionWidth, selectionHeight = self.selectionWidget:getSize()
    local selectionRight = selectionX + selectionWidth
    local selectionTop = selectionY + selectionHeight
    assert(selectionX <= selectionRight)
    assert(selectionY <= selectionTop)
    for node, nodeWidget in pairs(self.nodeWidgets) do
        local nodeWidgetX, nodeWidgetY = self:getContent():getRelativePosition(nodeWidget.container)
        local nodeWidgetWidth, nodeWidgetHeight = nodeWidget.container:getComputedSize()
        local centerX, centerY = nodeWidgetX + nodeWidgetWidth / 2, nodeWidgetY + nodeWidgetHeight / 2
        if selectionX <= centerX and centerX <= selectionRight and selectionY <= centerY and centerY <= selectionTop then
            self:selectNode(nodeWidget)
        end
    end
    self:removeSelectionWidget()
end

function MainWindow:updateAllNodesPinSocketWidgets()
    for node, nodeWidget in pairs(self.nodeWidgets) do
        nodeWidget:updatePinSocketWidgets()
    end
end

function MainWindow:updateCustomNodeEditors()
    local redrawLinks = false

    for node, nodeWidget in pairs(self.nodeWidgets) do
        local updated = nodeWidget:updateCustomNodeEditor()
        redrawLinks = redrawLinks or updated
    end

    if redrawLinks then
        self:drawLinks(true)
    end
end

return MainWindow