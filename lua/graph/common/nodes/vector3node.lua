local Node = flat.require 'graph/node'

local Vector3Node = Node:inherit 'Vector3'

function Vector3Node:buildPins()
    self.xInPin = self:addInputPin(flat.types.NUMBER, 'X')
    self.yInPin = self:addInputPin(flat.types.NUMBER, 'Y')
    self.zInPin = self:addInputPin(flat.types.NUMBER, 'Z')

    self.vector3OutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Vector3')
end

return Vector3Node