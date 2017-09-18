local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local NumberNode = FunctionalScriptNode:inherit 'Number'

function NumberNode:new()
    local o = FunctionalScriptNode.new(self)
    o.value = 0
    return o
end

function NumberNode:getInitArguments()
    return { self.value }
end

function NumberNode:buildPins()
    self.numberOutPin = self:addOutputPin(PinTypes.NUMBER, 'Number')
end

function NumberNode:execute(runtime)
    runtime:writePin(self.numberOutPin, self.value)
end

function NumberNode:setValue(value)
    assert(type(value) == 'number')
    self.value = value
end

NumberNode.init = NumberNode.setValue

return NumberNode