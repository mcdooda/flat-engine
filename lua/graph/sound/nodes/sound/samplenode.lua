local SoundNode = flat.require 'graph/sound/soundnode'
local PinTypes = flat.require 'graph/pintypes'

local SampleNode = SoundNode:inherit 'Sample'

function SampleNode:buildPins()
    self.sampleNameInPin = self:addInputPin(flat.types.STRING, 'Sample Name')

    self.soundOutPin = self:addOutputPin(flat.types['flat.audio.Chunk'], 'Sample')
end

function SampleNode:execute(runtime)
    print('SampleNode execute')
    if not self.sample then
        local sampleName = runtime:readPin(self.sampleNameInPin)
        self:loadSample(sampleName)
    end
    runtime:writePin(self.soundOutPin, self.sample)
end

function SampleNode:postLoad()
    local sampleName = self:tryReadConstantPin(self.sampleNameInPin)
    if sampleName then
        self:loadSample(sampleName)
    end
end

function SampleNode:loadSample(sampleName)
    print('SampleNode: ' .. sampleName)
    local samplePath = Mod.getFilePath('samples/' .. sampleName)
    self.sample = flat.audio.loadChunk(samplePath)
end

return SampleNode