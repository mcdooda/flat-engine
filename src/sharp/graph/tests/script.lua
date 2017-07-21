local Graph = require 'graph'
local ScriptRuntime = require 'script/scriptruntime'
local InitNode = require 'script/nodes/initnode'
local TestNode = require 'script/nodes/testnode'
local NumberNode = require 'script/nodes/numbernode'
local StringNode = require 'script/nodes/stringnode'
local MultiplyNode = require 'script/nodes/math/multiplynode'

local script = Graph:new()

local init = script:addNode(InitNode)



local test1 = script:addNode(TestNode)
do
    local number = script:addNode(NumberNode)
    number:setValue(3.14)
    number:plugPins(number.numberOutPin, test1, test1.numberInPin)
end
do
    local string = script:addNode(StringNode)
    string:setValue 'Hello World'
    string:plugPins(string.stringOutPin, test1, test1.stringInPin)
end
init:plugPins(init.impulseOutPin, test1, test1.impulseInPin)

local multiply = script:addNode(MultiplyNode)
test1:plugPins(test1.numberOutPin, multiply, multiply.operand1InPin)
do
    local number = script:addNode(NumberNode)
    number:setValue(2)
    number:plugPins(number.numberOutPin, multiply, multiply.operand2InPin)
end

local test2 = script:addNode(TestNode)
multiply:plugPins(multiply.resultOutPin, test2, test2.numberInPin)
test1:plugPins(test1.stringOutPin, test2, test2.stringInPin)
test1:plugPins(test1.impulseOutPin, test2, test2.impulseInPin)

local scriptRuntime = ScriptRuntime:new(script)
local runner = scriptRuntime:getRunner()
runner()