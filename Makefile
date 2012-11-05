all:
	g++ tester.cpp -o test -std=c++0x -fmax-errors=10 -ftemplate-depth-2100 -fconstexpr-depth=10000 -std=c++0x -O2

constexpr:
	g++ static_hash_map.cpp -o constexpr -ftemplate-depth-2100 -fconstexpr-depth=10000 -std=c++0x -O2
clean:
	rm ./constexpr
