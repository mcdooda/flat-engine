local ScriptNode = require 'script/scriptnode'

local FunctionalScriptNode = ScriptNode:inherit()

function FunctionalScriptNode:prepareReadPin(runtime, inputPin)
    self:execute(runtime, inputPin)
end

return FunctionalScriptNode