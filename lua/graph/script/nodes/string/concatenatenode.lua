local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ConcatenateNode = ScriptNode:inherit 'Concatenate'

function ConcatenateNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.string1InPin = self:addInputPin(flat.types.STRING, 'String 1')
    self.string2InPin = self:addInputPin(flat.types.STRING, 'String 2')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.resultOutPin = self:addOutputPin(flat.types.STRING, 'Result')
end

function ConcatenateNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local string1 = runtime:readPin(self.string1InPin)
    local string2 = runtime:readPin(self.string2InPin)

    local result = string1 .. string2

    runtime:writePin(self.resultOutPin, result)
    runtime:impulse(self.impulseOutPin)
end

return ConcatenateNode