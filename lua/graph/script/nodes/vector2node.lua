local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local CommonVector2Node = flat.require 'graph/common/nodes/vector2node'

local Vector2Node = (CommonVector2Node + FunctionalScriptNode):inherit 'Vector2'

function Vector2Node:execute(runtime)
    local x = runtime:readPin(self.xInPin)
    local y = runtime:readPin(self.yInPin)

    local result = flat.Vector2(x, y)

    runtime:writePin(self.vector2OutPin, result)
end

return Vector2Node