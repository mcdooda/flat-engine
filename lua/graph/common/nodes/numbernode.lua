local Node = flat.require 'graph/node'

local NumberNode = Node:inherit 'Number'

function NumberNode:buildPins()
    self.numberOutPin = self:addOutputPin(flat.types.NUMBER, '')
end

function NumberNode:setValue(value)
    assert(flat.type(value) == flat.types.NUMBER, 'Wrong type for setValue: number expected, got ' .. flat.typetostring(flat.type(value)))
    self.value = value
end

function NumberNode:getValue()
    assert(flat.type(self.value) == flat.types.NUMBER, 'Wrong type for self.value: number expected, got ' .. flat.typetostring(flat.type(self.value)))
    return self.value
end

function NumberNode:init()
    self:setValue(0)
end

function NumberNode:getLoadArguments()
    return self.value
end

NumberNode.load = NumberNode.setValue

return NumberNode