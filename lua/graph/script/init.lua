local Graph = flat.require 'graph/graph'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'

flat.graph.script = {}

function makeErrorPathString(error)
    local str = ''
    for i = 1, #error.path do
        local graph = error.path[i].graph
        local nodeIndex = error.path[i].nodeIndex
        local node = graph.nodeInstances[nodeIndex]
        str = str .. 'Graph: ' .. graph.graphOrigin .. '\nNode: ' .. node:getName() .. '#' .. nodeIndex .. '\n'
    end
    if error.inputPinIndex then
        local graph = error.path[#error.path].graph
        local nodeIndex = error.path[#error.path].nodeIndex
        local nodeInstance = graph.nodeInstances[nodeIndex]
        local inputPin = nodeInstance.inputPins[error.inputPinIndex]
        str = str .. 'Input Pin ' .. inputPin.pinName .. '#' .. error.inputPinIndex
    end
    return str
end

function flat.graph.script.run(scriptPath, ...)
    local script = Graph:new()

    local loaded = script:loadGraphFromFile(scriptPath)
    if not loaded then
        error('Could not run graph, file does not exist: ' .. scriptPath)
    end
    script:resolveCompoundsAndReroutes()

    if flat.debug then
        local errors = script:validate()
        for i = 1, #errors do
            local error = errors[i]
            flat.ui.warn(makeErrorPathString(error) .. ': ' .. error.message)
        end

        if #errors > 0 then
            flat.dump(script:getDescription())
        end
    end

    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    return runner()
end