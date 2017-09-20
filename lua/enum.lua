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

function Enum:__len()
    return self.length
end

function flat.enum(...)
    local length = select('#', ...)
    local newEnum = { length = length }
    for i = 1, length do
        newEnum[select(i, ...)] = i
    end
    return setmetatable(newEnum, Enum)
end

