local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local BoolNode = FunctionalScriptNode:inherit 'Bool'

function BoolNode:buildPins()
    self.boolOutPin = self:addOutputPin(flat.types.BOOLEAN, '')
end

function BoolNode:execute(runtime)
    runtime:writePin(self.boolOutPin, self.value)
end

function BoolNode:setValue(value)
    assert(flat.type(value) == flat.types.BOOLEAN, 'Wrong type for setValue: boolean expected, got ' .. flat.typename(value))
    self.value = value
end

function BoolNode:getValue()
    assert(flat.type(self.value) == flat.types.BOOLEAN, 'Wrong type for self.value: boolean expected, got ' .. flat.typename(self.value))
    return self.value
end

function BoolNode:init()
    self:setValue(false)
end

function BoolNode:getLoadArguments()
    return self.value
end

BoolNode.load = BoolNode.setValue

return BoolNode