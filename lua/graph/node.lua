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

function Node:isBrowsable()
    return true
end

function Node:inherit(name)
    -- name is optional (for virtual classes)
    local nodeType = {
        name = name
    }
    for k, v in pairs(self) do
        nodeType[k] = v
    end
    nodeType.__index = nodeType
    return setmetatable(nodeType, { __add = self.__add })
end

function Node:__add(nodeClass)
    -- used for multiple inheritance with (A + B):inherit()
    local mixin = {}
    for k, v in pairs(nodeClass) do
        mixin[k] = v
    end
    for k, v in pairs(self) do
        mixin[k] = v
    end
    mixin.__index = mixin
    return mixin
end

function Node:addedToGraph(graph)
    -- overidden when needed, called after the node is added to a graph
end

function Node:removedFromGraph(graph)
    -- overidden when needed, called after the node is removed from a graph (in editor)
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

function Node:setContextType(contextType)
    -- overriden when needed, set the inner graph context type or something...
end

function Node:clone()
    local clone = getmetatable(self):new()
    clone:load(self:getLoadArguments())
    clone:buildPins()
    return clone
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

    flat.arrayAdd(self.inputPins, inputPin)
    return inputPin
end

function Node:removeInputPin(inputPin)
    assert(not inputPin.pluggedOutputPin, 'the input pin is plugged')
    flat.arrayRemoveValue(self.inputPins, inputPin)
end

function Node:getInputPin(inputPinIndex)
    return self.inputPins[inputPinIndex]
end

function Node:findInputPinIndex(inputPin)
    return flat.arrayFindValueIndex(self.inputPins, inputPin)
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

    flat.arrayAdd(self.outputPins, outputPin)
    return outputPin
end

function Node:removeOutputPin(outputPin)
    flat.arrayRemoveValue(self.outputPins, outputPin)
end

function Node:getOutputPin(outputPinIndex)
    return self.outputPins[outputPinIndex]
end

function Node:findOutputPinIndex(outputPin)
    return flat.arrayFindValueIndex(self.outputPins, outputPin)
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

function Node:plugPins(outputPin, inputNode, inputPin, otherOutputPinUnplugged, isLoadingGraph)
    assert(outputPin, 'the output pin is missing')
    assert(inputNode, 'the input node is missing')
    assert(inputPin, 'the input pin is missing')
    assert(not inputPin.pluggedOutputPin, 'the input pin is already plugged')
    assert(not otherOutputPinUnplugged or flat.debug)
    assert(self:findOutputPinIndex(outputPin), 'output pin ' .. outputPin.pinName .. ' is not from node ' .. self:getName())
    assert(inputNode:findInputPinIndex(inputPin), 'input pin ' .. inputPin.pinName .. ' is not from node ' .. inputNode:getName())

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
    flat.arrayAdd(outputPin.pluggedInputPins, {
        inputPin = inputPin,
        node = inputNode
    })
    inputPin.pluggedOutputPin = {
        outputPin = outputPin,
        node = self
    }

    local updateOutputNode = false
    local updateInputNode = false
    if outputPin.onPlugged then
        updateOutputNode = outputPin.onPlugged(self, outputPin, inputPin, isLoadingGraph)
    end
    if inputPin.onPlugged then
        updateInputNode = inputPin.onPlugged(inputNode, inputPin, outputPin, otherOutputPinUnplugged, isLoadingGraph)
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

function Node:unplugOutputPin(outputPin)
    for j = #outputPin.pluggedInputPins, 1, -1 do
        local pluggedInputPin = outputPin.pluggedInputPins[j]
        local node = pluggedInputPin.node
        local inputPin = pluggedInputPin.inputPin
        node:unplugInputPin(inputPin)
    end
end

function Node:unplugAllPins()
    self:unplugAllInputPins()
    self:unplugAllOutputPins()
end

function Node:unplugAllInputPins()
    for i = #self.inputPins, 1, -1 do
        local inputPin = self.inputPins[i]
        if inputPin.pluggedOutputPin then
            self:unplugInputPin(inputPin)
        end
    end
end

function Node:unplugAllOutputPins()
    for i = #self.outputPins, 1, -1 do
        self:unplugOutputPin(self.outputPins[i])
    end
end

function Node:pinTypeToString(pinType)
    if PinTypes[pinType] then
        return pinType
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