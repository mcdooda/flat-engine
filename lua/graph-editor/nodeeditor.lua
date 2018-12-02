local NodeEditor = {}

function NodeEditor:inherit()
    return setmetatable({}, { __index = self })
end

return NodeEditor