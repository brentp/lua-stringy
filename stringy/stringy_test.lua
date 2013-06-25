stringy = require "stringy"

---------
-- split
---------
b = stringy.split("asdfsk", "s")
assert(#b == 3, #b)

assert('a|b|c' == (table.concat(stringy.split("a\tb\tc", "\t"), "|")))
assert(stringy.split("asdf", "s")[1] == "a")

assert('a||b||c' == (table.concat(stringy.split("aXXbXXc", "XX"), "|")))

---------
-- strip
---------
assert(stringy.strip(" asdf ") == "asdf")
assert(stringy.strip("asdf ") == "asdf")
assert(stringy.strip(" asdf  ") == "asdf")
assert(stringy.strip("asdf") == "asdf")
assert(stringy.strip("asdf") == "asdf")
assert(stringy.strip("\nasdf\r\n") == "asdf")
assert(stringy.strip(" asdf ") == "asdf")

--------------
-- startswith
--------------
assert(stringy.startswith("asdf", "a"))
assert(stringy.startswith("asdf", "asd"))
assert(stringy.startswith("asdf", "asdf"))
assert(stringy.startswith("asdf", ""))
assert(stringy.startswith("\0asdf", "\0"))
assert(stringy.startswith("\000asdf", "\000a"))

assert(not stringy.startswith("asdf", "asdfg"))
assert(not stringy.startswith("asdf", "b"))
assert(not stringy.startswith("asdf", "f"))
assert(not stringy.startswith("asdf", "\0"))

--------------
-- endswith
--------------
assert(stringy.endswith("asdf", "f"))
assert(stringy.endswith("asdf", "sdf"))
assert(stringy.endswith("asdf", "asdf"))
assert(stringy.endswith("asdf", "asdf"))
assert(stringy.endswith("asdf", ""))
assert(stringy.endswith("a\0df", "\0df"))

assert(not stringy.endswith("asdf", "\0"))
assert(not stringy.endswith("asdf", "asd"))
assert(not stringy.endswith("asdf", "a"))
assert(not stringy.endswith("a\0df", "a\0s"))

-------
--count
-------

assert(stringy.count("aaa", "a") == 3)
-- counts non-overlapping!
assert(stringy.count("aaa", "aa") == 1)
assert(stringy.count("aaa", "") == 0)
assert(stringy.count("aaa", "b") == 0)
assert(stringy.count("a\0a", "\0") == 1)

assert(stringy.count("aaa", "aaa") == 1)
assert(stringy.count("aaa", "aaaa") == 0)
assert(stringy.count("", "aaaa") == 0)

-- start arg.
-- index at 2, leaves "aaa"
assert(stringy.count("aaaa", "a", 2) == 3)
-- index at 1, leaves "aaa"
assert(stringy.count("aaaa", "a", 1) == 4)

-- index at 2, leaves "aaa" then only take to 3
assert(stringy.count("aaaa", "a", 2, 3) == 1)
-- start at 2 from end.
assert(stringy.count("aaaa", "a", -2) == 2)
assert(stringy.count("bbaa", "a", -2, -1) == 1)
assert(stringy.count("bbba", "a", -1) == 1)

--------
-- find
-------
assert(stringy.find("baaaa", "a") == 2)

assert(stringy.find("baaaa", "baaaa") == 1)
assert(stringy.find("baaaa", "baaaa", 1) == 1)

assert(stringy.find("baaaa", "baaaa", 2) == nil)

assert(stringy.find("abcdef", "b", 3) == nil)

assert(stringy.find("abcdef", "c", 3) == 3)
assert(stringy.find("abcdef", "c", 4) == nil)
assert(stringy.find("abcdef", "c", -1) == nil)
