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
    self.value = value
end

return NumberNode