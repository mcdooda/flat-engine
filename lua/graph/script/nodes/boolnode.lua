local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local BoolNode = FunctionalScriptNode:inherit 'Bool'

function BoolNode:new()
    local o = FunctionalScriptNode.new(self)
    o.value = false
    return o
end

function BoolNode:getInitArguments()
    return { self.value }
end

function BoolNode:buildPins()
    self.boolOutPin = self:addOutputPin(PinTypes.BOOL, '')
end

function BoolNode:execute(runtime)
    runtime:writePin(self.boolOutPin, self.value)
end

function BoolNode:setValue(value)
    assert(type(value) == 'boolean', 'Wrong type for setValue: boolean expected, got ' .. type(value))
    self.value = value
end

function BoolNode:getValue()
    assert(type(self.value) == 'boolean', 'Wrong type for self.value: boolean expected, got ' .. type(self.value))
    return self.value
end

BoolNode.init = BoolNode.setValue

return BoolNode