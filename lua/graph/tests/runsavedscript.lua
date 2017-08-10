flat = {}
flat.require = require
flat.loadfile = function(f, env) loadfile(f, 'bt', env) end
flat.dofile = dofile
require = nil

flat.dofile 'enum.lua'

local function runSavedScript(scriptPath, ...)
    local Graph = flat.require 'graph/graph'
    local ScriptRuntime = flat.require 'graph/script/scriptruntime'
    
    local script = Graph:new()

    script:loadGraph(scriptPath)

    local scriptRuntime = ScriptRuntime:new(script)
    local runner = scriptRuntime:getRunner(...)
    runner()
end

runSavedScript(
    'graph/tests/savedscript.lua',

    3.14,
    'Hello world from script input'
)