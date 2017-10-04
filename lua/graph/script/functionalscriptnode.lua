local ScriptNode = flat.require 'graph/script/scriptnode'

local FunctionalScriptNode = ScriptNode:inherit()

function FunctionalScriptNode:prepareReadPin(runtime, outputPin)
    self:execute(runtime, inputPin)
end

return FunctionalScriptNode