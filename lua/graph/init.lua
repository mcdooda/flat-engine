local Graph = flat.require 'graph/graph'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'

flat.node = flat.node or {}
flat.node.editor = {}

function flat.node.runScript(scriptPath, ...)
    local script = Graph:new()

    script:loadGraph(scriptPath .. '.graph.lua')

    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    runner()
end

