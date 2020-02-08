local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local MissingNode = Node:inherit 'Boolean'

function MissingNode:new(name)
    local o = Node.new(self)
    o.name = name
    o.isMissingNode = true
    return o
end

function MissingNode:buildPins()
    -- nothing to do
end

function MissingNode:getName()
    return self.name .. ' (missing)'
end

function MissingNode:getInputPin(inputPinIndex)
    assert(inputPinIndex >= 1)
    for i = #self.inputPins + 1, inputPinIndex, 1 do
        self:addInputPin(PinTypes.ANY, 'Input #' .. tostring(i))
    end
    return self.inputPins[inputPinIndex]
end

function MissingNode:getOutputPin(outputPinIndex)
    assert(outputPinIndex >= 1)
    for i = #self.outputPins + 1, outputPinIndex, 1 do
        self:addOutputPin(PinTypes.ANY, 'Output #' .. tostring(i))
    end
    return self.outputPins[outputPinIndex]
end

return MissingNode