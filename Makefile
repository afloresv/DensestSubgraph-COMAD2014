all: Solution1 Solution2 Solution3

Solution1:
	rm -f CharikarBatch
	c++ -O3 CharikarBatch.cc -o CharikarBatch

Solution2:
	rm -f Goldberg
	c++ -O3 Goldberg.cc -o Goldberg

Solution3:
	rm -f GoldbergBatch
	c++ -O3 GoldbergBatch.cc -o GoldbergBatch
