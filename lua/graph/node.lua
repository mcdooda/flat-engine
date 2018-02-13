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

function Node:addInputPin(pinType, pinName, onPlugged, onUnplugged)
    assert(pinType, 'no pin type for pin ' .. tostring(pinName))
    assert(pinName, 'no pin name')
    local inputPin = {
        pinType = pinType,
        pinName = pinName,
        pluggedOutputPin = nil,
        onPlugged = onPlugged
    }

    -- the editor always run with flat.debug
    if flat.debug then
        inputPin.onUnplugged = onUnplugged
    end

    self.inputPins[#self.inputPins + 1] = inputPin
    return inputPin
end

function Node:removeInputPin(inputPin)
    local inputPinIndex = self:findInputPinIndex(inputPin)
    for i = inputPinIndex, #self.inputPins - 1 do
        self.inputPins[i] = self.inputPins[i + 1]
    end
    self.inputPins[#self.inputPins] = nil
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

function Node:addOutputPin(pinType, pinName, onPlugged, onUnplugged)
    assert(pinType, 'no pin type for pin ' .. tostring(pinName))
    assert(pinName, 'no pin name')
    local outputPin = {
        pinType = pinType,
        pinName = pinName,
        pluggedInputPins = {},
        onPlugged = onPlugged
    }

    -- the editor always run with flat.debug
    if flat.debug then
        outputPin.onUnplugged = onUnplugged
    end

    self.outputPins[#self.outputPins + 1] = outputPin
    return outputPin
end

function Node:removeOutputPin(outputPin)
    local outputPinIndex = self:findOutputPinIndex(outputPin)
    for i = outputPinIndex, #self.outputPins - 1 do
        self.outputPins[i] = self.outputPins[i + 1]
    end
    self.outputPins[#self.outputPins] = nil
end

function Node:getOutputPin(outputPinIndex)
    return self.outputPins[outputPinIndex]
end

function Node:findOutputPinIndex(outputPin)
    for i = 1, #self.outputPins do
        if self.outputPins[i] == outputPin then
            return i
        end
    end
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

function Node:plugPins(outputPin, inputNode, inputPin, otherOutputPinUnplugged)
    assert(not inputPin.pluggedOutputPin, 'the input pin is already plugged')
    assert(not otherOutputPinUnplugged or flat.debug)

    if outputPin.pinType == PinTypes.ANY then
        assert(inputPin.pinType ~= PinTypes.ANY)
        outputPin.pinType = inputPin.pinType
    elseif inputPin.pinType == PinTypes.ANY then
        inputPin.pinType = outputPin.pinType
    end

    assert(
        outputPin.pinType == inputPin.pinType,
        'type mismatch between pins '
        .. self:getName() .. ' -> ' .. outputPin.pinName .. ' (' .. self:pinTypeToString(outputPin.pinType) .. ') '
        .. 'and '
        .. inputNode:getName() .. ' -> ' .. inputPin.pinName .. ' (' .. self:pinTypeToString(inputPin.pinType) .. ')'
    )
    outputPin.pluggedInputPins[#outputPin.pluggedInputPins + 1] = {
        inputPin = inputPin,
        node = inputNode
    }
    inputPin.pluggedOutputPin = {
        outputPin = outputPin,
        node = self
    }

    local updateOutputNode = false
    local updateInputNode = false
    if outputPin.onPlugged then
        updateOutputNode = outputPin.onPlugged(self, outputPin)
    end
    if inputPin.onPlugged then
        updateInputNode = inputPin.onPlugged(inputNode, inputPin, otherOutputPinUnplugged)
    end
    return updateOutputNode, updateInputNode
end

function Node:unplugInputPin(inputPin, otherOutputPinPlugged)
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
    local outputNode = inputPin.pluggedOutputPin.node
    inputPin.pluggedOutputPin = nil

    local updateOutputNode = false
    local updateInputNode = false
    if outputPin.onUnplugged then
        updateOutputNode = outputPin.onUnplugged(outputNode, outputPin)
    end
    if inputPin.onUnplugged then
        updateInputNode = inputPin.onUnplugged(self, inputPin, otherOutputPinPlugged)
    end
    return updateOutputNode, updateInputNode
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

function Node:pinTypeToString(pinType)
    if pinType == PinTypes.ANY then
        return 'ANY'
    elseif pinType == PinTypes.IMPULSE then
        return 'IMPULSE'
    else
        assert(type(pinType) == 'number', 'pinType ' .. tostring(pinType) .. ' should be an integer')
        return flat.typetostring(pinType)
    end
end

function Node:makePinNameFromType(pinType)
    -- remove the prefix and make the first letter uppercase
    local pinName = self:pinTypeToString(pinType)
    pinName = pinName:gsub('^(.+)%.(.+)$', '%2')
    return pinName:sub(1, 1):upper() .. pinName:sub(2):lower()
end

return Node