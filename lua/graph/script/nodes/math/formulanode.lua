local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local FormulaNode = FunctionalScriptNode:inherit 'Formula'

function FormulaNode:buildPins()
    self.pinsByName = {}
    if self.formula:isValid() then
        local variableNames = self.formula:getVariableNames()
        for i = 1, #variableNames do
            local pinName = variableNames[i]
            self.pinsByName[pinName] = self:addInputPin(flat.types.NUMBER, pinName)
        end
    end
    self.resultOutPin = self:addOutputPin(flat.types.NUMBER, '')
end

function FormulaNode:updateInputPins()
    if self.formula:isValid() then
        local previousPinsByName = self.pinsByName
        self.pinsByName = {}
        local variableNames = self.formula:getVariableNames()
        for i = 1, #variableNames do
            local pinName = variableNames[i]
            local inputPin = previousPinsByName[pinName]
            if inputPin then
                previousPinsByName[pinName] = nil
                flat.arrayAdd(self.inputPins, inputPin)
            else
                inputPin = self:addInputPin(flat.types.NUMBER, pinName)
            end
            self.pinsByName[pinName] = inputPin
        end
        for pinName, inputPin in pairs(previousPinsByName) do
            self:unplugInputPin(inputPin)
            self:removeInputPin(inputPin)
        end
    end
end

function FormulaNode:execute(runtime)
    assert(self.formula:isValid())
    local variableValues = {}
    for variableName, inputPin in pairs(self.pinsByName) do
        variableValues[variableName] = runtime:readPin(inputPin)
    end
    local result = self.formula:evaluate(variableValues)
    runtime:writePin(self.resultOutPin, result)
end

function FormulaNode:setFormula(formula)
    assert(flat.type(formula) == flat.types.STRING, 'Wrong type for setFormula: string expected, got ' .. flat.typetostring(flat.type(formula)))
    self.formula = flat.Formula:new(formula)
end

function FormulaNode:getFormula()
    return self.formula:getCode()
end

function FormulaNode:init()
    self:setFormula ''
end

function FormulaNode:getLoadArguments()
    return self:getFormula()
end

FormulaNode.load = FormulaNode.setFormula

return FormulaNode