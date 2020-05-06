local Graph = flat.require 'graph/graph'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'

flat.graph.script = {}

function flat.graph.script.run(scriptPath, ...)
    local script = Graph:new()

    local loaded = script:loadGraphFromFile(scriptPath .. '.graph.lua')
    if not loaded then
        error('Could not run graph, file does not exist: ' .. scriptPath)
    end
    script:resolveCompounds()

    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    return runner()
end