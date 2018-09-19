local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local CommonVector3Node = flat.require 'graph/common/nodes/vector3node'

local Vector3Node = (CommonVector3Node + FunctionalScriptNode):inherit 'Vector3'

function Vector3Node:execute(runtime)
    local x = runtime:readPin(self.xInPin)
    local y = runtime:readPin(self.yInPin)
    local z = runtime:readPin(self.zInPin)

    local result = flat.Vector3(x, y, z)

    runtime:writePin(self.vector3OutPin, result)
end

return Vector3Node