local Command = {}

function Command:new()
	error('pure virtual function')
end

function Command:inherit(name)
	local inheritedClass = { name = name }
	inheritedClass.__index = inheritedClass
    return setmetatable(inheritedClass, { __index = self })
end

function Command:executeDo()
	error('pure virtual function')
end

function Command:executeUndo()
	error('pure virtual function')
end

return Command