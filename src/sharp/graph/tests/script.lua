local Graph = require 'graph'
local ScriptRuntime = require 'script/scriptruntime'
local InitNode = require 'script/nodes/initnode'
local TestNode = require 'script/nodes/testnode'
local NumberNode = require 'script/nodes/numbernode'
local StringNode = require 'script/nodes/stringnode'

local script = Graph:new()

local init = script:addNode(InitNode)

local number = script:addNode(NumberNode)
number:setValue(3.14)

local string = script:addNode(StringNode)
string:setValue 'Hello World'

local test = script:addNode(TestNode)
init:plugPins(init.impulseOutPin, test, test.impulseInPin)

number:plugPins(number.numberOutPin, test, test.numberInPin)

string:plugPins(string.stringOutPin, test, test.stringInPin)

local scriptRuntime = ScriptRuntime:new(script)
local runner = scriptRuntime:getRunner()
runner()