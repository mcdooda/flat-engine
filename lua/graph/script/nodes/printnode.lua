local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local PrintNode = ScriptNode:inherit 'Print'

function PrintNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.valueInPin = self:addInputPin(PinTypes.ANY, 'Value', self.valuePinPlugged, self.valuePinUnplugged)

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function PrintNode:valuePinPlugged(pin)
    return true
end

function PrintNode:valuePinUnplugged(pin, otherOutputPinPlugged)
    if not otherOutputPinPlugged then
        self:setInputPinType(pin, PinTypes.ANY)
        return true
    end
end

function PrintNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local value = runtime:readPin(self.valueInPin)

    print('Print Node:', value)

    runtime:impulse(self.impulseOutPin)
end

return PrintNode