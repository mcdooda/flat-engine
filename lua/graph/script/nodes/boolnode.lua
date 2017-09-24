local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local BoolNode = FunctionalScriptNode:inherit 'Bool'

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

function BoolNode:init()
    self:setValue(false)
end

function BoolNode:getLoadArguments()
    return self.value
end

BoolNode.load = BoolNode.setValue

return BoolNode