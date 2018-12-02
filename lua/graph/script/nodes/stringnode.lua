local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local CommonStringNode = flat.require 'graph/common/nodes/stringnode'

local StringNode = (CommonStringNode + FunctionalScriptNode):inherit 'String'

function StringNode:execute(runtime)
    runtime:writePin(self.stringOutPin, self.value)
end

return StringNode