local Node = flat.require 'graph/node'
local PinTypes = flat.require 'graph/pintypes'

local RerouteNode = Node:inherit 'Reroute'

function RerouteNode:addedToGraph(graph)
    graph:addReroute(self)
end

function RerouteNode:buildPins()
    self.inPin = self:addInputPin(PinTypes.ANY, '', self.inPinPlugged, self.inPinUnplugged)

    self.outPin = self:addOutputPin(PinTypes.ANY, '', self.outPinPlugged, self.outPinUnplugged)
end

function RerouteNode:inPinPlugged(pin)
    self:setOutputPinType(self.outPin, pin.pinType)
    return true
end

function RerouteNode:inPinUnplugged(pin)
    if not self:isOutputPinPlugged(self.outPin) then
        self:setInputPinType(self.inPin, PinTypes.ANY)
        self:setOutputPinType(self.outPin, PinTypes.ANY)
        return true
    end
end

function RerouteNode:outPinPlugged(pin)
    self:setInputPinType(self.inPin, pin.pinType)
    return true
end

function RerouteNode:outPinUnplugged(pin)
    if not self:isInputPinPlugged(self.inPin) and not self:isOutputPinPlugged(pin) then
        self:setInputPinType(self.inPin, PinTypes.ANY)
        self:setOutputPinType(self.outPin, PinTypes.ANY)
        return true
    end
end

return RerouteNode