local PinTypes = flat.require 'graph/pintypes'

local NodeWidget = {}
NodeWidget.__index = NodeWidget

function NodeWidget:new(node, mainWindow)
    assert(node, 'no node given')
    local nodeType = mainWindow.graph.nodeType
    local nodePath = node.path
    local customNodeEditor
    pcall(function()
        customNodeEditor = node.require('graph-editor/' .. nodeType .. '/nodes/' .. nodePath .. 'node')
    end)
    local o = setmetatable({
        node = node,
        mainWindow = mainWindow,
        inputPinPlugWidgets = {},
        inputPinSocketWidgets = {},
        outputPinPlugWidgets = {},
        outputPinSocketWidgets = {},
        pinsWidget = nil,
        customNodeEditor = customNodeEditor
    }, self)

    local nodeWidget = Widget.makeColumnFlow()
    nodeWidget:setBackgroundColor(0xBDC3C7FF)
    nodeWidget:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
    nodeWidget:rightClick(function()
        mainWindow:openNodeContextualMenu()
        return true
    end)
    o.container = nodeWidget
    o:build()
    return o
end

function NodeWidget:build()
    local nodeWidget = self.container
    local node = self.node

    do
        local nodeNameContainer = Widget.makeExpand()
        nodeNameContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        nodeNameContainer:setBackgroundColor(0x2C3E50FF)
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
                    self.mainWindow:clearSelection()
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
            nodeNameText:setTextColor(0xECF0F1FF)
            nodeNameText:setPositionPolicy(Widget.PositionPolicy.CENTER)
            nodeNameText:setMargin(3)
            nodeNameContainer:addChild(nodeNameText)
        end

        nodeWidget:addChild(nodeNameContainer)
    end

    do
        local pinsWidget = Widget.makeLineFlow()

        do
            local inputPinsWidget = Widget.makeColumnFlow()
            inputPinsWidget:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)
            for i = 1, #node.inputPins do
                local pin = node.inputPins[i]
                local inputPinWidget = self:makeInputPinWidget(node, pin)
                self:setInputPinPlugged(pin, pin.pluggedOutputPin ~= nil)
                inputPinsWidget:addChild(inputPinWidget)
            end
            pinsWidget:addChild(inputPinsWidget)
        end

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

function NodeWidget:rebuild()
    self.container:removeAllChildren()
    self:build()
end

function NodeWidget:makeInputPinWidget(node, pin)
    local inputPinWidget = Widget.makeLineFlow()

    do
        local inputPinPlugWidget = Widget.makeFixedSize(8, 8)
        inputPinPlugWidget:setPositionPolicy(Widget.PositionPolicy.CENTER)
        inputPinPlugWidget:setMargin(0, 3, 0, 0)
        inputPinPlugWidget:setBackgroundColor(self:getPinColorByType(pin.pinType))

        local inputPinSocketWidget = Widget.makeFixedSize(6, 4)
        inputPinSocketWidget:setBackgroundColor(0xECF0F1FF)
        inputPinSocketWidget:setMargin(2, 2, 2, 0)
        inputPinPlugWidget:addChild(inputPinSocketWidget)
        self.inputPinSocketWidgets[pin] = inputPinSocketWidget

        inputPinPlugWidget:mouseDown(function()
            if pin.pluggedOutputPin then
                local outputPin = pin.pluggedOutputPin.outputPin
                local outputNode = pin.pluggedOutputPin.node
                node:unplugInputPin(pin)
                self:setInputPinPlugged(pin, false)
                self.mainWindow:beginDragWireFromOutputPin(outputNode, outputPin)
            else
                self.mainWindow:beginDragWireFromInputPin(node, pin)
            end
            return true
        end)

        inputPinPlugWidget:mouseUp(function()
            return self.mainWindow:linkReleasedOnInputPin(node, pin)
        end)

        inputPinWidget:addChild(inputPinPlugWidget)
        self.inputPinPlugWidgets[pin] = inputPinPlugWidget
    end

    do
        local inputPinNameWidget = Widget.makeText(pin.pinName, table.unpack(flat.ui.settings.defaultFont))
        inputPinNameWidget:setTextColor(0x000000FF)
        inputPinNameWidget:setMargin(1, 0, 1, 0)
        inputPinWidget:addChild(inputPinNameWidget)
    end

    return inputPinWidget
end

function NodeWidget:makeOutputPinWidget(node, pin)
    local outputPinWidget = Widget.makeLineFlow()
    outputPinWidget:setPositionPolicy(Widget.PositionPolicy.TOP_RIGHT)

    do
        local outputPinNameWidget = Widget.makeText(pin.pinName, table.unpack(flat.ui.settings.defaultFont))
        outputPinNameWidget:setTextColor(0x000000FF)
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
            outputPinSocketWidget:setBackgroundColor(0xECF0F1FF)
            outputPinSocketWidget:setMargin(2, 0, 2, 2)
            outputPinPlugWidget:addChild(outputPinSocketWidget)
            self.outputPinSocketWidgets[pin] = outputPinSocketWidget
        end

        outputPinPlugWidget:mouseDown(function()
            self.mainWindow:beginDragWireFromOutputPin(node, pin)
            return true
        end)

        outputPinPlugWidget:mouseUp(function()
            return self.mainWindow:linkReleasedOnOutputPin(node, pin)
        end)

        outputPinWidget:addChild(outputPinPlugWidget)
        self.outputPinPlugWidgets[pin] = outputPinPlugWidget
    end

    return outputPinWidget
end

function NodeWidget:select()
    self.container:setBackgroundColor(0x9BA1A5FF)
end

function NodeWidget:deselect()
    self.container:setBackgroundColor(0xBDC3C7FF)
end

local pinColors = {
    [PinTypes.ANY]        = 0x888888FF,
    [PinTypes.IMPULSE]    = 0xE74C3CFF,
    [flat.types.BOOLEAN]  = 0x2ECC71FF,
    [flat.types.NUMBER]   = 0xF39C12FF,
    [flat.types.STRING]   = 0x3498DBFF,
    [flat.types.TABLE]    = 0x8E44ADFF,
    [flat.types.FUNCTION] = 0x34495EFF,

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
    local inputPins = self.node.inputPins
    for i = 1, #inputPins do
        local inputPin = inputPins[i]
        local plugged = inputPin.pluggedOutputPin ~= nil
        self:setInputPinPlugged(inputPin, plugged)
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

return NodeWidget