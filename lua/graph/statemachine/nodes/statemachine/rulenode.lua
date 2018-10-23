local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'

local RuleNode = InnerGraphNode:inherit 'Rule'

function RuleNode:init()
    InnerGraphNode.init(self)
    self.innerGraph.nodeType = 'script'
end

function RuleNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')
    self.stateInPin = self:addInputPin(PinTypes.STATE_TO_RULE, 'Rules')

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
end

function RuleNode:load(subGraphId, savedGraph)
    savedGraph.nodeType = self.innerGraph.nodeType
    return InnerGraphNode.load(self, subGraphId, savedGraph)
end

return RuleNode