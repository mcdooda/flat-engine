local ComparisonOperatorNode = flat.require 'graph/script/nodes/math/comparisonoperatornode'

local LessThanNode = ComparisonOperatorNode:inherit 'Less Than'

function LessThanNode.compute(operand1, operand2)
    return operand1 < operand2
end

return LessThanNode