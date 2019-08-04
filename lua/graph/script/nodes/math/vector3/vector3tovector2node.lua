local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3ToVector2Node = FunctionalScriptNode:inherit 'Vector3 to Vector2'

function Vector3ToVector2Node:buildPins()
    self.vector3InPin = self:addInputPin(flat.types['flat.Vector3'], 'Vector3')

    self.vector2InPin = self:addOutputPin(flat.types['flat.Vector2'], 'Vector2')
end

function Vector3ToVector2Node:execute(runtime)
    local vector3 = runtime:readPin(self.vector3InPin)

    runtime:writePin(self.vector2InPin, vector3:toVector2())
end

return Vector3ToVector2Node