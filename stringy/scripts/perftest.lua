require "stringy"

local function perftest(fname, f, strings, keys)
  local time = os.clock  -- os.time or os.clock
  local t1 = time()
  io.stdout:write(string.format("%-12s", fname))
  for _, s in ipairs(strings) do
      if keys then
          for i=1,100000 do
            f(s, "zzz")f(s, "za b c")f(s, "h r s")f(s, "2 3 4")f(s, "99999") --f(s, "zzzzzz")
          end
      else
          for i=1,100000 do
            f(s)f(s)f(s)f(s)f(s)f(s)f(s)f(s)f(s)f(s)
          end
      end
      local dt = time() - t1
      io.stdout:write(string.format("\t%4.1f", dt))
  end
  io.stdout:write("\n")
end

s1 = "  a b c d e f g h i j k l m n o p q r s t u v w x y z A B C "
s1 = s1 .. s1
s2 = "1234"

local stringy_strip = stringy.strip

local match = string.match
local trim7 = function(s)
  return match(s,'^()%s*$') and '' or match(s,'^%s*(.*%S)')
end

perftest("stringy.strip", stringy_strip, {s1, s2})
perftest("wiki/trim7", trim7, {s1, s2})

local stringy_find = stringy.find
local string_find = string.find

for i = 1, 6 do
    s1 = s1 .. s1
end
s1 = s1 .. "zzzzzz"
perftest("stringy.find", stringy_find, {s1, s2}, true)
perftest("string.find", string_find, {s1, s2}, true)

print(string_find(s1, "a"))
