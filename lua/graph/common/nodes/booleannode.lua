local Node = flat.require 'graph/node'

local BooleanNode = Node:inherit 'Boolean'

function BooleanNode:buildPins()
    self.boolOutPin = self:addOutputPin(flat.types.BOOLEAN, '')
end

function BooleanNode:setValue(value)
    assert(flat.type(value) == flat.types.BOOLEAN, 'Wrong type for setValue: boolean expected, got ' .. flat.typetostring(flat.type(value)))
    self.value = value
end

function BooleanNode:getValue()
    assert(flat.type(self.value) == flat.types.BOOLEAN, 'Wrong type for self.value: boolean expected, got ' .. flat.typetostring(flat.type(self.value)))
    return self.value
end

function BooleanNode:init()
    self:setValue(false)
end

function BooleanNode:getLoadArguments()
    return self.value
end

BooleanNode.load = BooleanNode.setValue

return BooleanNode