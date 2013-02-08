constexpr_hash_map
===============
# please make sure you compile this code with the latest GCC
Constexpr hash map, generates a constant KV hash map at compile time.

This is part of the bigger project that I am working on, called Basic Model
View Controller, an HMVC strutured web framework based on Node.js.

The objective for this project is to move the construction time of a hashmap
to run-time, at the cost of compile time. The code uses a lot of template
programming as well as lots of c++11 features such as constant expression,
decltype, variadic template.

This code is inspired by D.Abraham and partly based on his github gist.
Please visit https://gist.github.com/1457531 

I was considering to build a const hashmap until I came across constexpr, which
enables things to be evaluated at compile time, so the hash map will be copied
into memory while the program is initialized, minimizing the overhead for 
hash_map creation. This is useful also for rtti system solution if one does
not desire QObject that comes with a rather large overhead which aims to solve
a lot of problems
