local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3NumberMultiplyNode = FunctionalScriptNode:inherit 'Vector3 * Number'

function Vector3NumberMultiplyNode:buildPins()
    self.vector3InPin = self:addInputPin(flat.types['flat.Vector3'], 'Vector3')
    self.factorInPin = self:addInputPin(flat.types.NUMBER, 'Factor')

    self.resultOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Result')
end

function Vector3NumberMultiplyNode:execute(runtime)
    local vector3 = runtime:readPin(self.vector3InPin)
    local factor = runtime:readPin(self.factorInPin)

    local result = vector3 * factor

    runtime:writePin(self.resultOutPin, result)
end

return Vector3NumberMultiplyNode