local PinTypes = flat.require 'graph/pintypes'

local Node = {}
Node.__index = Node

function Node:new()
    local o = setmetatable({
        inputPins = {},
        outputPins = {}
    }, self)
    o:init()
    return o
end

function Node:getName()
    assert(self.name, 'the node has no name')
    return self.name
end

function Node:inherit(name)
    -- name is optional (for virtual classes)
    local nodeType = {
        name = name
    }
    nodeType.__index = nodeType
    return setmetatable(nodeType, { __index = self })
end

function Node:addedToGraph(graph)
    -- overidden when needed, called after the node is added to a graph
end

function Node:init()
    -- overriden when needed, called after a new node is instanced
end

function Node:load(...)
    -- overidden when needed, called after deserialization
end

function Node:getLoadArguments()
    -- overriden when needed, should return the arguments to pass to load(...)
end

function Node:addInputPin(pinType, pinName)
    assert(pinType, 'no pin type')
    assert(pinName, 'no pin name')
    local inputPin = {
        pinType = pinType,
        pinName = pinName,
        pluggedOutputPin = nil
    }
    self.inputPins[#self.inputPins + 1] = inputPin
    return inputPin
end

function Node:addInputPinAny(pinName, onPlugged)
    assert(onPlugged, 'no plugged callback')
    local inputPin = self:addInputPin(PinTypes.ANY, pinName)
    inputPin.onPlugged = onPlugged
    return inputPin
end

function Node:getInputPin(inputPinIndex)
    return self.inputPins[inputPinIndex]
end

function Node:findInputPinIndex(inputPin)
    for i = 1, #self.inputPins do
        if self.inputPins[i] == inputPin then
            return i
        end
    end
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

function Node:addOutputPinAny(pinName, onPlugged)
    assert(onPlugged, 'no plugged callback')
    local outputPin = self:addOutputPin(PinTypes.ANY, pinName)
    outputPin.onPlugged = onPlugged
    return outputPin
end

function Node:getOutputPin(outputPinIndex)
    return self.outputPins[outputPinIndex]
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
    assert(not inputPin.pluggedOutputPin, 'the input pin is already plugged')

    local onOutputPinPlugged
    local onInputPinPlugged
    if outputPin.pinType == PinTypes.ANY then
        assert(inputPin.pinType ~= PinTypes.ANY)
        outputPin.pinType = inputPin.pinType
        onOutputPinPlugged = outputPin.onPlugged
        outputPin.onPlugged = nil
    elseif inputPin.pinType == PinTypes.ANY then
        inputPin.pinType = outputPin.pinType
        onInputPinPlugged = inputPin.onPlugged
        inputPin.onPlugged = nil
    end

    assert(outputPin.pinType == inputPin.pinType, 'pin types mismatch')
    outputPin.pluggedInputPins[#outputPin.pluggedInputPins + 1] = {
        inputPin = inputPin,
        node = node
    }
    inputPin.pluggedOutputPin = {
        outputPin = outputPin,
        node = self
    }

    if onInputPinPlugged then
        return onInputPinPlugged(self, inputPin)
    elseif onOutputPinPlugged then
        return onOutputPinPlugged(self, outputPin)
    end
end

function Node:unplugInputPin(inputPin)
    assert(inputPin.pluggedOutputPin, 'the input pin is not plugged')
    local outputPin = inputPin.pluggedOutputPin.outputPin
    local numPluggedInputPins = #outputPin.pluggedInputPins
    for i = 1, numPluggedInputPins do
        local pluggedInputPin = outputPin.pluggedInputPins[i]
        if pluggedInputPin.inputPin == inputPin then
            if i < numPluggedInputPins then
                outputPin.pluggedInputPins[i] = outputPin.pluggedInputPins[numPluggedInputPins]
            end
            outputPin.pluggedInputPins[numPluggedInputPins] = nil
            break
        end
    end
    inputPin.pluggedOutputPin = nil
end

function Node:unplugAllPins()
    self:unplugAllInputPins()
    self:unplugAllOutputPins()
end

function Node:unplugAllInputPins()
    for i = 1, #self.inputPins do
        local inputPin = self.inputPins[i]
        if inputPin.pluggedOutputPin then
            self:unplugInputPin(inputPin)
        end
    end
end

function Node:unplugAllOutputPins()
    for i = 1, #self.outputPins do
        local outputPin = self.outputPins[i]
        for j = 1, #outputPin.pluggedInputPins do
            local pluggedInputPin = outputPin.pluggedInputPins[j]
            local node = pluggedInputPin.node
            local inputPin = pluggedInputPin.inputPin
            node:unplugInputPin(inputPin)
        end
    end
end

return Node