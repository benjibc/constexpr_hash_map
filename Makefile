all:
	g++ tester.cpp -o constexpr -ftemplate-depth-2100 -fconstexpr-depth=10000 -std=c++0x -O2
clean:
	rm ./constexpr
