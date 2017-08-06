local function runSavedScript(scriptPath, ...)
    local Graph = require 'graph'
    local ScriptRuntime = require 'script/scriptruntime'
    
    local script = Graph:new()

    script:loadGraph(scriptPath)

    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    runner()
end

runSavedScript(
    'tests/savedscript.lua',

    3.14,
    'Hello world from script input'
)