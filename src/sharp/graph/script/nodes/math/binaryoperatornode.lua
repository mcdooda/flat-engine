local FunctionalScriptNode = require 'script/functionalscriptnode'
local PinTypes = require 'pintypes'

local BinaryOperatorNode = FunctionalScriptNode:inherit()

function BinaryOperatorNode:buildPins()
    self.operand1InPin = self:addInputPin(PinTypes.NUMBER)
    self.operand2InPin = self:addInputPin(PinTypes.NUMBER)

    self.resultOutPin = self:addOutputPin(PinTypes.NUMBER)
end

function BinaryOperatorNode:execute(runtime)
    local operand1 = runtime:readPin(self.operand1InPin)
    local operand2 = runtime:readPin(self.operand2InPin)

    local result = self.compute(operand1, operand2)

    runtime:writePin(self.resultOutPin, result)
end

function BinaryOperatorNode.compute(operand1, operand2)
    error('pure virtual function')
end

return BinaryOperatorNode