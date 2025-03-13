CXX = g++
CXXFLAGS = -std=c++17 -I ~/rapidjson/include -lcurl

all: graph_crawler

graph_crawler: graph_crawler.cpp
	$(CXX) $(CXXFLAGS) graph_crawler.cpp -o graph_crawler

clean:
	rm -f graph_crawler
