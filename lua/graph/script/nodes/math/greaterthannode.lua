local ComparisonOperatorNode = flat.require 'graph/script/nodes/math/comparisonoperatornode'

local GreaterThanNode = ComparisonOperatorNode:inherit 'Greater Than'

function GreaterThanNode.compute(operand1, operand2)
    return operand1 > operand2
end

return GreaterThanNode