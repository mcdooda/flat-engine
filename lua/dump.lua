local type = type
local pairs = pairs
local sortedPairs = flat.sortedPairs
local floor = math.floor

local function isInt(value)
	return type(value) == 'number' and value == floor(value)
end

local function dumpString(write, str)
	local escape = {
		['\a'] = '\\\a',
		['\b'] = '\\\b',
		['\f'] = '\\\f',
		['\n'] = '\\\n',
		['\r'] = '\\\r',
		['\t'] = '\\\t',
		['\v'] = '\\\v',
		['\''] = '\\\'',
	}
	for pattern, replacement in pairs(escape) do
		str = str:gsub(pattern, replacement)
	end
	write('\'', str, '\'')
end

local function isLuaName(str)
	assert(type(str) == 'string')
	return str:match('^[_a-zA-Z][_a-zA-Z0-9]*$')
end

local function dumpKeyString(write, str)
	if isLuaName(str) then
		write(str)
	else
		write '['
		dumpString(write, str)
		write ']'
	end
end

local function dump(write, value, tabs, allowAllTypes, seen, isIntKeyValue)
	if type(value) == 'table' then
		if seen[value] then
			if allowAllTypes then -- basically a debug output
				write(tabs, tostring(value) .. ' *RECURSIVE*')
				return
			else
				error('trying to serialize a recursive object')
			end
		end
		seen[value] = true
	end
	tabs = tabs or ''
	local t = type(value)
	if t == 'table' then
		if isIntKeyValue then
			write(tabs)
		end
		write('{\n')
		local length = #value
		for i = 1, length do
			dump(write, value[i], tabs .. '  ', allowAllTypes, seen, true)
			write ',\n'
		end
		for k, v in sortedPairs(value) do
			if not (isInt(k) and k <= length) then
				write(tabs, '  ')
				if type(k) == 'string' then
					dumpKeyString(write, k)
				else
					write '['
					dump(write, k, '', allowAllTypes, seen)
					write ']'
				end
				write ' = '
				local t1 = type(v)
				if t1 == 'string' or t1 == 'number' then
					dump(write, v, '', allowAllTypes, seen)
				else
					dump(write, v, tabs .. '  ', allowAllTypes, seen)
				end
				write ',\n'
			end
		end
		write(tabs, '}')
	elseif t == 'string' then
		write(tabs)
		dumpString(write, value)
	elseif t == 'number' then
		write(tabs, value)
	elseif t == 'boolean' then
		write(tabs, tostring(value))
	elseif t == 'nil' then
		write(tabs, 'nil')
	elseif allowAllTypes then
		write(tabs, tostring(value))
	else
		error('cannot serialize value ' .. tostring(value) .. ' of type ' .. t)
	end
end

local function dumpToString(value)
	local buffer = {}
	local bufferSize = 0
	local function addToBuffer(...)
		for i = 1, select('#', ...) do
			bufferSize = bufferSize + 1
			buffer[bufferSize] = select(i, ...)
		end
	end
	dump(addToBuffer, value, '', false, {})
	return table.concat(buffer)
end

local function dumpToOutput(output, value, allowAllTypes, seen)
	local function writeToOutput(...)
		output:write(...)
	end
	seen = seen or {}
	dump(writeToOutput, value, '', allowAllTypes, seen)
end

local function easyDump(value)
	dumpToOutput(io.output(), value, true)
end

local function dumpFlat(value)
	if type(value) == 'table' then
		print '{'
		local length = #value
		for i = 1, length do
			print('', tostring(value[i]) .. ',')
		end
		for k, v in sortedPairs(value) do
			if not (isInt(k) and k <= length) then
				print('', tostring(k), '=', tostring(v) .. ',')
			end
		end
		print '}'
	else
		print(tostring(value))
	end
end

flat.dump = easyDump
flat.dumpFlat = dumpFlat
flat.dumpToString = dumpToString
flat.dumpToOutput = dumpToOutput
