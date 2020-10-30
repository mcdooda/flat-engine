local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'

local StateNode = InnerGraphNode:inherit 'State'

function StateNode:init()
    InnerGraphNode.init(self)
    self.innerGraph.nodeType = 'script'
end

function StateNode:setContextType(contextType, isLoadingGraph)
    self.innerGraph:setContextType(contextType, isLoadingGraph)
end

function StateNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')
    local numStateInPins = self.numStateInPins or 1
    self.numStateInPins = nil
    self.stateInPins = {}
    for i = 1, numStateInPins do
        self.stateInPins[i] = self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged)
    end

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
    self.rulesOutPin = self:addOutputPin(PinTypes.STATE_TO_RULE, 'Rules')
end

function StateNode:inputPinPlugged(inputPin, outputPin, otherOutputPinUnplugged, isLoadingGraph)
    if not isLoadingGraph and not otherOutputPinUnplugged then
        flat.arrayAdd(self.stateInPins, self:addInputPin(PinTypes.TO_STATE, 'State', self.inputPinPlugged, self.inputPinUnplugged))
    end
    return true
end

function StateNode:inputPinUnplugged(inputPin, otherOutputPinPlugged)
    if not otherOutputPinPlugged and #self.stateInPins > 0 then
        self:removeInputPin(inputPin)
        flat.arrayRemoveValue(self.stateInPins, inputPin)
        return true
    end
end

function StateNode:getLoadArguments()
    return #self.stateInPins, InnerGraphNode.getLoadArguments(self)
end

function StateNode:load(numStateInPins, subGraphId, savedGraph)
    self.numStateInPins = numStateInPins
    savedGraph.nodeType = self.innerGraph.nodeType
    return InnerGraphNode.load(self, subGraphId, savedGraph)
end

function StateNode:validate()
    return self:validateInnerGraph()
end

return StateNode