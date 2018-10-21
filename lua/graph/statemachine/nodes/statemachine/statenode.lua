local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'

local StateNode = InnerGraphNode:inherit 'State'

local constantPinsCount = 1

function StateNode:init()
    InnerGraphNode.init(self)
    self.innerGraph.nodeType = 'script'
end

function StateNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')
    local numInputPins = self.numInputPins or 1
    for i = 1, numInputPins do
        self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    end
    self.numInputPins = nil

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
    self.rulesOutPin = self:addOutputPin(PinTypes.STATE_TO_RULE, 'Rules')
end

function StateNode:inputPinPlugged(inputPin, outputPin, otherOutputPinUnplugged, isLoadingGraph)
    if not isLoadingGraph and not otherOutputPinUnplugged then
        self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    end
    return true
end

function StateNode:inputPinUnplugged(inputPin, otherOutputPinPlugged)
    if not otherOutputPinPlugged and #self.inputPins > constantPinsCount + 1 then
        self:removeInputPin(inputPin)
        return true
    end
end

function StateNode:getLoadArguments()
    return #self.inputPins - constantPinsCount, InnerGraphNode.getLoadArguments(self)
end

function StateNode:load(numInputPins, subGraphId, savedGraph)
    self.numInputPins = numInputPins
    savedGraph.nodeType = self.innerGraph.nodeType
    return InnerGraphNode.load(self, subGraphId, savedGraph)
end

return StateNode