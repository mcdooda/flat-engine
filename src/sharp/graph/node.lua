local Node = {}
Node.__index = Node

function Node:new()
    local o = setmetatable({
        inputPins = {},
        outputPins = {}
    }, self)
    o:buildPins()
    return o
end

function Node:inherit()
    local nodeType = {}
    nodeType.__index = nodeType
    return setmetatable(nodeType, { __index = self })
end

function Node:addedToGraph(graph)

end

function Node:addInputPin(pinType, pinName)
    local inputPin = {
        pinType = pinType,
        pinName = pinName,
        pluggedOutputPin = nil
    }
    self.inputPins[#self.inputPins + 1] = inputPin
    return inputPin
end

function Node:addOutputPin(pinType, pinName)
    local outputPin = {
        pinType = pinType,
        pinName = pinName,
        pluggedInputPins = {}
    }
    self.outputPins[#self.outputPins + 1] = outputPin
    return outputPin
end

function Node:clearPins()
    self.inputPins = {}
    self.outputPins = {}
end

function Node:buildPins()
    error('pure virtual function')
end

function Node:rebuildPins()
    self:clearPins()
    self:buildPins()
end

function Node:plugPins(outputPin, node, inputPin)
    assert(outputPin.pinType == inputPin.pinType)
    outputPin.pluggedInputPins[#outputPin.pluggedInputPins + 1] = {
        inputPin = inputPin,
        node = node
    }
    inputPin.pluggedOutputPin = {
        outputPin = outputPin,
        node = self
    }
end

return Node