local InnerGraphNode = flat.require 'graph/innergraphnode'
local PinTypes = flat.require 'graph/pintypes'

local StateMachineNode = InnerGraphNode:inherit 'State Machine'

function StateMachineNode:buildPins()
    
end

function StateMachineNode:getLoadArguments()
    return InnerGraphNode.getLoadArguments(self)
end

function StateMachineNode:load(graphPath)
    InnerGraphNode.load(self, graphPath)
end

return StateMachineNode