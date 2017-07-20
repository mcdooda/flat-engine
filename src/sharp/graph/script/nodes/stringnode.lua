local FunctionalScriptNode = require 'script/functionalscriptnode'
local PinTypes = require 'pintypes'

local StringNode = FunctionalScriptNode:inherit()

function StringNode:buildPins()
    self.stringOutPin = self:addOutputPin(PinTypes.STRING)
end

function StringNode:execute(runtime)
    runtime:writePin(self.stringOutPin, self.value)
end

function StringNode:setValue(value)
    self.value = value
end

return StringNode