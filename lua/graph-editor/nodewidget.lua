local PinTypes = flat.require 'graph/pintypes'

local NodeWidget = {}
NodeWidget.__index = NodeWidget

function NodeWidget:new(node)
    assert(node)
    local o = setmetatable({
        node = node
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
                local inputPinWidget = self:makeInputPinWidget(pin)
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
                local outputPinWidget = self:makeOutputPinWidget(pin)
                outputPinsWidget:addChild(outputPinWidget)
            end
            pinsWidget:addChild(outputPinsWidget)
        end

        nodeWidget:addChild(pinsWidget)
    end

    self.container = nodeWidget
end

function NodeWidget:makeInputPinWidget(pin)
    local inputPinWidget = Widget.makeLineFlow()

    do
        local inputPinPlugWidget = Widget.makeFixedSize(8, 8)
        inputPinPlugWidget:setPositionPolicy(Widget.PositionPolicy.CENTER)
        inputPinPlugWidget:setMargin(0, 3, 0, 0)
        inputPinPlugWidget:setBackgroundColor(self:getPinColorByType(pin.pinType))
        inputPinWidget:addChild(inputPinPlugWidget)
    end

    do
        local inputPinNameWidget = Widget.makeText(pin.pinName, table.unpack(flat.ui.settings.defaultFont))
        inputPinNameWidget:setTextColor(0x000000FF)
        inputPinNameWidget:setMargin(1, 0, 1, 0)
        inputPinWidget:addChild(inputPinNameWidget)
    end

    return inputPinWidget
end

function NodeWidget:makeOutputPinWidget(pin)
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
        outputPinWidget:addChild(outputPinPlugWidget)
    end

    return outputPinWidget
end

local pinColors = {
    [PinTypes.IMPULSE]  = 0xE74C3CFF,
    [PinTypes.USERDATA] = 0x2ECC71FF,
    [PinTypes.BOOL]     = 0x0000FFFF,
    [PinTypes.NUMBER]   = 0xF39C12FF,
    [PinTypes.STRING]   = 0x3498DBFF,
}

function NodeWidget:getPinColorByType(pinType)
    return pinColors[pinType]
end

return NodeWidget