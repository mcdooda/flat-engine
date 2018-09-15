local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local RuleNode = Node:inherit 'Rule'

function RuleNode:buildPins()
    self.stateInPin = self:addInputPin(PinTypes.STATE_TO_RULE, 'Rules')

    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
end

return RuleNode