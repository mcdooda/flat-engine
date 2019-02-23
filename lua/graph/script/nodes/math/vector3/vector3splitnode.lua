local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3SplitNode = FunctionalScriptNode:inherit 'Vector3 Split'

function Vector3SplitNode:buildPins()
    self.vector3InPin = self:addInputPin(flat.types['flat.Vector3'], 'Vector3')

    self.xOutPin = self:addOutputPin(flat.types.NUMBER, 'X')
    self.yOutPin = self:addOutputPin(flat.types.NUMBER, 'Y')
    self.zOutPin = self:addOutputPin(flat.types.NUMBER, 'Z')
end

function Vector3SplitNode:execute(runtime)
    local vector3 = runtime:readPin(self.vector3InPin)

    runtime:writePin(self.xOutPin, vector3:x())
    runtime:writePin(self.yOutPin, vector3:y())
    runtime:writePin(self.zOutPin, vector3:z())
end

return Vector3SplitNode