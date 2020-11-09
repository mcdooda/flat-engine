local Distance = flat.eqs.test.Test:inherit 'Distance'
Distance.cost = flat.eqs.Cost.CONSTANT

function Distance:setupSettings(settings)
    self.fromContext = assert(settings.from)
    self.toContext   = assert(settings.to)
end

function Distance:computeItemScore(queryInstance, item)
    local fromPosition = queryInstance:getContextPosition2d(self.fromContext, item)
    local toPosition   = queryInstance:getContextPosition2d(self.toContext, item)
    return (toPosition - fromPosition):length()
end

flat.eqs.test.distance = Distance