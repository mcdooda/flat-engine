local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local CommonNumberNode = flat.require 'graph/common/nodes/numbernode'

local NumberNode = (CommonNumberNode + FunctionalScriptNode):inherit 'Number'

function NumberNode:execute(runtime)
    runtime:writePin(self.numberOutPin, self.value)
end

return NumberNode