local Enum = {}
Enum.__index = Enum

function Enum:toString(value)
    assert(
        1 <= value and value <= self._length,
        'value ' .. tostring(value) .. ' is outside of range [1, ' .. self._length .. ']'
    )
    for k, v in pairs(self) do
        if v == value and k ~= '_length' then
            return k
        end
    end
    error('no value ' .. tostring(value) .. ' in this enum')
end

function Enum:__len()
    return self._length
end

function flat.enum(...)
    local length = select('#', ...)
    local newEnum = { _length = length }
    for i = 1, length do
        newEnum[select(i, ...)] = i
    end
    return setmetatable(newEnum, Enum)
end

