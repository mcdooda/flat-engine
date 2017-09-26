local PinTypes = flat.require 'graph/pintypes'

local ScriptNodeRuntime = {}
ScriptNodeRuntime.__index = ScriptNodeRuntime

function ScriptNodeRuntime:new(scriptRuntime, node, inputPinValues, outputPinValues)
    local o = {
        scriptRuntime = scriptRuntime,
        node = node,
        inputPinValues = inputPinValues,
        outputPinValues = outputPinValues
    }
    return setmetatable(o, self)
end

function ScriptNodeRuntime:inherit()
    local runtimeType = {}
    runtimeType.__index = runtimeType
    return setmetatable(runtimeType, { __index = self })
end

function ScriptNodeRuntime:readPin(inputPin)
    local value = self:readOptionalPin(inputPin)
    assert(value ~= nil, 'Cannot read nil from pin ' .. inputPin.pinName .. ' of type ' .. PinTypes:toString(inputPin.pinType))
    return value
end

function ScriptNodeRuntime:readOptionalPin(inputPin, defaultValue)
    local pinValue = self.inputPinValues[inputPin]
    assert(pinValue, 'invalid input pin')

    local pluggedOutputPin = inputPin.pluggedOutputPin
    local node = pluggedOutputPin.node
    local nodeRuntime = self.scriptRuntime:getNodeRuntime(node)
    node:prepareReadPin(nodeRuntime, inputPin)

    local value = pinValue.value
    if value == nil then
        return defaultValue
    else
        return value
    end
end

function ScriptNodeRuntime:writePin(outputPin, value)
    assert(value ~= nil, 'Cannot write nil to pin ' .. outputPin.pinName .. ' of type ' .. PinTypes:toString(outputPin.pinType))
    self:writeOptionalPin(outputPin, value)
end

function ScriptNodeRuntime:writeOptionalPin(outputPin, value)
    local pinValue = self.outputPinValues[outputPin]
    assert(pinValue, 'invalid output pin')
    assert(
        value == nil or PinTypes[string.upper(type(value))] == outputPin.pinType,
        'wrong value for pin ' .. outputPin.pinName .. ': ' .. tostring(value) .. ' of type ' .. type(value) .. ', expected ' .. PinTypes:toString(outputPin.pinType)
    )
    pinValue.value = value
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