local math = math
local cos = math.cos
local sin = math.sin
local pi = math.pi
local pi2 = pi / 2
local pow = math.pow
local sqrt = math.sqrt

local easing = {
    easeIn = function(f)
        return f
    end,
    easeOut = function(f)
        return function(t)
            return 1 - f(1 - t)
        end
    end,
    easeInOut = function(f)
        return function(t)
            if t < 0.5 then
                return f(2 * t) / 2
            else
                return 0.5 + f(2 * t - 1) / 2
            end
        end
    end,

    lerp = function(x, a, b, f)
        return a * f(1 - x) + b * f(x)
    end,

    linear = function(t)
        return t
    end,
    sine = function(t)
        return 1 - cos(t * pi2)
    end,
    quad = function(t)
        return t * t
    end,
    cubic = function(t)
        return t * t * t
    end,
    quart = function(t)
        return t * t * t * t
    end,
    quint = function(t)
        return t * t * t * t * t
    end,
    expo = function(t)
        return t == 0 and 0 or 2 ^ (10 * (t - 1))
    end,
    circ = function(t)
        return 1 - sqrt(1 - t * t)
    end,
    back = function(t)
        return t * t * (2.70158 * t - 1.70158)
    end,
    elastic = function(t)
        return 2 ^ (-10 * t) * sin((t - 0.075) * (2 * pi) / 0.3) + 1
    end,
}

flat.easing = easing