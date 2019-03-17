local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local HoldValueNode = ScriptNode:inherit 'Hold Value'

function HoldValueNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.valueInPin = self:addInputPin(PinTypes.ANY, 'Value')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.valueOutPin = self:addOutputPin(PinTypes.ANY, 'Init Value')
end

function HoldValueNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local value = runtime:readPin(self.valueInPin)
    runtime:writePin(self.valueOutPin, value)
    runtime:impulse(self.impulseOutPin)
end

return HoldValueNode