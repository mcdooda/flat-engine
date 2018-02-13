local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local PrintNode = ScriptNode:inherit 'Print'

function PrintNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.valueInPin = self:addInputPin(PinTypes.ANY, 'Value', function() return true end)

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function PrintNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local value = runtime:readPin(self.valueInPin)

    print('Print Node:', value)

    runtime:impulse(self.impulseOutPin)
end

return PrintNode