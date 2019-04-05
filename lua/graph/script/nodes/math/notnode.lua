local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local NotNode = FunctionalScriptNode:inherit 'Not'

function NotNode:buildPins()
    self.valueInPin = self:addInputPin(flat.types.BOOLEAN, 'Value')

    self.resultOutPin = self:addOutputPin(flat.types.BOOLEAN, 'Result')
end

function NotNode:execute(runtime)
    local value = runtime:readPin(self.valueInPin)

    local result = not value

    runtime:writePin(self.resultOutPin, result)
end

return NotNode