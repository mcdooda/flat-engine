
local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'
local IsInRangeNode = FunctionalScriptNode:inherit 'Is In Range'
function IsInRangeNode:buildPins()
    self.operand1InPin = self:addInputPin(flat.types.NUMBER, 'Operand 1')
    self.operand2InPin = self:addInputPin(flat.types.NUMBER, 'Operand 2')
    self.operand3InPin = self:addInputPin(flat.types.NUMBER, 'Operand 3')

    self.resultOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Out')
end


function IsInRangeNode.compute(operand1, operand2, operand3)
    return operand1 > operand2 and operand1 < operand3
end

function IsInRangeNode:execute(runtime)
    local operand1 = runtime:readPin(self.operand1InPin)
    local operand2 = runtime:readPin(self.operand2InPin)
    local operand3 = runtime:readPin(self.operand3InPin)
    print(operand1)
    print(operand2)
    print(operand3)

    local result = self.compute(operand1, operand2, operand3)

    runtime:writePin(self.resultOutPin, result)
end

return IsInRangeNode