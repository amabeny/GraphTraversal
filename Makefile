CXX = g++
CXXFLAGS = -std=c++11 -I ~/rapidjson/include -lcurl

all: graph_crawler

graph_crawler: graph_crawler.cpp
	$(CXX) $(CXXFLAGS) graph_crawler.cpp -o graph_crawler

clean:
	rm -f graph_crawler
