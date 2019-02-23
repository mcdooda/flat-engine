local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector2LengthNode = FunctionalScriptNode:inherit 'Vector2 Length'

function Vector2LengthNode:buildPins()
    self.vector2InPin = self:addInputPin(flat.types['flat.Vector2'], 'Vector2')

    self.lengthOutPin = self:addOutputPin(flat.types.NUMBER, 'Length')
end

function Vector2LengthNode:execute(runtime)
    local vector2 = runtime:readPin(self.vector2InPin)

    local result = vector2:length()

    runtime:writePin(self.lengthOutPin, result)
end

return Vector2LengthNode