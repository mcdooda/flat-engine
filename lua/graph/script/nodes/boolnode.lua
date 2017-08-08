local FunctionalScriptNode = require 'script/functionalscriptnode'
local PinTypes = require 'pintypes'

local BoolNode = FunctionalScriptNode:inherit()

function BoolNode:buildPins()
    self.boolOutPin = self:addOutputPin(PinTypes.BOOL)
end

function BoolNode:execute(runtime)
    runtime:writePin(self.boolOutPin, self.value)
end

function BoolNode:setValue(value)
    assert(type(value) == 'boolean')
    self.value = value
end

return BoolNode