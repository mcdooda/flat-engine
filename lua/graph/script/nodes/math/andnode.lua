local BinaryLogicNode = flat.require 'graph/script/nodes/math/binarylogicnode'

local AndNode = BinaryLogicNode:inherit 'And'

function AndNode.compute(a, b)
    return a and b
end

return AndNode