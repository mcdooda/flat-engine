local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local BooleanNode = FunctionalScriptNode:inherit 'Boolean'

function BooleanNode:buildPins()
    self.boolOutPin = self:addOutputPin(flat.types.BOOLEAN, '')
end

function BooleanNode:execute(runtime)
    runtime:writePin(self.boolOutPin, self.value)
end

function BooleanNode:setValue(value)
    assert(flat.type(value) == flat.types.BOOLEAN, 'Wrong type for setValue: boolean expected, got ' .. flat.typetostring(flat.type(value)))
    self.value = value
end

function BooleanNode:getValue()
    assert(flat.type(self.value) == flat.types.BOOLEAN, 'Wrong type for self.value: boolean expected, got ' .. flat.typetostring(flat.type(value)))
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