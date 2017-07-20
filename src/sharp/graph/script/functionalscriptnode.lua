local ScriptNode = require 'script/scriptnode'
local FunctionalScriptNodeRuntime = require 'script/functionalscriptnoderuntime'

local FunctionalScriptNode = ScriptNode:inherit()

function FunctionalScriptNode:getRuntimeType()
    return FunctionalScriptNodeRuntime
end

function FunctionalScriptNode:prepareReadPin(runtime, inputPin)
    self:execute(runtime, inputPin)
end

return FunctionalScriptNode