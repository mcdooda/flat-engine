local InnerGraphNodeEditor = flat.require 'graph-editor/innergraphnode'

local StateMachineNodeEditor = setmetatable({}, { __index = InnerGraphNodeEditor })

return StateMachineNodeEditor