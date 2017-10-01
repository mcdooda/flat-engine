local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local StringNode = FunctionalScriptNode:inherit 'String'

function StringNode:buildPins()
    self.stringOutPin = self:addOutputPin(flat.types.STRING, '')
end

function StringNode:execute(runtime)
    runtime:writePin(self.stringOutPin, self.value)
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

function StringNode:getLoadArguments()
    return self.value
end

StringNode.load = StringNode.setValue

return StringNode