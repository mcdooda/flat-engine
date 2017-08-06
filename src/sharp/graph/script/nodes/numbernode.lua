local FunctionalScriptNode = require 'script/functionalscriptnode'
local PinTypes = require 'pintypes'

local NumberNode = FunctionalScriptNode:inherit()

function NumberNode:buildPins()
    self.numberOutPin = self:addOutputPin(PinTypes.NUMBER)
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