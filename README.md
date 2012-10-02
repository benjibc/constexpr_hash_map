constexpr_hash_map
===============

Constexpr hash map, generates a constant KV hash map at compile time.

This is part of the bigger project that I am working on, called Basic Model
View Controller, an HMVC strutured web framework based on GWAN.

There was a need for really fast routing for the controller functions of the
framework, which requires RTTI and a universal router for the framework. The
RTTI system was reasonably easy to build, but the routing was the major
overhead for the framework because GWAN treats C++ source code as scripts,
compiles it every single time before each visit (GWAN does cache the script,
but routing is still a fairly big overhead becuase all of the unordered_map
will require some sort of heap dynamic allocation.

I was considering to build a const hashmap until I came across constexpr, which
enables things to be evaluated at compile time, so the hash map will be copied
into memory while the program is initialized, minimizing the overhead for 
hash_map creation. This is useful also for rtti system solution if one does
not desire QObject that comes with a rather large overhead which aims to solve
a lot of problems.
