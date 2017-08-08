local ScriptNode = require 'script/scriptnode'
local PinTypes = require 'pintypes'

local TestNode = ScriptNode:inherit()

function TestNode:buildPins()
    self.numberInPin = self:addInputPin(PinTypes.NUMBER)
    self.stringInPin = self:addInputPin(PinTypes.STRING)
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE)

    self.numberOutPin = self:addOutputPin(PinTypes.NUMBER)
    self.stringOutPin = self:addOutputPin(PinTypes.STRING)
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE)
end

function TestNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local number = runtime:readPin(self.numberInPin)
    local string = runtime:readPin(self.stringInPin)

    print('number: ' .. tostring(number))
    print('string: ' .. tostring(string))

    runtime:writePin(self.numberOutPin, number)
    runtime:writePin(self.stringOutPin, string)
    runtime:impulse(self.impulseOutPin)
end

return TestNode