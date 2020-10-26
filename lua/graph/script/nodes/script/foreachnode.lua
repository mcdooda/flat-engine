local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local ForEachNode = ScriptNode:inherit 'For Each'

function ForEachNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.arrayInPin = self:addInputPin(flat.types.TABLE, 'Array')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.loopOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Loop')
    self.itemOutPin = self:addOutputPin(PinTypes.ANY, 'Item')
end

function ForEachNode:execute(runtime)
    local array = runtime:readPin(self.arrayInPin)

    for i = 1, #array do
        local item = array[i]
        runtime:writePin(self.itemOutPin, item)
        runtime:impulse(self.loopOutPin)
    end

    runtime:impulse(self.impulseOutPin)
end

return ForEachNode