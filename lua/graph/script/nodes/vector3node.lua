local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3Node = FunctionalScriptNode:inherit 'Vector3'

function Vector3Node:buildPins()
    self.xInPin = self:addInputPin(flat.types.NUMBER, 'X')
    self.yInPin = self:addInputPin(flat.types.NUMBER, 'Y')
    self.zInPin = self:addInputPin(flat.types.NUMBER, 'Z')

    self.vector3OutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Vector3')
end

function Vector3Node:execute(runtime)
    local x = runtime:readPin(self.xInPin)
    local y = runtime:readPin(self.yInPin)
    local z = runtime:readPin(self.zInPin)

    local result = flat.Vector3(x, y, z)

    runtime:writePin(self.vector3OutPin, result)
end

return Vector3Node