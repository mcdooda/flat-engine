local function escapeRegex(str)
    return str:gsub('([%(%)%.%%%+%-%*%?%[%^%$])', '%%%1')
end

flat.escapeRegex = escapeRegex