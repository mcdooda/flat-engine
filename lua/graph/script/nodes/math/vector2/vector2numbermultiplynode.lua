local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector2NumberMultiplyNode = FunctionalScriptNode:inherit 'Vector2 * Number'

function Vector2NumberMultiplyNode:buildPins()
    self.vector2InPin = self:addInputPin(flat.types['flat.Vector2'], 'Vector2')
    self.factorInPin = self:addInputPin(flat.types.NUMBER, 'Factor')

    self.resultOutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Result')
end

function Vector2NumberMultiplyNode:execute(runtime)
    local vector2 = runtime:readPin(self.vector2InPin)
    local factor = runtime:readPin(self.factorInPin)

    local result = vector2 * factor

    runtime:writePin(self.resultOutPin, result)
end

return Vector2NumberMultiplyNode