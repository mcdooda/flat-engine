local FunctionalScriptNode = require 'script/functionalscriptnode'
local PinTypes = require 'pintypes'

local MultiplyNode = FunctionalScriptNode:inherit()

function MultiplyNode:buildPins()
    self.operand1InPin = self:addInputPin(PinTypes.NUMBER)
    self.operand2InPin = self:addInputPin(PinTypes.NUMBER)

    self.resultOutPin = self:addOutputPin(PinTypes.NUMBER)
end

function MultiplyNode:execute(runtime)
    local operand1 = runtime:readPin(self.operand1InPin)
    local operand2 = runtime:readPin(self.operand2InPin)

    runtime:writePin(self.resultOutPin, operand1 * operand2)
end

return MultiplyNode