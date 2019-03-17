local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ImpulseAllNode = ScriptNode:inherit 'Impulse All'

function ImpulseAllNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')

    if self.numOutputPins then
        for i = 1, self.numOutputPins - 1 do
            self:addOutputPin(PinTypes.IMPULSE, 'Out')
        end
        self.numOutputPins = nil
    end
    self:addOutputPin(PinTypes.IMPULSE, 'Out', self.addCustomPin)
end

function ImpulseAllNode:addCustomPin(pin, otherPin)
    self:addOutputPin(PinTypes.IMPULSE, 'Out', self.addCustomPin)
    return true
end

function ImpulseAllNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)

    for i = 1, #self.outputPins - 1 do
        runtime:impulse(self.outputPins[i])
    end
end

function ImpulseAllNode:getLoadArguments()
    return #self.outputPins
end

function ImpulseAllNode:load(numOutputPins)
    self.numOutputPins = numOutputPins
end

return ImpulseAllNode