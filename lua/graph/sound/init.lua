local Graph = flat.require 'graph/graph'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'

flat.graph.sound = {}

function flat.graph.sound.play(soundPath)
    local chunk = flat.graph.script.run(soundPath)
    flat.audio.playChunk(chunk)
end