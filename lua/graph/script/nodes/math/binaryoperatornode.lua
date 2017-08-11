local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local BinaryOperatorNode = FunctionalScriptNode:inherit()

function BinaryOperatorNode:buildPins()
    self.operand1InPin = self:addInputPin(PinTypes.NUMBER, 'Operand 1')
    self.operand2InPin = self:addInputPin(PinTypes.NUMBER, 'Operand 2')

    self.resultOutPin = self:addOutputPin(PinTypes.NUMBER, 'Result')
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