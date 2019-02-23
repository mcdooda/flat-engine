local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector2AddNode = FunctionalScriptNode:inherit 'Vector2 Add'

function Vector2AddNode:buildPins()
    self.operand1InPin = self:addInputPin(flat.types['flat.Vector2'], 'Operand 1')
    self.operand2InPin = self:addInputPin(flat.types['flat.Vector2'], 'Operand 2')

    self.resultOutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Result')
end

function Vector2AddNode:execute(runtime)
    local operand1 = runtime:readPin(self.operand1InPin)
    local operand2 = runtime:readPin(self.operand2InPin)

    local result = operand1 + operand2

    runtime:writePin(self.resultOutPin, result)
end

return Vector2AddNode