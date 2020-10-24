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
        self:setOutputPinType(self.contextOutPin, self.contextType)
    end
end

function GetContextNode:setContextType(contextType, isLoadingGraph)
    if self.contextType ~= contextType then
        self.contextType = contextType
        self:setOutputPinType(self.contextOutPin, contextType, isLoadingGraph)
    end
end

function GetContextNode:execute(runtime)
    -- nothing to do, the pin has already been written when initializing the script runtime
end

return GetContextNode