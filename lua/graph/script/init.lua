local Graph = flat.require 'graph/graph'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'

flat.graph.script = {}

function flat.graph.script.run(scriptPath, ...)
    local script = Graph:new()

    script:loadGraph(scriptPath .. '.graph.lua')
    script:resolveCompounds()

    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    return runner()
end