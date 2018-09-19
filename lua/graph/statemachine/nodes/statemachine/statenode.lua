local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local StateNode = Node:inherit 'State'

local constantPinsCount = 1

function StateNode:buildPins()
    self:addInputPin(flat.types.STRING, 'Name')
    local numInputPins = self.numInputPins or 1
    for i = 1, numInputPins do
        self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    end
    self.numInputPins = nil

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
    self.rulesOutPin = self:addOutputPin(PinTypes.STATE_TO_RULE, 'Rules')
end

function StateNode:inputPinPlugged(inputPin, outputPin, otherOutputPinUnplugged, isLoadingGraph)
    if not isLoadingGraph then
        self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    end
    return true
end

function StateNode:inputPinUnplugged(inputPin)
    if #self.inputPins > constantPinsCount + 1 then
        self:removeInputPin(inputPin)
        return true
    end
end

function StateNode:getLoadArguments()
    return #self.inputPins - constantPinsCount
end

function StateNode:load(numInputPins)
    self.numInputPins = numInputPins
    return true
end

return StateNode