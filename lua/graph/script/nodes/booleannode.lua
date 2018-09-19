local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local CommonBooleanNode = flat.require 'graph/common/nodes/booleannode'

local BooleanNode = (CommonBooleanNode + FunctionalScriptNode):inherit 'Boolean'

function BooleanNode:execute(runtime)
    runtime:writePin(self.boolOutPin, self.value)
end

return BooleanNode