local BinaryLogicNode = flat.require 'graph/script/nodes/math/binarylogicnode'

local OrNode = BinaryLogicNode:inherit 'Or'

function OrNode.compute(a, b)
    return a or b
end

return OrNode