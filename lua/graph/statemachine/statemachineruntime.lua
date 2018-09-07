local StateMachineRuntime = {}
StateMachineRuntime.__index = StateMachineRuntime

function StateMachineRuntime:new(graph)
    local o = {
        graph = graph
    }

    return setmetatable(o, self)
end

return StateMachineRuntime