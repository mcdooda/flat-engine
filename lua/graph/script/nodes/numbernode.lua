local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local NumberNode = FunctionalScriptNode:inherit 'Number'

function NumberNode:buildPins()
    self.numberOutPin = self:addOutputPin(PinTypes.NUMBER, '')
end

function NumberNode:execute(runtime)
    runtime:writePin(self.numberOutPin, self.value)
end

function NumberNode:setValue(value)
    assert(type(value) == 'number', 'Wrong type for setValue: number expected, got ' .. type(value))
    self.value = value
end

function NumberNode:getValue()
    assert(type(self.value) == 'number', 'Wrong type for self.value: number expected, got ' .. type(self.value))
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