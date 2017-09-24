local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local TestNode = ScriptNode:inherit 'Test'

function TestNode:buildPins()
    self.boolInPin = self:addInputPin(PinTypes.BOOLEAN, 'Boolean')
    self.numberInPin = self:addInputPin(PinTypes.NUMBER, 'Number')
    self.stringInPin = self:addInputPin(PinTypes.STRING, 'String')
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')

    self.boolOutPin = self:addOutputPin(PinTypes.BOOLEAN, 'Boolean')
    self.numberOutPin = self:addOutputPin(PinTypes.NUMBER, 'Number')
    self.stringOutPin = self:addOutputPin(PinTypes.STRING, 'String')
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function TestNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local bool = runtime:readPin(self.boolInPin)
    local number = runtime:readPin(self.numberInPin)
    local string = runtime:readPin(self.stringInPin)

    print('bool: ' .. tostring(bool))
    print('number: ' .. tostring(number))
    print('string: ' .. tostring(string))

    runtime:writePin(self.boolOutPin, bool)
    runtime:writePin(self.numberOutPin, number)
    runtime:writePin(self.stringOutPin, string)
    runtime:impulse(self.impulseOutPin)
end

return TestNode