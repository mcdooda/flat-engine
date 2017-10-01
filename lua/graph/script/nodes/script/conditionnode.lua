local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ConditionNode = ScriptNode:inherit 'Condition'

function ConditionNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.conditionInPin = self:addInputPin(flat.types.BOOLEAN, 'Condition')

    self.trueImpulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'True')
    self.falseImpulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'False')
end

function ConditionNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local condition = runtime:readPin(self.conditionInPin)

    local impulseOutPin = condition and self.trueImpulseOutPin or self.falseImpulseOutPin

    runtime:impulse(impulseOutPin)
end

return ConditionNode