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
    local pluggedOutputPin = inputPin.pluggedOutputPin
    local node = pluggedOutputPin.node
    local nodeRuntime = self.scriptRuntime:getNodeRuntime(node)
    node:prepareReadPin(nodeRuntime, inputPin)

    local pinValue = self.inputPinValues[inputPin]
    return pinValue.value
end

function ScriptNodeRuntime:writePin(outputPin, value)
    local pinValue = self.outputPinValues[outputPin]
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