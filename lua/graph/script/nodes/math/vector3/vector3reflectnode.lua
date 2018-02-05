local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local Vector3ReflectNode = FunctionalScriptNode:inherit 'Vector3 Reflect'

function Vector3ReflectNode:buildPins()
    self.incidentInPin = self:addInputPin(flat.types['flat.Vector3'], 'Incident')
    self.normalInPin = self:addInputPin(flat.types['flat.Vector3'], 'Normal')

    self.vector3OutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Result')
end

function Vector3ReflectNode:execute(runtime)
    local incident = runtime:readPin(self.incidentInPin)
    local normal = runtime:readPin(self.normalInPin)

    local result = incident:reflect(normal)

    runtime:writePin(self.vector3OutPin, result)
end

return Vector3ReflectNode