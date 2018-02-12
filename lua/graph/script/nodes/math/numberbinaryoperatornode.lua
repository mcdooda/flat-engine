local BinaryOperatorTemplateNode = flat.require 'graph/script/nodes/math/binaryoperatortemplatenode'
local PinTypes = flat.require 'graph/pintypes'

local BinaryOperatorNode = BinaryOperatorTemplateNode(
    nil,
    flat.types.NUMBER, 'Operand 1',
    flat.types.NUMBER, 'Operand 2',
    flat.types.NUMBER, 'Result'
)

return BinaryOperatorNode