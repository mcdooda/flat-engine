local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3LengthNode = FunctionalScriptNode:inherit 'Vector3 Length'

function Vector3LengthNode:buildPins()
    self.vector3InPin = self:addInputPin(flat.types['flat.Vector3'], 'Vector3')

    self.lengthOutPin = self:addOutputPin(flat.types.NUMBER, 'Length')
end

function Vector3LengthNode:execute(runtime)
    local vector3 = runtime:readPin(self.vector3InPin)

    local result = vector3:length()

    runtime:writePin(self.lengthOutPin, result)
end

return Vector3LengthNode