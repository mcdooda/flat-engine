local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'

local RuleNode = InnerGraphNode:inherit 'Rule'

function RuleNode:init()
    InnerGraphNode.init(self)
    self.innerGraph.nodeType = 'script'
end

function RuleNode:setContextType(contextType)
    self.innerGraph:setContextType(contextType)
end

function RuleNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')

    local numRulesInPins = self.numRulesInPins or 1
    self.numRulesInPins = nil
    self.rulesInPins = {}
    for i = 1, numRulesInPins do
        self.rulesInPins[i] = self:addInputPin(PinTypes.STATE_TO_RULE, 'Rules', self.inputPinPlugged, self.inputPinUnplugged)
    end

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
end

function RuleNode:inputPinPlugged(inputPin, outputPin, otherOutputPinUnplugged, isLoadingGraph)
    if not isLoadingGraph and not otherOutputPinUnplugged then
        flat.arrayAdd(self.rulesInPins, self:addInputPin(PinTypes.STATE_TO_RULE, 'Rules', self.inputPinPlugged, self.inputPinUnplugged))
    end
    return true
end

function RuleNode:inputPinUnplugged(inputPin, otherOutputPinPlugged)
    if not otherOutputPinPlugged and #self.rulesInPins > 0 then
        self:removeInputPin(inputPin)
        flat.arrayRemoveValue(self.rulesInPins, inputPin)
        return true
    end
end

function RuleNode:getLoadArguments()
    return #self.rulesInPins, InnerGraphNode.getLoadArguments(self)
end

function RuleNode:load(numRulesInPins, subGraphId, savedGraph)
    self.numRulesInPins = numRulesInPins
    savedGraph.nodeType = self.innerGraph.nodeType
    return InnerGraphNode.load(self, subGraphId, savedGraph)
end

return RuleNode