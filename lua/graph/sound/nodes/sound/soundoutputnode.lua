local SoundNode = flat.require 'graph/sound/soundnode'
local PinTypes = flat.require 'graph/pintypes'

local SoundOutputNode = SoundNode:inherit 'Sound Output'

function SoundOutputNode:addedToGraph(graph)
    graph.outputNode = self
end

function SoundOutputNode:getPinsToRead()
    return { self.soundInPin }
end

function SoundOutputNode:buildPins()
    self.soundInPin = self:addInputPin(flat.types['flat.audio.Chunk'], 'Sound')
end

function SoundOutputNode:execute(runtime)
    -- nothing to do, all the pins will be read from the connected pins' nodes
    assert(false)
end

return SoundOutputNode