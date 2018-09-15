local PinTypes = flat.require 'graph/pintypes'

local Theme = flat.ui.settings.theme.graphEditor.node
local GridTheme = flat.ui.settings.theme.graphEditor.grid

local MAGNET_BARS_WIDTH = 8

local NodeWidget = {}
NodeWidget.__index = NodeWidget

function NodeWidget:new(node, mainWindow, foldedNodes)
    assert(foldedNodes)
    assert(node, 'no node given')
    local nodeType = mainWindow:getCurrentGraph().nodeType
    local nodePath = node.path
    local customNodeEditor
    pcall(function()
        customNodeEditor = node.require('graph-editor/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
    end)
    if not customNodeEditor then
        pcall(function()
            customNodeEditor = node.require('graph-editor/common/nodes/' .. nodePath .. 'node')
        end)
    end
    local o = setmetatable({
        node = node,
        mainWindow = mainWindow,
        inputPinPlugWidgets = {},
        inputPinSocketWidgets = {},
        outputPinPlugWidgets = {},
        outputPinSocketWidgets = {},
        pinsWidget = nil,
        customNodeEditor = customNodeEditor,
        inputPinNameWidgetContainers = {},
        foldedConstantEditorWidgets = {},
        nodeNameContainer = nil,
        closestInputPin = nil,
        closestOutputPin = nil,
    }, self)

    o:buildContainer()
    o:build(foldedNodes)

    return o
end

function NodeWidget:buildContainer()
    local container
    local visibleContainer
    do
        container = Widget.makeLineFlow()

        -- left magnet bar
        do
            local leftBar = Widget.makeExpand()
            leftBar:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
            leftBar:setSize(MAGNET_BARS_WIDTH, 1)
            leftBar:setBackgroundColor(0x00660000)
            container:addChild(leftBar)
            leftBar:mouseMove(function(bar, x, y)
                local closestInputPin = self:getClosestInputPin(bar, x, y)
                if closestInputPin then
                    self.mainWindow:snapTo(self.node, closestInputPin)
                else
                    self.mainWindow:clearSnap()
                end
            end)
            leftBar:mouseLeave(function()
                self.mainWindow:clearSnap()
            end)
            leftBar:mouseDown(function(bar, x, y)
                local closestInputPin = self:getClosestInputPin(bar, x, y)
                self:drawLinkFromInputPin(self.node, closestInputPin)
                return true
            end)
            leftBar:mouseUp(function()
                self.mainWindow:validSnap()
            end)
        end

        -- visible container
        do
            visibleContainer = Widget.makeColumnFlow()
            visibleContainer:setBackgroundColor(Theme.BACKGROUND_COLOR)
            visibleContainer:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
            visibleContainer:rightClick(function()
                self.mainWindow:openNodeContextualMenu()
                return true
            end)
            container:addChild(visibleContainer)
        end

        -- right magnet bar
        do
            local rightBar = Widget.makeExpand()
            rightBar:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
            rightBar:setSize(MAGNET_BARS_WIDTH, 1)
            rightBar:setBackgroundColor(0x0066F500)
            container:addChild(rightBar)
            rightBar:mouseMove(function(bar, x, y)
                local closestOutputPin = self:getClosestOutputPin(bar, x, y)
                if closestOutputPin then
                    self.mainWindow:snapTo(self.node, closestOutputPin)
                else
                    self.mainWindow:clearSnap()
                end
            end)
            rightBar:mouseDown(function(bar, x, y)
                local closestOutputPin = self:getClosestOutputPin(bar, x, y)
                self.mainWindow:beginDragWireFromOutputPin(self.node, closestOutputPin)
                return true
            end)
            rightBar:mouseLeave(function()
                self.mainWindow:clearSnap()
            end)
            rightBar:mouseUp(function()
                self.mainWindow:validSnap()
            end)
        end
    end

    self.container = container
    self.visibleContainer = visibleContainer
end

function NodeWidget:build(foldedNodes)
    assert(foldedNodes)

    local nodeWidget = self.visibleContainer
    local node = self.node

    -- node name
    do
        local nodeNameContainer = Widget.makeExpand()
        nodeNameContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        nodeNameContainer:setBackgroundColor(Theme.TITLE_BACKGROUND_COLOR)
        local mouseMoved = false
        local mouseDown = false
        local selectedNode = false
        nodeNameContainer:mouseDown(function()
            mouseMoved = false
            mouseDown = true
            selectedNode = false
            return true
        end)
        nodeNameContainer:mouseMove(function()
            if mouseDown and not mouseMoved then
                mouseMoved = true
                if not self.mainWindow:isNodeSelected(self) then
                    self.mainWindow:clearSelectedWidgets()
                    selectedNode = self.mainWindow:selectNode(self)
                end
                self.mainWindow:dragSelectedNodeWidgets()
            end
        end)
        nodeNameContainer:mouseUp(function()
            if mouseDown then
                mouseDown = false
                if mouseMoved then
                    self.mainWindow:dropSelectedNodeWidgets()
                    if selectedNode then
                        self.mainWindow:deselectNode(self)
                    end
                else
                    self.mainWindow:selectNode(self)
                end
            end
            return true
        end)

        do
            local nodeNameText = Widget.makeText(node:getName(), table.unpack(flat.ui.settings.defaultFont))
            nodeNameText:setTextColor(Theme.TITLE_TEXT_COLOR)
            nodeNameText:setPositionPolicy(Widget.PositionPolicy.CENTER)
            nodeNameText:setMargin(3)
            nodeNameContainer:addChild(nodeNameText)
        end

        nodeWidget:addChild(nodeNameContainer)

        self.nodeNameContainer = nodeNameContainer
    end

    -- all pins
    do
        local pinsWidget = Widget.makeLineFlow()

        -- input pins
        do
            local inputPinsWidget = Widget.makeColumnFlow()
            inputPinsWidget:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
            for i = 1, #node.inputPins do
                local pin = node.inputPins[i]
                local pinPlugged = pin.pluggedOutputPin ~= nil and not foldedNodes[pin.pluggedOutputPin.node]
                local inputPinWidget = self:makeInputPinWidget(node, pin, foldedNodes)
                self:setInputPinPlugged(pin, pinPlugged)
                inputPinsWidget:addChild(inputPinWidget)
            end
            pinsWidget:addChild(inputPinsWidget)
        end

        -- optional custom editor
        do
            local customEditor = false
            if self.customNodeEditor then
                customEditor = self.customNodeEditor.build(node, self, pinsWidget)
            end

            if not customEditor then
                local centerMargin = Widget.makeFixedSize(20, 1)
                pinsWidget:addChild(centerMargin)
            end
        end

        -- output pins
        do
            local outputPinsWidget = Widget.makeColumnFlow()
            outputPinsWidget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)
            for i = 1, #node.outputPins do
                local pin = node.outputPins[i]
                local outputPinWidget = self:makeOutputPinWidget(node, pin)
                self:setOutputPinPlugged(pin, #pin.pluggedInputPins > 0)
                outputPinsWidget:addChild(outputPinWidget)
            end
            pinsWidget:addChild(outputPinsWidget)
        end

        nodeWidget:addChild(pinsWidget)

        self.pinsWidget = pinsWidget
    end
end

function NodeWidget:rebuild(foldedNodes)
    assert(foldedNodes)
    self.visibleContainer:removeAllChildren()
    self:build(foldedNodes)
end

function NodeWidget:getVisiblePosition()
    local trueX, trueY = self.container:getPosition()
    return trueX + MAGNET_BARS_WIDTH, trueY
end

function NodeWidget:dragged(callback)
    self.container:dragged(callback)
end

function NodeWidget:setVisiblePosition(x, y)
    self.container:setPosition(x - MAGNET_BARS_WIDTH, y)
end

function NodeWidget:delete(position)
    self.container:removeFromParent()
end

function NodeWidget:drag()
    self.container:drag()
end

function NodeWidget:drop()
    self.container:drop()
end

function NodeWidget:getRelativePositionInWindow()
    return self.mainWindow:getContent():getRelativePosition(self.visibleContainer)
end

function NodeWidget:getVisibleComputedSize()
    return self.visibleContainer:getComputedSize()
end

function NodeWidget:getContainer()
    return self.container
end

function NodeWidget:drawLinkFromInputPin(node, pin)
    if pin.pluggedOutputPin then
        local outputPin = pin.pluggedOutputPin.outputPin
        local outputNode = pin.pluggedOutputPin.node
        local outputNodeWidget = self.mainWindow:getCurrentGraphNodeWidgets()[outputNode]
        if outputNodeWidget then
            local updateOutputNodeWidget, updateInputNodeWidget = node:unplugInputPin(pin)

            if updateOutputNodeWidget then
                outputNode:rebuild(self.mainWindow:getFoldedNodes())
            end

            if updateInputNodeWidget then
                self:rebuild(self.mainWindow:getFoldedNodes())
            else
                self:setInputPinPlugged(pin, false)
            end

            self.mainWindow:beginDragWireFromOutputPin(outputNode, outputPin)
            self:showFoldedConstantNode(pin)
        else
            self.mainWindow:beginDragWireFromInputPin(node, pin)
        end
    else
        self.mainWindow:beginDragWireFromInputPin(node, pin)
    end
end

function NodeWidget:makeInputPinWidget(node, pin, foldedNodes)
    local inputPinWidget = Widget.makeLineFlow()

    do
        local inputPinPlugWidget = Widget.makeFixedSize(8, 8)
        inputPinPlugWidget:setPositionPolicy(Widget.PositionPolicy.CENTER)
        inputPinPlugWidget:setMargin(0, 3, 0, 0)
        inputPinPlugWidget:setBackgroundColor(self:getPinColorByType(pin.pinType))

        local inputPinSocketWidget = Widget.makeFixedSize(6, 4)
        inputPinSocketWidget:setBackgroundColor(GridTheme.BACKGROUND_COLOR)
        inputPinSocketWidget:setMargin(2, 2, 2, 0)
        inputPinPlugWidget:addChild(inputPinSocketWidget)
        self.inputPinSocketWidgets[pin] = inputPinSocketWidget

        local mouseIsInside = false
        inputPinPlugWidget:mouseDown(function()
            self:drawLinkFromInputPin(node, pin)
            return true
        end)

        inputPinPlugWidget:mouseUp(function()
            self.mainWindow:validSnap()
        end)

        inputPinPlugWidget:mouseMove(function()
            if mouseIsInside then
                self.mainWindow:snapTo(node, pin)
            end
        end)
        inputPinPlugWidget:mouseLeave(function()
            mouseIsInside = false
            self.mainWindow:clearSnap()
        end)
        inputPinPlugWidget:mouseEnter(function()
            mouseIsInside = true
        end)

        inputPinWidget:addChild(inputPinPlugWidget)
        self.inputPinPlugWidgets[pin] = inputPinPlugWidget
    end

    do
        local inputPinNameWidgetContainer = Widget.makeColumnFlow()
        self.inputPinNameWidgetContainers[pin] = inputPinNameWidgetContainer

        do
            local inputPinNameWidget = Widget.makeText(pin.pinName, table.unpack(flat.ui.settings.defaultFont))
            inputPinNameWidget:setTextColor(Theme.PIN_NAME_TEXT_COLOR)
            inputPinNameWidget:setMargin(1, 0, 1, 0)
            inputPinNameWidgetContainer:addChild(inputPinNameWidget)
        end

        do
            if not pin.pluggedOutputPin or foldedNodes[pin.pluggedOutputPin.node] then
                -- custom constant node editor inside empty pin
                self:showFoldedConstantNode(pin)
            end
        end

        inputPinWidget:addChild(inputPinNameWidgetContainer)
    end

    return inputPinWidget
end

function NodeWidget:hideFoldedConstantNode(pin)
    local foldedConstantEditorWidget = assert(self.foldedConstantEditorWidgets[pin])
    foldedConstantEditorWidget:removeFromParent()
    self.foldedConstantEditorWidgets[pin] = nil
end

function NodeWidget:showFoldedConstantNode(pin)
    self.foldedConstantEditorWidgets[pin] = nil
    local graph = self.mainWindow:getCurrentGraph()
    local node = self.node
    local nodeType = graph.nodeType
    local nodeName = node:pinTypeToString(pin.pinType):lower()
    local customConstantNodeEditor
    pcall(function()
        customConstantNodeEditor = flat.require('graph-editor/' .. nodeType .. '/nodes/' .. nodeName .. 'node')
    end)
    if customConstantNodeEditor then
        local constantNode
        if pin.pluggedOutputPin then
            constantNode = pin.pluggedOutputPin.node
        else
            local nodeClasses = flat.graph.getNodeClasses(nodeType)
            constantNode = graph:addNode(nodeClasses[nodeName])
            constantNode:plugPins(constantNode:getOutputPin(1), node, pin)
        end
        local inputPinNameWidgetContainer = assert(self.inputPinNameWidgetContainers[pin])
        local customEditor, customWidget = customConstantNodeEditor.build(constantNode, nil, inputPinNameWidgetContainer)
        if customWidget then
            self.foldedConstantEditorWidgets[pin] = customWidget
        end
        return true
    end
    return false
end

function NodeWidget:makeOutputPinWidget(node, pin)
    local outputPinWidget = Widget.makeLineFlow()
    outputPinWidget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

    do
        local outputPinNameWidget = Widget.makeText(pin.pinName, table.unpack(flat.ui.settings.defaultFont))
        outputPinNameWidget:setTextColor(Theme.PIN_NAME_TEXT_COLOR)
        outputPinNameWidget:setMargin(1, 0, 1, 0)
        outputPinWidget:addChild(outputPinNameWidget)
    end

    do
        local outputPinPlugWidget = Widget.makeFixedSize(8, 8)
        outputPinPlugWidget:setPositionPolicy(Widget.PositionPolicy.CENTER)
        outputPinPlugWidget:setMargin(0, 0, 0, 3)
        outputPinPlugWidget:setBackgroundColor(self:getPinColorByType(pin.pinType))

        do
            local outputPinSocketWidget = Widget.makeFixedSize(6, 4)
            outputPinSocketWidget:setBackgroundColor(GridTheme.BACKGROUND_COLOR)
            outputPinSocketWidget:setMargin(2, 0, 2, 2)
            outputPinPlugWidget:addChild(outputPinSocketWidget)
            self.outputPinSocketWidgets[pin] = outputPinSocketWidget
        end

        local mouseIsInside = false

        outputPinPlugWidget:mouseEnter(function()
            mouseIsInside = true
        end)

        outputPinPlugWidget:mouseMove(function()
            if mouseIsInside then
                self.mainWindow:snapTo(node, pin)
            end
        end)
        outputPinPlugWidget:mouseLeave(function()
            mouseIsInside = false
            self.mainWindow:clearSnap()
        end)

        outputPinPlugWidget:mouseDown(function()
            self.mainWindow:beginDragWireFromOutputPin(node, pin)
            return true
        end)

        outputPinPlugWidget:mouseUp(function()
            return self.mainWindow:validSnap()
        end)

        outputPinWidget:addChild(outputPinPlugWidget)
        self.outputPinPlugWidgets[pin] = outputPinPlugWidget
    end

    return outputPinWidget
end

function NodeWidget:select()
    self.visibleContainer:setBackgroundColor(Theme.SELECTED_BACKGROUND_COLOR)
end

function NodeWidget:deselect()
    self.visibleContainer:setBackgroundColor(Theme.BACKGROUND_COLOR)
end

local pinColors = {
    [PinTypes.ANY]           = 0x888888FF,

    [PinTypes.IMPULSE]       = 0xE74C3CFF,

    [PinTypes.TO_STATE]      = 0x1ABC9CFF,
    [PinTypes.STATE_TO_RULE] = 0x9B59B6FF,

    [flat.types.BOOLEAN]     = 0x2ECC71FF,
    [flat.types.NUMBER]      = 0xF39C12FF,
    [flat.types.STRING]      = 0x3498DBFF,
    [flat.types.TABLE]       = 0x8E44ADFF,
    [flat.types.FUNCTION]    = 0x34495EFF,

    [flat.types['flat.Vector2']] = 0xF17A88FF,
    [flat.types['flat.Vector3']] = 0xF17A00FF,
    [flat.types['CG.Entity']]    = 0x006600FF, -- TODO: get this out of flat
}

function NodeWidget:getPinColorByType(pinType)
    local color = pinColors[pinType]
    assert(
        color,
        'no color for pin type ' .. self.node:pinTypeToString(pinType)
    )
    return color
end

function NodeWidget:getInputPinPlugWidget(pin)
    return self.inputPinPlugWidgets[pin]
end

function NodeWidget:getOutputPinPlugWidget(pin)
    return self.outputPinPlugWidgets[pin]
end

function NodeWidget:setInputPinPlugged(pin, plugged)
    local inputPinSocketWidget = self.inputPinSocketWidgets[pin]
    inputPinSocketWidget:setVisible(not plugged)
end

function NodeWidget:setOutputPinPlugged(pin, plugged)
    local outputPinSocketWidget = self.outputPinSocketWidgets[pin]
    outputPinSocketWidget:setVisible(not plugged)
end

function NodeWidget:updatePinSocketWidgets()
    self:updateInputPinSocketWidgets()
    self:updateOutputPinSocketWidgets()
end

function NodeWidget:updateInputPinSocketWidgets()
    local nodeWidgets = self.mainWindow:getCurrentGraphNodeWidgets()
    local inputPins = self.node.inputPins
    for i = 1, #inputPins do
        local inputPin = inputPins[i]
        local plugged = inputPin.pluggedOutputPin and nodeWidgets[inputPin.pluggedOutputPin.node]
        self:setInputPinPlugged(inputPin, plugged)
        if not plugged and not self.foldedConstantEditorWidgets[inputPin] then
            self:showFoldedConstantNode(inputPin)
        end
    end
end

function NodeWidget:updateOutputPinSocketWidgets()
    local outputPins = self.node.outputPins
    for i = 1, #outputPins do
        local outputPin = outputPins[i]
        local plugged = #outputPin.pluggedInputPins > 0
        self:setOutputPinPlugged(outputPin, plugged)
    end
end

function NodeWidget:updateCustomNodeEditor()
    if self.customNodeEditor and self.customNodeEditor.update then
        return self.customNodeEditor.update(self.node, self, self.pinsWidget)
    end
end

local function getClosestPin(bar, pinPlugWidgets, x, y)
    local closestPin = nil
    local closestDistance = math.huge
    local barWidth, barHeight = bar:getComputedSize()
    local barX, barY = bar:getPosition()
    for pin, plugWidget in pairs(pinPlugWidgets) do
        local _, socketY = bar:getRelativePosition(plugWidget)
        local _, plugHeight = plugWidget:getComputedSize()
        local distance = math.abs(socketY + plugHeight / 2 - y)
        local isInBar = x >= barX and x <= barX + barWidth
        if distance < closestDistance and distance < 50 and isInBar then
            closestDistance = distance
            closestPin = pin
        end
    end
    return closestPin
end

function NodeWidget:getClosestInputPin(leftMagnetBar, x, y)
    return getClosestPin(leftMagnetBar, self.inputPinPlugWidgets, x, y)
end

function NodeWidget:getClosestOutputPin(rightMagnetBar, x, y)
    return getClosestPin(rightMagnetBar, self.outputPinPlugWidgets, x, y)
end


return NodeWidget