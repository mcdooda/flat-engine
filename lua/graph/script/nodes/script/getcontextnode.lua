local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local GetContextNode = ScriptNode:inherit 'Get Context'

function GetContextNode:init()
    self.contextType = PinTypes.ANY
end

function GetContextNode:buildPins()
    self.contextOutPin = self:addOutputPin(self.contextType, 'Context')
end

function GetContextNode:addedToGraph(graph)
    graph:addContextNode(self)
    self.contextType = graph:getContextType()
    if self.contextOutPin then
        self.contextOutPin.pinType = self.contextType
    end
end

function GetContextNode:setContextType(contextType)
    if self.contextType ~= contextType then
        if self.contextType ~= PinTypes.ANY then -- do not unplug ANY pins as it's loading
            self:unplugOutputPin(self.contextOutPin)
        end
        self.contextType = contextType
        self.contextOutPin.pinType = contextType
    end
end

function GetContextNode:execute(runtime)
    -- nothing to do, the pin has already been written when initializing the script runtime
end

return GetContextNode