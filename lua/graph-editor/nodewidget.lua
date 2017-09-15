local PinTypes = flat.require 'graph/pintypes'

local NodeWidget = {}
NodeWidget.__index = NodeWidget

function NodeWidget:new(node, mainWindow)
    assert(node, 'no node given')
    local o = setmetatable({
        node = node,
        mainWindow = mainWindow,
        inputPinPlugWidgets = {},
        inputPinSocketWidgets = {},
        outputPinPlugWidgets = {},
        outputPinSocketWidgets = {}
    }, self)
    o:build()
    return o
end

function NodeWidget:build()
    local nodeWidget = Widget.makeColumnFlow()
    local node = self.node

    nodeWidget:setBackgroundColor(0xBDC3C7FF)
    nodeWidget:setPositionPolicy(Widget.PositionPolicy.TOP_LEFT)

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
            local centerMargin = Widget.makeFixedSize(20, 1)
            pinsWidget:addChild(centerMargin)
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
    end

    self.container = nodeWidget
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
    [PinTypes.IMPULSE]  = 0xE74C3CFF,
    [PinTypes.USERDATA] = 0x2ECC71FF,
    [PinTypes.BOOL]     = 0x27AE60FF,
    [PinTypes.NUMBER]   = 0xF39C12FF,
    [PinTypes.STRING]   = 0x3498DBFF,
}

function NodeWidget:getPinColorByType(pinType)
    return pinColors[pinType]
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

return NodeWidget