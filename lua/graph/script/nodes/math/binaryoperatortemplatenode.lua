local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

return function(name, op1Type, op1Name, op2Type, op2Name, resType, resName)
    local BinaryOperatorTemplateNode = FunctionalScriptNode:inherit(name)

    function BinaryOperatorTemplateNode:buildPins()
        self.operand1InPin = self:addInputPin(op1Type, op1Name)
        self.operand2InPin = self:addInputPin(op2Type, op2Name)

        self.resultOutPin = self:addOutputPin(resType, resName)
    end

    function BinaryOperatorTemplateNode:execute(runtime)
        local operand1 = runtime:readPin(self.operand1InPin)
        local operand2 = runtime:readPin(self.operand2InPin)

        local result = self.compute(operand1, operand2)

        runtime:writePin(self.resultOutPin, result)
    end

    function BinaryOperatorTemplateNode.compute(operand1, operand2)
        error('BinaryOperatorTemplateNode.compute is a pure virtual function')
    end

    return BinaryOperatorTemplateNode
end