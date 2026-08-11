#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

// g++ -g -O3 -march=native -Wall -pedantic -o SubStringSort SubstringSort.cpp

constexpr unsigned int L = 1 << 18;
constexpr unsigned int N = 1 << 15;

bool compare(const char *s1, const char *s2);

int
main()
{
	std::unique_ptr<char[]> s(new char[L]);
	std::vector<const char *> vs(N);

	// prepare the string and sub string pointers
	{
		std::minstd_rand rgen;
		std::memset(s.get(), 'a', N * sizeof(char));
		for (unsigned int i = 0; i < L / 1024; ++i)
		{
			s[rgen() % (L - 1)] = 'a' + (rgen() % ('z' - 'a' + 1));
		}
		s[L - 1] = 0;
		for (unsigned int i = 0; i < N; ++i)
		{
			vs[i] = &s[rgen() % (L - 1)];
		}
	}

	std::size_t count = 0;
	auto t1 = std::chrono::system_clock::now();
	std::sort(vs.begin(),
			  vs.end(),
			  [&](const char *a, const char *b)
			  {
				  ++count;
				  return compare(a, b);
			  });
	auto t2 = std::chrono::system_clock::now();
	std::cout << "Sort time: "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms ("
			  << count << " comparisons)" << std::endl;

	return 0;
}

bool
compare(const char *s1, const char *s2)
{
	if (s1 == s2)
	{
		return false;
	}

	for (int i1 = 0, i2 = 0;; ++i1, ++i2)
	{
		if (s1[i1] != s2[i2])
		{
			return s1[i1] > s2[i2];
		}
	}

	return false;
}
