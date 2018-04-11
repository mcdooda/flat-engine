local assert = assert

local coroutine = coroutine
local cocreate = coroutine.create
local coresume = coroutine.resume
local costatus = coroutine.status

local function task(f)
    return function()
        local co = cocreate(f)
        local function fakeThread()
            local timer = Timer.new()
            timer:onEnd(function()
                assert(coresume(co))
                if costatus(co) ~= 'dead' then
                    fakeThread()
                end
            end)
            timer:start(0)
        end
        fakeThread()
        return true
    end
end

flat.ui.task = task