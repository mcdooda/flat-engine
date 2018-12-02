local Node = flat.require 'graph/node'

local Vector2Node = Node:inherit 'Vector2'

function Vector2Node:buildPins()
    self.xInPin = self:addInputPin(flat.types.NUMBER, 'X')
    self.yInPin = self:addInputPin(flat.types.NUMBER, 'Y')

    self.vector2OutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Vector2')
end

return Vector2Node