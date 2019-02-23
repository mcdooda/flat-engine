local Graph = flat.require 'graph/graph'
local NodeWidget = flat.require 'graph-editor/nodewidget'
local PinTypes = flat.require 'graph/pintypes'

local MainWindow = {}
MainWindow.__index = MainWindow
setmetatable(MainWindow, { __index = flat.ui.Window })

function MainWindow:new(parent, metadata, onSave)
    local o = flat.ui.Window:new(parent)
    setmetatable(o, self)

    -- open graphs
    o.graphInfos = {}
    o.currentGraphInfo = nil

    -- window ui
    o.breadcrumb = nil

    -- graph ui
    o.selectionWidget = nil
    o.initialSelectionPosition = nil
    o.currentLink = nil

    o.nodeListMenu = nil

    o.nodeContextualMenu = nil

    o.metadata = metadata
    o.onSave = onSave
    o.isNew = false
    o.snap = nil
    o:build()
    return o
end

function MainWindow:build()
    self:setTitle 'Node Graph Editor'

    -- toolbar
    do
        local toolbar = self:addToolbar()

        do
            local saveButton = toolbar:addButton 'Save Graph'
            saveButton:click(function()
                self:saveGraphToFile()
                self:saveGraphLayoutToFile()
                self:saveLuaRunnerFile() -- TODO: only for components?
                self:updateCustomNodeEditors()
                if self.onSave then
                    self.onSave(self.isNew)
                    self.isNew = false
                end
                local graphInfo = self:getCurrentRootGraphInfo()
                flat.ui.success('Graph ' .. graphInfo.path .. ' saved')
            end)
        end

        do
            local useCompoundButton = toolbar:addButton 'Use Compound'
            useCompoundButton:click(flat.ui.task(function()
                local foldedNodes = self:getFoldedNodes()
                while true do
                    local compoundPath = flat.ui.prompt 'Compound Path:'
                    if compoundPath then
                        local content = self:getContent()
                        local graphInfo = self.currentGraphInfo
                        local graph = graphInfo.graph
                        local nodeClasses = flat.graph.getNodeClasses(graph.nodeType)
                        local compoundNode = graph:addNode(nodeClasses['compound'], false)
                        if compoundNode:load(compoundPath) then
                            compoundNode:buildPins()
                            local nodeIndex = graph:findNodeIndex(compoundNode)
                            graphInfo.layout[nodeIndex] = {0, 0}
                            local nodeWidget = self:makeNodeWidget(compoundNode, foldedNodes)
                            local contentSizeX, contentSizeY = content:getComputedSize()
                            local scrollX, scrollY = content:getScrollPosition()
                            local nodeWidgetX = scrollX + contentSizeX / 2
                            local nodeWidgetY = scrollY - contentSizeY / 2 -- move the relative position from bottom left to top left
                            nodeWidget.container:setPosition(nodeWidgetX, nodeWidgetY)
                            content:addChild(nodeWidget.container)
                            break
                        else
                            graph:removeNode(compoundNode)
                            flat.ui.error('Cound not load compound ' .. compoundPath)
                        end
                    else
                        break
                    end
                end
            end))
        end
    end

    -- open graphs breadcrumb
    do
        local breadcrumbToolbar = self:addToolbar()
        local breadcrumb = flat.ui.Breadcrumb:new(breadcrumbToolbar.toolbar)
        -- no item for now, they are added when a graph is opened
        self.breadcrumb = breadcrumb
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

function MainWindow:openGraphFromFile(graphPath, nodeType)
    local graphInfos = self.graphInfos

    -- if the graph is already open right below, only change the breadcrumb focus
    if self.currentGraphInfo then
        local subGraphInfo = graphInfos[self.currentGraphInfo.index + 1]
        if subGraphInfo and subGraphInfo.path == graphPath then
            self:setCurrentGraphInfo(subGraphInfo)
            self.breadcrumb:setCurrentItem(subGraphInfo.index)
            return
        end

        -- remove sub graphs
        for i = #graphInfos, self.currentGraphInfo.index + 1, -1 do
            graphInfos[i] = nil
            self.breadcrumb:removeItem(i)
        end
    end

    local graph = self:loadGraphFromFile(graphPath)

    local graphInfo = {
        graph = graph,
        path = graphPath,
        index = #graphInfos + 1,
        nodeWidgets = {},
        selectedNodeWidgets = {},
        parentGraphInfo = nil,
        subGraphId = nil
    }
    graphInfos[graphInfo.index] = graphInfo
    self:setCurrentGraphInfo(graphInfo)

    local item = self.breadcrumb:addItem(self:getBreadcrumbName(graphInfo))
    item:click(function()
        self:setCurrentGraphInfo(graphInfo)
    end)
    self.breadcrumb:setCurrentItem(graphInfo.index)

    if graph.nodeType then
        -- the graph has been loaded
        assert(
            not nodeType or graph.nodeType == nodeType,
            'unexpected graph type: ' .. tostring(nodeType) .. ' expected, got ' .. tostring(graph.nodeType)
        )

        local graphLayout = self:getGraphLayout(graphInfo)
        local content = self:getContent()

        local foldedNodes = {}
        for i = 1, #graph.nodeInstances do
            local node = graph.nodeInstances[i]
            local nodePosition = graphLayout[i]
            if not nodePosition then
                --assert(node:isConstant(), 'Cannot fold a non constant node: ' .. node:getName())
                if node:isConstant() then
                    foldedNodes[node] = true
                else
                    flat.ui.error('Cannot fold a non constant node: ' .. node:getName())
                    graphLayout[i] = {0, 0}
                end
            end
        end

        for i = 1, #graph.nodeInstances do
            local node = graph.nodeInstances[i]
            local nodePosition = graphLayout[i]
            if nodePosition then
                local nodeWidget = self:makeNodeWidget(node, foldedNodes)
                nodeWidget:setVisiblePosition(table.unpack(nodePosition))
                content:addChild(nodeWidget:getContainer())
            else
                assert(node:isConstant(), 'No position for non constant node: ' .. node:getName())
            end
        end

        self:updateCustomNodeEditors()
        content:redraw()

        assert(self:layoutSanityCheck())
        return true
    else
        -- the graph has not been loaded
        graph.nodeType = nodeType
        graphInfo.layout = {}
        self.isNew = true

        assert(self:layoutSanityCheck())
        return false
    end
end

function MainWindow:openSubGraph(graph, subGraphId, parentGraphInfo)
    assert(graph)
    assert(subGraphId)
    assert(parentGraphInfo)
    local graphInfos = self.graphInfos

    -- if the graph is already open right below, only change the breadcrumb focus
    if self.currentGraphInfo then
        local subGraphInfo = graphInfos[self.currentGraphInfo.index + 1]
        if subGraphInfo and subGraphInfo.graph == graph then
            self:setCurrentGraphInfo(subGraphInfo)
            self.breadcrumb:setCurrentItem(subGraphInfo.index)
            return
        end

        -- remove sub graphs
        for i = #graphInfos, self.currentGraphInfo.index + 1, -1 do
            graphInfos[i] = nil
            self.breadcrumb:removeItem(i)
        end
    end

    local graphInfo = {
        graph = graph,
        path = nil,
        index = #graphInfos + 1,
        nodeWidgets = {},
        selectedNodeWidgets = {},
        parentGraphInfo = parentGraphInfo,
        subGraphId = subGraphId
    }
    graphInfos[graphInfo.index] = graphInfo
    self:setCurrentGraphInfo(graphInfo)

    local item = self.breadcrumb:addItem(self:getBreadcrumbName(graphInfo))
    item:click(function()
        self:setCurrentGraphInfo(graphInfo)
    end)
    self.breadcrumb:setCurrentItem(graphInfo.index)

    if graph.nodeType then
        -- the graph has been loaded
        assert(graph.nodeType, 'unknown graph type')

        local graphLayout = self:getGraphLayout(graphInfo)
        local content = self:getContent()

        local foldedNodes = {}
        for i = 1, #graph.nodeInstances do
            local node = graph.nodeInstances[i]
            local nodePosition = graphLayout[i]
            if not nodePosition then
                --assert(node:isConstant(), 'Cannot fold a non constant node: ' .. node:getName())
                if node:isConstant() then
                    foldedNodes[node] = true
                else
                    flat.ui.error('Cannot fold a non constant node: ' .. node:getName())
                    graphLayout[i] = {0, 0}
                end
            end
        end

        local nodes = graph:getNodes()
        for index, position in pairs(graphLayout) do
            if type(index) == 'number' then
                if not nodes[index] then
                    flat.ui.error('Layout has field ' .. index .. ' but no corresponding node')
                    graphLayout[index] = nil
                end
            end
        end

        for i = 1, #graph.nodeInstances do
            local node = graph.nodeInstances[i]
            local nodePosition = graphLayout[i]
            if nodePosition then
                local nodeWidget = self:makeNodeWidget(node, foldedNodes)
                nodeWidget:setVisiblePosition(table.unpack(nodePosition))
                content:addChild(nodeWidget:getContainer())
            else
                assert(node:isConstant(), 'No position for non constant node: ' .. node:getName())
            end
        end

        self:updateCustomNodeEditors()

        assert(self:layoutSanityCheck())
        content:redraw()

        assert(self:layoutSanityCheck())
        return true
    else
        -- the graph has not been loaded
        graph.nodeType = nodeType
        graphInfo.layout = {}
        if not parentGraphInfo.layout.subGraphLayouts then
            parentGraphInfo.layout.subGraphLayouts = {}
        end
        parentGraphInfo.layout.subGraphLayouts[subGraphId] = graphInfo.layout
        self.isNew = true

        assert(self:layoutSanityCheck())
        return false
    end
end

function MainWindow:closeSubGraphIfOpen(subGraphId)
    local graphInfos = self.graphInfos
    local subGraphInfoIndex
    for i = 1, #graphInfos do
        local graphInfo = graphInfos[i]
        if graphInfo.subGraphId == subGraphId then
            subGraphInfoIndex = i
            break
        end
    end

    if subGraphInfoIndex then
        for i = #graphInfos, subGraphInfoIndex, -1 do
            graphInfos[i] = nil
            self.breadcrumb:removeItem(i)
        end
    end
end

function MainWindow:loadGraphFromFile(graphPath)
    assert(graphPath)
    local graph = Graph:new()
    pcall(function()
        -- if the file does not exist, we want to create a new graph
        graph:loadGraphFromFile(graphPath .. '.graph.lua')
    end)
    return graph
end

function MainWindow:saveGraphToFile()
    assert(self:layoutSanityCheck())

    local graphInfo = self:getCurrentRootGraphInfo()
    graphInfo.graph:saveGraph(graphInfo.path .. '.graph.lua')
end

function MainWindow:loadGraphLayoutFromFile(graphPath)
    assert(graphPath)
    local graphLayout
    if not pcall(function()
        graphLayout = dofile(graphPath .. '.layout.lua')
    end) then
        graphLayout = {}
    end
    return graphLayout
end

function MainWindow:saveGraphLayoutToFile()
    assert(self:layoutSanityCheck())

    local graphInfo = self:getCurrentRootGraphInfo()
    local layout = graphInfo.layout

    -- layout sanity check
    if flat.debug then
        for i = 1, #graphInfo.graph.nodeInstances do
            if not layout[i] then
                local node = graphInfo.graph.nodeInstances[i]
                assert(node:isConstant(), 'Cannot fold a non constant node: ' .. node:getName())
            end
        end
    end

    local f = io.open(graphInfo.path .. '.layout.lua', 'w')
    f:write 'return '
    flat.dumpToOutput(f, layout)
    f:close()
end

function MainWindow:saveLuaRunnerFile()
    local graphPath = self.currentGraphInfo.path
    if graphPath then
        local componentFilePath = graphPath .. '.lua'
        if not io.open(componentFilePath, 'r') then
            -- TODO: should depend on the node type
            local runnerCode = ([[return flat.graph.script.run '%s']]):format(graphPath)

            local f = io.open(componentFilePath, 'w')
            assert(runnerCode)
            f:write(runnerCode)
            f:close()
        end
    end
end

function MainWindow:makeNodeWidget(node, foldedNodes)
    assert(node)
    assert(foldedNodes)
    local graphInfo = self.currentGraphInfo
    local nodeWidget = NodeWidget:new(node, self, foldedNodes)
    local nodeIndex = assert(graphInfo.graph:findNodeIndex(node))
    local nodeLayout = assert(graphInfo.layout[nodeIndex])
    nodeWidget:dragged(function()
        self:drawLinks()
        local x, y = nodeWidget:getVisiblePosition()
        nodeLayout[1] = x
        nodeLayout[2] = y
    end)
    graphInfo.nodeWidgets[node] = nodeWidget
    return nodeWidget
end

function MainWindow:drawLinks(delayToNextFrame)
    local function draw()
        local content = self:getContent()

        content:clear(0xECF0F1FF)

        -- draw grid
        do
            local scrollX, scrollY = content:getScrollPosition()
            local w, h = content:getComputedSize()

            local gridTheme = flat.ui.settings.theme.graphEditor.grid

            do
                local firstSmallX = -scrollX % gridTheme.SMALL_STEP
                local firstBigX = -scrollX % gridTheme.BIG_STEP
                for x = firstSmallX, w, gridTheme.SMALL_STEP do
                    if (x - firstBigX) % gridTheme.BIG_STEP > 0 then
                        content:drawLine(
                            gridTheme.SMALL_LINE_COLOR,
                            gridTheme.SMALL_LINE_WIDTH,
                            false,
                            flat.Vector2(x, 0),
                            flat.Vector2(x, h)
                        )
                    end
                end
                for x = firstBigX, w, gridTheme.BIG_STEP do
                    content:drawLine(
                        gridTheme.BIG_LINE_COLOR,
                        gridTheme.BIG_LINE_WIDTH,
                        false,
                        flat.Vector2(x, 0),
                        flat.Vector2(x, h)
                    )
                end
            end

            do
                local firstSmallY = (scrollY % gridTheme.SMALL_STEP) - gridTheme.SMALL_STEP
                local firstBigY = (scrollY % gridTheme.BIG_STEP) - gridTheme.BIG_STEP
                for y = firstSmallY, h, gridTheme.SMALL_STEP do
                    if (y - firstBigY) % gridTheme.BIG_STEP > 0 then
                        content:drawLine(
                            gridTheme.SMALL_LINE_COLOR,
                            gridTheme.SMALL_LINE_WIDTH,
                            false,
                            flat.Vector2(0, h - y),
                            flat.Vector2(w, h - y)
                        )
                    end
                end
                for y = firstBigY, h, gridTheme.BIG_STEP do
                    content:drawLine(
                        gridTheme.BIG_LINE_COLOR,
                        gridTheme.BIG_LINE_WIDTH,
                        false,
                        flat.Vector2(0, h - y),
                        flat.Vector2(w, h - y)
                    )
                end
            end
        end
        local currentLink = self.currentLink

        -- draw plugged links
        local graphInfo = self.currentGraphInfo
        local graph = graphInfo.graph
        local nodeWidgets = graphInfo.nodeWidgets
        for i = 1, #graph.nodeInstances do
            local inputNode = graph.nodeInstances[i]
            for j = 1, #inputNode.inputPins do
                local inputPin = inputNode.inputPins[j]
                if inputPin.pluggedOutputPin then
                    local outputNode = inputPin.pluggedOutputPin.node
                    if nodeWidgets[outputNode] then
                        local outputPin = inputPin.pluggedOutputPin.outputPin

                        local inputPinX, inputPinY = self:getInputPinPosition(inputNode, inputPin)
                        local outputPinX, outputPinY = self:getOutputPinPosition(outputNode, outputPin)

                        local linkColor = self:getPinColor(inputNode, inputPin)
                        if currentLink and currentLink.snapped and self.snapPin == inputPin then
                            linkColor = (linkColor & 0xFFFFFF00) | (math.floor((linkColor & 0x000000FF) / 2))
                        end

                        self:drawLink(
                            linkColor,
                            inputPinX, inputPinY,
                            outputPinX, outputPinY
                        )
                    end
                end
            end
        end

        -- draw currently dragged link
        if currentLink then
            local linkColor = currentLink.color
            if currentLink.snapped and ((currentLink.inputPin and currentLink.inputPin.pinType == PinTypes.ANY) or (currentLink.outputPin and currentLink.outputPin.pinType == PinTypes.ANY)) then
                if self.snap.inputNode then
                    linkColor = self:getPinColor(self.snap.inputNode, self.snap.inputPin)
                else
                    linkColor = self:getPinColor(self.snap.outputNode, self.snap.outputPin)
                end
            end
            self:drawLink(
                linkColor,
                currentLink.inputPinX, currentLink.inputPinY,
                currentLink.outputPinX, currentLink.outputPinY
            )
        end

    end

    if delayToNextFrame then
        local timer = flat.Timer()
        timer:onEnd(draw)
        timer:start(0.01)
    else
        draw()
    end
end

function MainWindow:updateCurrentLink(x, y)
    local currentLink = self.currentLink
    assert(currentLink, 'no current link')
    currentLink.snapped = false
    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    if self.snap then
        if currentLink.inputNode and self:canPlugPins(self.snap.outputNode, self.snap.outputPin, currentLink.inputNode, currentLink.inputPin) then
            currentLink.outputPinX, currentLink.outputPinY = self:getOutputPinPosition(self.snap.outputNode, self.snap.outputPin)
            currentLink.snapped = true
            nodeWidgets[self.snap.outputNode]:setOutputPinPlugged(self.snap.outputPin, true)
        elseif currentLink.outputNode and self:canPlugPins(currentLink.outputNode, currentLink.outputPin, self.snap.inputNode, self.snap.inputPin) then
            currentLink.inputPinX, currentLink.inputPinY = self:getInputPinPosition(self.snap.inputNode, self.snap.inputPin)
            currentLink.snapped = true
            nodeWidgets[self.snap.inputNode]:setInputPinPlugged(self.snap.inputPin, true)
        end
    end
    if not currentLink.snapped then
        if currentLink.inputNode then
            currentLink.outputPinX = x
            currentLink.outputPinY = y
            currentLink.inputPinX, currentLink.inputPinY = self:getInputPinPosition(currentLink.inputNode, currentLink.inputPin)
        else
            assert(currentLink.outputNode)
            currentLink.inputPinX = x
            currentLink.inputPinY = y
            currentLink.outputPinX, currentLink.outputPinY = self:getOutputPinPosition(currentLink.outputNode, currentLink.outputPin)
        end
    end
    self:drawLinks()
end

function MainWindow:clearCurrentLink()
    local currentLink = self.currentLink
    assert(currentLink, 'no current link')
    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    if currentLink.inputNode then
        local nodeWidget = nodeWidgets[currentLink.inputNode]
        nodeWidget:setInputPinPlugged(currentLink.inputPin, false)
    else
        assert(currentLink.outputNode)
        local nodeWidget = nodeWidgets[currentLink.outputNode]
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
    self:getContent():drawBezier(linkColor, 2, true, bezier)
end

function MainWindow:getPinColor(inputNode, inputPin)
    local nodeWidget = self.currentGraphInfo.nodeWidgets[inputNode]
    return nodeWidget:getPinColorByType(inputPin.pinType)
end

function MainWindow:getInputPinPosition(inputNode, inputPin)
    local nodeWidget = assert(self.currentGraphInfo.nodeWidgets[inputNode], 'No widget for node ' .. inputNode:getName() .. ' (@=' .. tostring(inputNode) .. ') graphInfo=' .. tostring(self.currentGraphInfo) .. ')')
    local inputPinPlugWidget = assert(nodeWidget:getInputPinPlugWidget(inputPin))
    local x, y = self:getContent():getRelativePosition(inputPinPlugWidget)
    local sx, sy = inputPinPlugWidget:getSize()
    return x, y + sy / 2
end

function MainWindow:getOutputPinPosition(outputNode, outputPin)
    local nodeWidget = self.currentGraphInfo.nodeWidgets[outputNode]
    local outputPinPlugWidget = nodeWidget:getOutputPinPlugWidget(outputPin)
    assert(outputPinPlugWidget)
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
    local nodeWidget = self.currentGraphInfo.nodeWidgets[inputNode]
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
    local nodeWidget = self.currentGraphInfo.nodeWidgets[outputNode]
    nodeWidget:setOutputPinPlugged(outputPin, true)
    self:drawLinks()
end

function MainWindow:canPlugPins(outputNode, outputPin, inputNode, inputPin)
    local nodeType = self:getCurrentGraph().nodeType
    if nodeType == 'script' then
        if outputNode == inputNode then
            return false
        end
    end
    if inputPin.pinType == PinTypes.ANY then
        return outputPin.pinType ~= PinTypes.ANY
    elseif outputPin.pinType == PinTypes.ANY then
        return inputPin.pinType ~= PinTypes.ANY
    end
    return inputPin.pinType == outputPin.pinType
end

function MainWindow:linkReleasedOnInputPin(inputNode, inputPin)
    local currentLink = self.currentLink
    if currentLink and currentLink.outputNode then
        local nodeWidgets = self.currentGraphInfo.nodeWidgets
        local outputPin = currentLink.outputPin
        if self:canPlugPins(currentLink.outputNode, currentLink.outputPin, inputNode, inputPin) then
            local updateInputNodeWidget = false
            local unpluggedOutputPin = false
            if inputPin.pluggedOutputPin then
                unpluggedOutputPin = true
                local oldOutputPin = inputPin.pluggedOutputPin.outputPin
                local previousOutputNode = inputPin.pluggedOutputPin.node
                local updateOutputNodeWidget, updateInputNodeWidgetUnplug = inputNode:unplugInputPin(inputPin, true)

                do
                    local outputNodeWidget = nodeWidgets[previousOutputNode]
                    if outputNodeWidget then
                        if updateOutputNodeWidget then
                            outputNodeWidget:rebuild(self:getFoldedNodes())
                        else
                            if oldOutputPin ~= outputPin then
                                outputNodeWidget:setOutputPinPlugged(oldOutputPin, #oldOutputPin.pluggedInputPins > 0)
                            end
                        end
                        outputNodeWidget:updateCustomNodeEditor()
                    else
                        local graph = self:getCurrentGraph()
                        graph:removeNode(previousOutputNode)
                        local inputNodeWidget = assert(nodeWidgets[inputNode])
                        inputNodeWidget:hideFoldedConstantNode(inputPin)
                        self:drawLinks(true)
                    end
                end

                do
                    updateInputNodeWidget = updateInputNodeWidget or updateInputNodeWidgetUnplug
                end
            end

            local updateOutputNodeWidget, updateInputNodeWidgetPlug = currentLink.outputNode:plugPins(currentLink.outputPin, inputNode, inputPin, unpluggedOutputPin)
            self.currentLink = nil
            self:drawLinks()

            do
                local outputNodeWidget = nodeWidgets[currentLink.outputNode]
                if updateOutputNodeWidget then
                    outputNodeWidget:rebuild(self:getFoldedNodes())
                end
                outputNodeWidget:updateCustomNodeEditor()
            end

            do
                updateInputNodeWidget = updateInputNodeWidget or updateInputNodeWidgetPlug
                local inputNodeWidget = nodeWidgets[inputNode]
                if updateInputNodeWidget then
                    inputNodeWidget:rebuild(self:getFoldedNodes())
                else
                    inputNodeWidget:setInputPinPlugged(inputPin, true)
                end
                inputNodeWidget:updateCustomNodeEditor()
            end
        else
            local nodeWidget = nodeWidgets[currentLink.outputNode]
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
        local nodeWidgets = self.currentGraphInfo.nodeWidgets
        if self:canPlugPins(outputNode, outputPin, currentLink.inputNode, currentLink.inputPin) then
            local inputNodeWidget = assert(nodeWidgets[currentLink.inputNode])

            -- unplug the current folded constant node
            if currentLink.inputPin.pluggedOutputPin then
                local constantNode = currentLink.inputPin.pluggedOutputPin.node
                assert(constantNode and not nodeWidgets[constantNode])
                currentLink.inputNode:unplugInputPin(currentLink.inputPin, true)
                local graph = self:getCurrentGraph()
                graph:removeNode(constantNode)
                inputNodeWidget:hideFoldedConstantNode(currentLink.inputPin)
                self:drawLinks(true)
            end

            local updateOutputNodeWidget, updateInputNodeWidget = outputNode:plugPins(outputPin, currentLink.inputNode, currentLink.inputPin)
            self.currentLink = nil
            self:drawLinks()

            do
                local outputNodeWidget = assert(nodeWidgets[outputNode])
                if updateOutputNodeWidget then
                    outputNodeWidget:rebuild(self:getFoldedNodes())
                else
                    outputNodeWidget:setOutputPinPlugged(outputPin, true)
                end
                outputNodeWidget:updateCustomNodeEditor()
            end

            do
                if updateInputNodeWidget then
                    inputNodeWidget:rebuild(self:getFoldedNodes())
                end
                inputNodeWidget:updateCustomNodeEditor()
            end
        else
            local nodeWidget = nodeWidgets[currentLink.inputNode]
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

    local graph = self:getCurrentGraph()
    local nodeClasses = flat.graph.getNodeClasses(graph.nodeType)
    local nodesContainer
    local textInputWidget

    local searchNodes = {}
    local function updateNodeSearch()
        searchNodes = {}
        local search = textInputWidget:getText():lower()
        for nodeName, nodeClass in pairs(nodeClasses) do
            if nodeClass:isBrowsable() then
                local nodeVisualName = nodeClass:getName()
                if #search == 0 or nodeVisualName:lower():match(search) then
                    flat.arrayAdd(searchNodes, {
                        visualName = nodeVisualName,
                        name = nodeName
                    })
                end
            end
        end
        table.sort(searchNodes, function(a, b) return a.visualName < b.visualName end)
    end

    local function insertNode(nodeName)
        local foldedNodes = self:getFoldedNodes() -- this must be called before adding the new node to the graph!
        local content = self:getContent()
        local graphInfo = self.currentGraphInfo
        local graph = graphInfo.graph
        local node = graph:addNode(nodeClasses[nodeName])
        local nodeIndex = graph:findNodeIndex(node)
        local contentSizeX, contentSizeY = content:getComputedSize()
        local scrollX, scrollY = content:getScrollPosition()
        local nodeWidgetX = x + scrollX
        local nodeWidgetY = y + scrollY - contentSizeY -- move the relative position from bottom left to top left
        graphInfo.layout[nodeIndex] = {nodeWidgetX, nodeWidgetY}
        local nodeWidget = self:makeNodeWidget(node, foldedNodes)
        nodeWidget:setVisiblePosition(nodeWidgetX, nodeWidgetY)
        content:addChild(nodeWidget:getContainer())
        self:closeNodeListMenu()
        assert(self:layoutSanityCheck())
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
    return self.currentGraphInfo.selectedNodeWidgets[nodeWidget]
end

function MainWindow:selectNode(nodeWidget)
    local selectedNodeWidgets = self.currentGraphInfo.selectedNodeWidgets
    if not selectedNodeWidgets[nodeWidget] then
        selectedNodeWidgets[nodeWidget] = true
        nodeWidget:select()
        return true
    end
    return false
end

function MainWindow:deselectNode(nodeWidget)
    local selectedNodeWidgets = self.currentGraphInfo.selectedNodeWidgets
    if selectedNodeWidgets[nodeWidget] then
        selectedNodeWidgets[nodeWidget] = nil
        nodeWidget:deselect()
        return true
    end
    return false
end

function MainWindow:clearSelectedWidgets()
    local selectedNodeWidgets = self.currentGraphInfo.selectedNodeWidgets
    for selectedNodeWidget in pairs(selectedNodeWidgets) do
        selectedNodeWidget:deselect()
    end
    self.currentGraphInfo.selectedNodeWidgets = {}
end

function MainWindow:dragSelectedNodeWidgets()
    local selectedNodeWidgets = self.currentGraphInfo.selectedNodeWidgets
    for selectedNodeWidget in pairs(selectedNodeWidgets) do
        selectedNodeWidget:drag()
    end
end

function MainWindow:dropSelectedNodeWidgets()
    local selectedNodeWidgets = self.currentGraphInfo.selectedNodeWidgets
    for selectedNodeWidget in pairs(selectedNodeWidgets) do
        selectedNodeWidget:drop()
    end
end

function MainWindow:deleteSelectedNodes()
    assert(self:layoutSanityCheck())
    local graphInfo = self.currentGraphInfo
    local graph = graphInfo.graph
    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    local selectedNodeWidgets = graphInfo.selectedNodeWidgets
    local layout = graphInfo.layout
    for selectedNodeWidget in pairs(selectedNodeWidgets) do
        local node = selectedNodeWidget.node
        local nodeIndex = graph:findNodeIndex(node)
        flat.arrayRemoveIndexCyclic(layout, nodeIndex)
        if node.subGraphId then
            layout.subGraphLayouts[node.subGraphId] = nil
            assert(node.subGraphId ~= graphInfo.subGraphId)
            self:closeSubGraphIfOpen(node.subGraphId)
        end
        graph:removeNode(node)
        nodeWidgets[node] = nil
        selectedNodeWidget:delete()
    end
    assert(self:layoutSanityCheck())
    graphInfo.selectedNodeWidgets = {}
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
    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    for node, nodeWidget in pairs(nodeWidgets) do
        local nodeWidgetX, nodeWidgetY = nodeWidget:getRelativePositionInWindow()
        local nodeWidgetWidth, nodeWidgetHeight = nodeWidget:getVisibleComputedSize()
        local centerX, centerY = nodeWidgetX + nodeWidgetWidth / 2, nodeWidgetY + nodeWidgetHeight / 2
        if selectionX <= centerX and centerX <= selectionRight and selectionY <= centerY and centerY <= selectionTop then
            self:selectNode(nodeWidget)
        end
    end
    self:removeSelectionWidget()
end

function MainWindow:updateAllNodesPinSocketWidgets()
    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    for node, nodeWidget in pairs(nodeWidgets) do
        nodeWidget:updatePinSocketWidgets()
    end
end

function MainWindow:updateCustomNodeEditors()
    local redrawLinks = false

    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    for node, nodeWidget in pairs(nodeWidgets) do
        local updated = nodeWidget:updateCustomNodeEditor()
        redrawLinks = redrawLinks or updated
    end

    if redrawLinks then
        self:drawLinks(true)
    end
end

function MainWindow:getFoldedNodes()
    local foldedNodes = {}
    local graphInfo = self.currentGraphInfo
    local graph = graphInfo.graph
    local nodes = graph:getNodes()
    local nodeWidgets = graphInfo.nodeWidgets
    for i = 1, #nodes do
        local node = nodes[i]
        if not nodeWidgets[node] then
            foldedNodes[node] = true
            assert(node:isConstant(), 'Only constant nodes can be folded')
        end
    end
    return foldedNodes
end

function MainWindow:getCurrentGraphInfo()
    return self.currentGraphInfo
end

function MainWindow:getCurrentGraph()
    return self.currentGraphInfo.graph
end

function MainWindow:getCurrentGraphNodeWidgets()
    return self.currentGraphInfo.nodeWidgets
end

function MainWindow:setCurrentGraphInfo(graphInfo)
    if graphInfo ~= self.currentGraphInfo then
        local content = self:getContent()
        content:removeAllChildren()
        self.currentGraphInfo = graphInfo
        local nodeWidgets = graphInfo.nodeWidgets
        for node, nodeWidget in pairs(nodeWidgets) do
            content:addChild(nodeWidget.container)
        end
        self:drawLinks()
    end
end

function MainWindow:getCurrentRootGraphInfo()
    local graphInfo = self.currentGraphInfo
    while graphInfo.parentGraphInfo do
        graphInfo = graphInfo.parentGraphInfo
    end
    assert(graphInfo)
    return graphInfo
end

function MainWindow:getGraphLayout(graphInfo)
    assert(graphInfo.path or graphInfo.parentGraphInfo)
    if graphInfo.layout then
        return graphInfo.layout
    end
    if graphInfo.path then
        graphInfo.layout = self:loadGraphLayoutFromFile(graphInfo.path)
        return graphInfo.layout
    else
        local parentLayout = self:getGraphLayout(graphInfo.parentGraphInfo)
        if not parentLayout.subGraphLayouts then
            parentLayout.subGraphLayouts = {}
        end
        local layout = parentLayout.subGraphLayouts[graphInfo.subGraphId]
        if not layout then
            layout = {}
            parentLayout.subGraphLayouts[graphInfo.subGraphId] = layout
        end
        graphInfo.layout = layout
        return layout
    end
end

function MainWindow:getBreadcrumbName(graphInfo)
    if graphInfo.path then
        return graphInfo.index .. '. ' .. string.gsub(graphInfo.path, '.+/([^/]-)$', '%1')
    else
        return graphInfo.index .. '. (' .. graphInfo.graph.nodeType .. ')'
    end
end

function MainWindow:snapToInputPin(node, pin)
    assert(node and pin)
    if self.currentLink and not pin.pluggedInputPins and self.currentLink.outputNode then
        if self.snap then
            self:clearSnapPinWidget()
        end
        self.snap = {inputPin = pin, inputNode = node}
    end
end

function MainWindow:snapToOutputPin(node, pin)
    assert(node and pin)
    if self.currentLink and pin.pluggedInputPins and self.currentLink.inputNode then
        if self.snap then
            self:clearSnapPinWidget()
        end
        self.snap = {outputPin = pin, outputNode = node}
    end
end

function MainWindow:clearSnapPinWidget()
    assert(self.snap)
    local nodeWidgets = self.currentGraphInfo.nodeWidgets
    -- pluggedInputPins only exists in outputPins
    if self.snap.outputPin then
        nodeWidgets[self.snap.outputNode]:setOutputPinPlugged(self.snap.outputPin, #self.snap.outputPin.pluggedInputPins > 0)
    else
        local foldedNodes = self:getFoldedNodes()
        nodeWidgets[self.snap.inputNode]:setInputPinPlugged(self.snap.inputPin, self.snap.inputPin.pluggedOutputPin and foldedNodes[self.snap.inputPin.pluggedOutputPin.node] == nil)
    end
end

function MainWindow:clearSnap()
    if self.snap and self.currentLink then
        self:clearSnapPinWidget()
    end

    self.snap = nil
end

function MainWindow:validSnap()
    if self.snap then
        if self.snap.outputPin then
            self:linkReleasedOnOutputPin(self.snap.outputNode, self.snap.outputPin)
        else
            self:linkReleasedOnInputPin(self.snap.inputNode, self.snap.inputPin)
        end
    end
end

function MainWindow:layoutSanityCheck()
    local graphInfos = self.graphInfos
    for i = 1, #graphInfos do
        local graphInfo = graphInfos[i]
        local nodeWidgets = graphInfo.nodeWidgets
        local layout = self:getGraphLayout(graphInfo)
        local graph = graphInfo.graph
        local nodes = graph:getNodes()
        for j = 1, #nodes do
            local node = nodes[j]
            if not nodeWidgets[node] then
                assert(not layout[j], 'Layout exists for node ' .. node:getName() .. ' but there is no node widget')
                assert(node:isConstant(), 'No node widget for node ' .. node:getName() .. ' which is not constant')
            else
                assert(layout[j], 'Node ' .. node:getName() .. ' has a node widget but no layout')
            end
        end
    end
    return true
end

return MainWindow