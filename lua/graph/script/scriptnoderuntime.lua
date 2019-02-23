local PinTypes = flat.require 'graph/pintypes'

local ScriptNodeRuntime = {}
ScriptNodeRuntime.__index = ScriptNodeRuntime

function ScriptNodeRuntime:new(scriptRuntime, node, inputPinValues, outputPinValues)
    local o = setmetatable({
        scriptRuntime = scriptRuntime,
        node = node,
        inputPinValues = inputPinValues,
        outputPinValues = outputPinValues
    }, self)
    o:init(node)
    return o
end

function ScriptNodeRuntime:inherit()
    local runtimeType = {}
    runtimeType.__index = runtimeType
    return setmetatable(runtimeType, { __index = self })
end

function ScriptNodeRuntime:init()
    -- overriden when needed, called when creating the script runtime
end

function ScriptNodeRuntime:readPin(inputPin)
    local value = self:readOptionalPin(inputPin)
    if value == nil then
        local msg = 'Cannot read nil from pin ' .. self.node:getName() .. '->' .. inputPin.pinName .. ' of type ' .. self.node:pinTypeToString(inputPin.pinType)
        if inputPin.pluggedOutputPin then
            msg = msg .. ' ' .. '(plugged to ' .. inputPin.pluggedOutputPin.node:getName() .. '->' .. inputPin.pluggedOutputPin.outputPin.pinName .. ')'
        else
            msg = msg .. ' (not plugged)'
        end
        error(msg)
    end
    return value
end

function ScriptNodeRuntime:readOptionalPin(inputPin, defaultValue)
    local pinValue = self.inputPinValues[inputPin]
    if pinValue then
        local pluggedOutputPin = inputPin.pluggedOutputPin
        if pluggedOutputPin then
            local node = pluggedOutputPin.node
            local nodeRuntime = self.scriptRuntime:getNodeRuntime(node)
            node:prepareReadPin(nodeRuntime, pluggedOutputPin)

            local value = pinValue.value
            if value == nil then
                return defaultValue
            else
                return value
            end
        end
    end
end

-- should only be used in editor!
function ScriptNodeRuntime:tryReadFromOutputPin(outputPin)
    local pinValue = self.outputPinValues[outputPin]
    if pinValue then
        self.node:prepareReadPin(self, outputPin)
        return pinValue.value
    end
end

function ScriptNodeRuntime:writePin(outputPin, value)
    assert(outputPin and type(outputPin) == 'table')
    assert(
        value ~= nil,
        'Cannot write nil to pin ' .. self.node:getName() .. '->' .. outputPin.pinName .. ' of type ' .. self.node:pinTypeToString(outputPin.pinType)
    )
    self:writeOptionalPin(outputPin, value)
end

function ScriptNodeRuntime:writeOptionalPin(outputPin, value)
    -- pinValue is nil if the pin is not plugged to anything
    local pinValue = self.outputPinValues[outputPin]
    if pinValue then
        assert(
            value == nil or flat.type(value) == outputPin.pinType,
            'wrong value for pin ' .. outputPin.pinName .. ': ' .. tostring(value) .. ' of type ' .. flat.typetostring(flat.type(value)) .. ', expected ' .. flat.typetostring(outputPin.pinType)
        )
        pinValue.value = value
    end
end

function ScriptNodeRuntime:impulse(outputPin)
    local pluggedInputPins = outputPin.pluggedInputPins
    for i = 1, #pluggedInputPins do
        local pluggedInputPin = pluggedInputPins[i]
        local inputPin = pluggedInputPin.inputPin
        local node = pluggedInputPin.node
        local nodeRuntime = self.scriptRuntime:getNodeRuntime(node)
        node:execute(nodeRuntime, inputPin)
    end
end

return ScriptNodeRuntime