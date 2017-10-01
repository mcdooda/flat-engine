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
    assert(
        value ~= nil,
        'Cannot read nil from pin ' .. self.node:getName() .. ' -> ' .. inputPin.pinName .. ' of type ' .. self.node:pinTypeToString(inputPin.pinType)
    )
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
    assert(
        value ~= nil,
        'Cannot write nil to pin ' .. self.node:getName() .. ' -> ' .. outputPin.pinName .. ' of type ' .. self.node:pinTypeToString(outputPin.pinType)
    )
    self:writeOptionalPin(outputPin, value)
end

function ScriptNodeRuntime:writeOptionalPin(outputPin, value)
    local pinValue = self.outputPinValues[outputPin]
    assert(pinValue, 'invalid output pin')
    assert(
        value == nil or flat.type(value) == outputPin.pinType,
        'wrong value for pin ' .. outputPin.pinName .. ': ' .. tostring(value) .. ' of type ' .. flat.typetostring(flat.type(value)) .. ', expected ' .. flat.typetostring(outputPin.pinType)
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