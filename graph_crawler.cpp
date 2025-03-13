#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <curl/curl.h>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;

const string BASE_URL = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

// function to handle libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}
string format_node_name(string node);

// to fetch neighbors 
set<string> get_neighbors(const string& node) {
    CURL* curl = curl_easy_init();
    string url = BASE_URL + format_node_name(node);
    string response;

    // debug line
    cout << "Fetching URL: " << url << endl;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

// only for debugging
cout << "Raw API Response for " << formatted_node << ": " << response << endl;
    
// JSON 
    set<string> neighbors;
    Document doc;
    if (doc.Parse(response.c_str()).HasParseError()) {
        cerr << "Error parsing JSON response" << endl;
        cerr << "Raw response: " << response << endl; // debug line
        return neighbors;
    }

    if (doc.HasMember("neighbors") && doc["neighbors"].IsArray()) {
        for (const auto& neighbor : doc["neighbors"].GetArray()) {
            neighbors.insert(neighbor.GetString());
        }
    }

    return neighbors;
}

// bfs function
void bfs(const string& start_node, int max_depth) {
    queue<pair<string, int>> q;
    set<string> visited;

    q.push({start_node, 0});
    visited.insert(start_node);

    while (!q.empty()) {
        auto [node, depth] = q.front();
        q.pop();
        cout << "Visited: " << node << " (Depth: " << depth << ")" << endl;

        if (depth < max_depth) {
            set<string> neighbors = get_neighbors(node);
            
            // added to help debug
            cout << "Neighbors of " << node << ": ";
            for (const auto& n : neighbors) cout << n << " ";
            cout << endl;
            
            for (const string& neighbor : neighbors) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push({neighbor, depth + 1});
                    cout << "Adding " << neighbor << " to queue at depth " << (depth + 1) << endl;
                }
            }
        }
    }
}
string format_node_name(string node) {
    for (char &c : node) {
        if (c == ' ') c = '_';
    }
    return node;
}
// main 
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <start_node> <depth>" << endl;
        return 1;
    }

    string start_node = argv[1];
    int depth = stoi(argv[2]);

     // debug line
    cout << "Formatted node name: " << format_node_name(start_node) << endl;

    bfs(start_node, depth);
    return 0;
}
