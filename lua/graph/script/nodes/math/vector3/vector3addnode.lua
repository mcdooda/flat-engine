local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3NumberAddNode = FunctionalScriptNode:inherit 'Vector3 Add'

function Vector3NumberAddNode:buildPins()
    self.vector3AInPin = self:addInputPin(flat.types['flat.Vector3'], 'Operand 1')
    self.vector3BInPin = self:addInputPin(flat.types['flat.Vector3'], 'Operand 2')

    self.resultOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Result')
end

function Vector3NumberAddNode:execute(runtime)
    local vector3A = runtime:readPin(self.vector3AInPin)
    local vector3B = runtime:readPin(self.vector3BInPin)

    local result = vector3A + vector3B

    runtime:writePin(self.resultOutPin, result)
end

return Vector3NumberAddNode