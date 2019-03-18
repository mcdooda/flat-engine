local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector2SplitNode = FunctionalScriptNode:inherit 'Vector2 Split'

function Vector2SplitNode:buildPins()
    self.Vector2InPin = self:addInputPin(flat.types['flat.Vector2'], 'Vector2')

    self.xOutPin = self:addOutputPin(flat.types.NUMBER, 'X')
    self.yOutPin = self:addOutputPin(flat.types.NUMBER, 'Y')
end

function Vector2SplitNode:execute(runtime)
    local Vector2 = runtime:readPin(self.Vector2InPin)

    runtime:writePin(self.xOutPin, Vector2:x())
    runtime:writePin(self.yOutPin, Vector2:y())
end

return Vector2SplitNode