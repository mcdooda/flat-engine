local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local StringNode = FunctionalScriptNode:inherit 'String'

function StringNode:buildPins()
    self.stringOutPin = self:addOutputPin(PinTypes.STRING, '')
end

function StringNode:execute(runtime)
    runtime:writePin(self.stringOutPin, self.value)
end

function StringNode:setValue(value)
    assert(type(value) == 'string', 'Wrong type for setValue: string expected, got ' .. type(value))
    self.value = value
end

function StringNode:getValue()
    assert(type(self.value) == 'string', 'Wrong type for self.value: string expected, got ' .. type(self.value))
    return self.value
end

function StringNode:init()
    self:setValue ''
end

function StringNode:getLoadArguments()
    return self.value
end

StringNode.load = StringNode.setValue

return StringNode