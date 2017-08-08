local ScriptNode = require 'script/scriptnode'
local PinTypes = require 'pintypes'

local InitNode = ScriptNode:inherit()

function InitNode:buildPins()
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE)
end

function InitNode:execute(runtime)
    runtime:impulse(self.impulseOutPin)
end

function InitNode:addedToGraph(graph)
    graph:addEntryNode(self)
end

return InitNode