local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local LuaVariableNode = FunctionalScriptNode:inherit 'Lua Variable'

function LuaVariableNode:buildPins()
    self.nameInPin = self:addInputPin(flat.types.STRING, 'Name')

    self.valueOutPin = self:addOutputPin(PinTypes.ANY, 'Value')
end

function LuaVariableNode:execute(runtime)
    local name = runtime:readPin(self.nameInPin)

    local value, status, func
    status, func = pcall(load, 'return ' .. name)
    if not status then
        flat.ui.error('Invalid variable name: ' .. name)
    end
    if func then
        status, value = pcall(func)
        if not value then
            flat.ui.error('Could not get variable: ' .. name)
        end
    end

    runtime:writeOptionalPin(self.valueOutPin, value)
end

return LuaVariableNode