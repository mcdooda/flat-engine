local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local StateNode = Node:inherit 'State'

function StateNode:buildPins()
    self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
    self.rulesOutPin = self:addOutputPin(PinTypes.STATE_TO_RULE, 'Rules')
end

function StateNode:inputPinPlugged()
    self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    return true
end

function StateNode:inputPinUnplugged(inputPin)
    if #self.inputPins > 1 then
        self:removeInputPin(inputPin)
        return true
    end
end

function StateNode:getLoadArguments()
    return #self.inputPins
end

function StateNode:load(numInputPins)
    for i = 1, numInputPins - 1 do
        self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    end
    return true
end

return StateNode