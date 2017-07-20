local Enum = {}
Enum.__index = Enum

function Enum:toString(value)
    for k, v in pairs(self) do
        if v == value then
            return k
        end
    end
    error('no value ' .. value .. ' in this enum')
end

local function enum(...)
    local newEnum = {}
    for i = 1, select('#', ...) do
        newEnum[select(i, ...)] = i
    end
    return setmetatable(newEnum, Enum)
end

return enum