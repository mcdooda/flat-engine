local Node = flat.require 'graph/node'

local StringNode = Node:inherit 'String'

function StringNode:buildPins()
    self.stringOutPin = self:addOutputPin(flat.types.STRING, '')
end

function StringNode:setValue(value)
    assert(flat.type(value) == flat.types.STRING, 'Wrong type for setValue: string expected, got ' .. flat.typetostring(flat.type(value)))
    self.value = value
end

function StringNode:getValue()
    assert(flat.type(self.value) == flat.types.STRING, 'Wrong type for self.value: string expected, got ' .. flat.typetostring(flat.type(self.value)))
    return self.value
end

function StringNode:init()
    self:setValue ''
end

function StringNode:isConstant()
    return true
end

function StringNode:getLoadArguments()
    return self.value
end

StringNode.load = StringNode.setValue

return StringNode