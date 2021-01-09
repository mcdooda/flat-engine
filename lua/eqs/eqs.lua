local QueryInstance = {}
QueryInstance.__index = QueryInstance

local QueryTemplate = {}
QueryTemplate.__index = QueryTemplate

local Test = {}
Test.__index = Test

local Cost = flat.enum(
    'CONSTANT',
    'LINEAR',
    'EXPONENTIAL'
)

-- query instance
function QueryInstance:new(template, querier)
    return setmetatable({
        template          = template,
        querier           = querier,
        items             = nil,
        itemScoresPerTest = {},
        itemFinalScores   = {}
    }, self)
end

function QueryInstance:run(items, runMode)
    self:setItems(items)
    local tests = self.template:getTests()
    for i = 1, #tests do
        local test = tests[i]
        test:prepareContext()
        local minScore = math.huge
        local maxScore = -math.huge
        for itemIndex = #items, 1, -1 do
            local item = items[itemIndex]
            local score = test:computeItemScore(self, item)
            score = test:processCommonScoreFilters(score)
            if score then
                minScore = math.min(minScore, score)
                maxScore = math.max(maxScore, score)
                self:setItemScore(test, item, score)
            else
                self:discardItem(test, itemIndex)
            end
        end
        local scoringFactor = test:getScoringFactor()
        local scoringFunction = test:getScoringFunction()
        for itemIndex = 1, #items do
            local item = items[itemIndex]
            self:normalizeItemScore(test, item, minScore, maxScore, scoringFactor, scoringFunction)
        end
    end
    self:computeFinalScores()
    return runMode.computeFinalResult(self)
end

function QueryInstance:setItems(items)
    self.items = items
    for i = 1, #items do
        local item = items[i]
        self.itemScoresPerTest[item] = {}
    end
end

function QueryInstance:setItemScore(test, item, score)
    assert(score == score)
    self.itemScoresPerTest[item][test] = score
end

function QueryInstance:discardItem(test, itemIndex)
    local item = self.items[itemIndex]
    self.itemScoresPerTest[item] = nil
    flat.arrayRemoveIndexCyclic(self.items, itemIndex)
end

function QueryInstance:normalizeItemScore(test, item, minScore, maxScore, scoringFactor, scoringFunction)
    local absoluteScore = self.itemScoresPerTest[item][test]
    if minScore < maxScore then
        local normalizedScore = scoringFunction((absoluteScore - minScore) / (maxScore - minScore)) * scoringFactor
        assert(normalizedScore == normalizedScore)
        self.itemScoresPerTest[item][test] = normalizedScore
    else
        -- all items have the same score for this test, thus it does not matter and we can set it to 0
        self.itemScoresPerTest[item][test] = 0
    end
end

function QueryInstance:getItemFinalScores()
    return self.itemFinalScores
end

function QueryInstance:computeFinalScores()
    local items = self.items
    for i = 1, #items do
        local item = items[i]
        local itemScoresPerTest = self.itemScoresPerTest[item]
        local itemFinalScore = 0
        for test, score in pairs(itemScoresPerTest) do
            itemFinalScore = itemFinalScore + score
        end
        assert(itemFinalScore == itemFinalScore)
        flat.arrayAdd(self.itemFinalScores, {
            item  = item,
            score = itemFinalScore
        })
    end
end

function QueryInstance:getContextPosition2d(context, item)
    local contextObject = context(self, item)
    local contextObjectType = flat.type(contextObject)
    if contextObjectType == flat.types['flat.Vector2'] then
        return contextObject
    elseif contextObjectType == flat.types['flat.Vector3'] then
        return contextObject:toVector2()
    elseif contextObjectType == flat.types['CG.Entity'] then -- TODO remove this from flat
        return contextObject:getBaseCenter():toVector2()
    end
    error('Cannot find position 2d for object \'' .. tostring(contextObject) .. '\' of type ' .. flat.typetostring(contextObjectType))
end

function QueryInstance:getQuerier()
    return self.querier
end

-- test
function Test:new(scoringFactor, scoringFunction)
    assert(scoringFactor)
    assert(scoringFunction)
    return setmetatable({
        scoringFactor   = scoringFactor,
        scoringFunction = scoringFunction
    }, self)
end

function Test:makeInstance(scoringFactor, scoringFunction, settings)
    local test = self:new(scoringFactor, scoringFunction)
    test:setupCommonSettings(settings)
    test:setupSettings(settings)
    return test
end

function Test:inherit(name)
    local testType = {
        name = name
    }
    testType.__index = testType
    return setmetatable(testType, { __index = self, __call = self.makeInstance })
end

function Test:getName()
    assert(self.name, 'the test has no name')
    return self.name
end

function Test:getScoringFactor()
    return self.scoringFactor
end

function Test:getScoringFunction()
    return self.scoringFunction
end

function Test:getIsFiltering()
    return self.isFiltering
end

function Test:getCost()
    return self.cost
end

function Test:setupParams()

end

function Test:prepareContext()
    
end

function Test:computeItemScore(queryInstance, item)
    error('Test:computeItemScore is a pure virtual function')
end

function Test:processCommonScoreFilters(score)
    if not score then
        return nil
    elseif self.min and score < self.min then
        return nil
    elseif self.max and score > self.max then
        return nil
    else
        return score
    end
end

function Test:setupCommonSettings(settings)
    if not settings then
        return
    end

    self.min = settings.min
    self.max = settings.max
    assert(not self.min or not self.max or self.min < self.max)

    self.isFiltering = self.min or self.max
end

function Test:setupSettings(settings)

end

local test = {}
test.Test = Test

-- run mode
local RunMode = {}

do
    RunMode.SingleBestItem = {}
    function RunMode.SingleBestItem.computeFinalResult(queryInstance)
        local bestScore = -math.huge
        local bestScoreIndex
        local itemFinalScores = queryInstance:getItemFinalScores()
        for i = 1, #itemFinalScores do
            local itemFinalScore = itemFinalScores[i]
            if itemFinalScore.score > bestScore then
                bestScore = itemFinalScore.score
                bestScoreIndex = i
            end
        end
        if bestScoreIndex then
            return itemFinalScores[bestScoreIndex]
        else
            return nil
        end
    end
end

do
    RunMode.AllMatching = {}
    local function compareItems(a, b)
        return a.score < b.score
    end
    function RunMode.AllMatching.computeFinalResult(queryInstance)
        local itemFinalScores = queryInstance:getItemFinalScores()
        table.sort(itemFinalScores, compareItems)
        return itemFinalScores
    end
end

-- scoring functions
local scoring = {}

function scoring.linear(score)
    return score
end

function scoring.inverseLinear(score)
    return 1 - score
end

-- context
local context = {}

function context.querier(queryInstance, item)
    return queryInstance:getQuerier()
end

function context.item(queryInstance, item)
    return item
end

-- template
function QueryTemplate:new(generator, tests)
    return setmetatable({
        generator = generator,
        tests     = tests
    }, self)
end

function QueryTemplate:sortTests()
    table.sort(self.tests, function(a, b)
        local aIsFiltering = a:getIsFiltering()
        local bIsFiltering = b:getIsFiltering()
        if aIsFiltering ~= bIsFiltering then
            return aIsFiltering
        else
            return a:getCost() < b:getCost()
        end
    end)
end

function QueryTemplate:generateItems(queryInstance)
    return self.generator(queryInstance)
end

function QueryTemplate:getTests()
    return self.tests
end

-- public api
local function run(template, querier, runMode)
    local queryInstance = QueryInstance:new(template, querier)
    local items = template:generateItems(queryInstance)
    return queryInstance:run(items, runMode)
end

local function template(templateSettings)
    local generator = assert(templateSettings.generator)
    local tests = assert(templateSettings.tests)
    local queryTemplate = QueryTemplate:new(generator, tests)
    queryTemplate:sortTests()
    return queryTemplate
end

flat.eqs = {
    run      = run,
    template = template,
    scoring  = scoring,
    context  = context,
    test     = test,
    RunMode  = RunMode,
    Cost     = Cost
}