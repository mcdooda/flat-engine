local CommandHistory = {}
CommandHistory.__index = CommandHistory

function CommandHistory:new()
	local o = setmetatable({
        stack   = {},
        index   = 0
    }, self)
	return o
end

function CommandHistory:push(command, execute)
	execute = execute == nil or execute
	flat.stackPop(self.stack, #self.stack - self.index)
	flat.stackPush(self.stack, command)
	if execute then
		command:executeDo()
	end
	self.index = self.index + 1
end

function CommandHistory:setIndex(index)
	if index < self.index then
		self:undo(self.index - index)
	elseif index > self.index then
		self:redo(index - self.index)
	end
end

function CommandHistory:undo(num)
	num = num or 1
	assert(self.index - num >= 0)
	for i = self.index, self.index - num + 1, -1 do
		local command = self.stack[i]
		command:executeUndo()
	end
	self.index = math.max(self.index - num, 0)
end

function CommandHistory:redo(num)
	num = num or 1
	assert(self.index + num <= #self.stack)
	for i = math.max(self.index, 1), self.index + num do
		local command = self.stack[i]
		command:executeDo()
	end
	self.index = self.index + num
end

return CommandHistory