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

=============

MIT License

Copyright (c) 2016 Yufei (Benny) Chen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

