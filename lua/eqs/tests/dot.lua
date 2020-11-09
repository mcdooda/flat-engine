local Dot = flat.eqs.test.Test:inherit 'Dot'
Dot.cost = flat.eqs.Cost.CONSTANT

function Dot:setupSettings(settings)
    self.line1 = assert(settings.line1)
    self.line2 = assert(settings.line2)
end

function Dot:computeItemScore(queryInstance, item)
    local line1 = self.line1(queryInstance, item)
    local line2 = self.line2(queryInstance, item)
    return line1:dot(line2)
end

function Dot.forward(context)
    return function(queryInstance, item)
        local entity = context(queryInstance, item)
        return entity:getForward():toVector2()
    end
end

function Dot.lineBetween(fromContext, toContext)
    return function(queryInstance, item)
        local fromPosition = queryInstance:getContextPosition2d(fromContext, item)
        local toPosition   = queryInstance:getContextPosition2d(toContext, item)
        return (toPosition - fromPosition):getNormalized()
    end
end

flat.eqs.test.dot = Dot