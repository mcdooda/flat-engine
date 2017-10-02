local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector2Node = FunctionalScriptNode:inherit 'Vector2'

function Vector2Node:buildPins()
    self.xInPin = self:addInputPin(flat.types.NUMBER, 'X')
    self.yInPin = self:addInputPin(flat.types.NUMBER, 'Y')

    self.vector2OutPin = self:addOutputPin(flat.types['flat.Vector2'], 'Vector2')
end

function Vector2Node:execute(runtime)
    local x = runtime:readPin(self.xInPin)
    local y = runtime:readPin(self.yInPin)

    local result = flat.Vector2(x, y)

    runtime:writePin(self.vector2OutPin, result)
end

return Vector2Node