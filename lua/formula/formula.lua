local load = load

local Formula = {}
Formula.__index = Formula

-- fake env used to fetch variable names
local fakeNumber = {}
local function fakeFunction()
    return fakeNumber
end
setmetatable(fakeNumber, {
    __add = fakeFunction,
    __sub = fakeFunction,
    __mul = fakeFunction,
    __div = fakeFunction,
    __mod = fakeFunction,
    __pow = fakeFunction,
    __unm = fakeFunction,
    __idiv = fakeFunction
})
local fakeEnv = {}
for k, v in pairs(math) do
    fakeEnv[k] = fakeFunction
end
local variableNames
local getVariableNamesMt = {
    __index = function(env, variableName)
        local found = false
        for i = 1, #variableNames do
            if variableNames[i] == variableName then
                found = true
                break
            end
        end
        if not found then
            flat.arrayAdd(variableNames, variableName)
        end
        return fakeNumber
    end
}
setmetatable(fakeEnv, getVariableNamesMt)

function Formula:new(code)
    local o = setmetatable({
        code           = code,
        compiledChunk  = nil,
        variableNames  = nil,
        constantResult = nil
    }, self)

    if o:fetchVariableNames() then
        o:compile()
        if #o:getVariableNames() == 0 then
            o:computeConstantResult()
        end
    end

    return o
end

function Formula:getCode()
    return self.code
end

function Formula:getVariableNames()
    return self.variableNames
end

function Formula:evaluate(variableValues)
    setmetatable(self.env, { __index = variableValues })
    return self.compiledChunk()
end

function Formula:isValid()
    return self.compiledChunk ~= nil
end

function Formula:fetchVariableNames()
    variableNames = {}
    local compiledChunk = load(self:getFinalCode(), 'Formula: ' .. self.code, 't', fakeEnv)
    if not compiledChunk then
        return false
    end
    pcall(compiledChunk)
    self.variableNames = variableNames
    variableNames = nil
    return true
end

function Formula:compile()
    local env = {}
    for k, v in pairs(math) do
        env[k] = v
    end
    self.compiledChunk = load(self:getFinalCode(), 'Formula: ' .. self.code, 't', env)
    self.env = env
end

function Formula:getFinalCode()
    -- add parentheses to forbid several return values
    return 'return (' .. self.code .. ')'
end

function Formula:isConstant()
    assert((#self.variableNames == 0) == (self.constantResult ~= nil))
    return self.constantResult ~= nil
end

function Formula:computeConstantResult()
    assert(#self.variableNames == 0)
    self.constantResult = self:evaluate{}
end

function Formula:getConstantResult()
    return assert(self.constantResult)
end

flat.Formula = Formula