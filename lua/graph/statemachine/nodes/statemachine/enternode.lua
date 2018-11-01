local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local EnterNode = Node:inherit 'Enter'

function EnterNode:buildPins()
    self.stateOutPin = self:addOutputPin(PinTypes.TO_STATE, 'State')
end

return EnterNode