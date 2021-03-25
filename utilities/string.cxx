#include "string.hxx"

using namespace std;

vector<string> split(string_view topic, char splitter) {
    size_t pos = string::npos, nextPos = 0;
    auto result = vector<string>{};

    while((nextPos = topic.find(splitter, pos + 1)) != string::npos) {
        result.push_back((string)topic.substr(pos + 1, nextPos - pos - 1));
        pos = nextPos;
    }
    result.push_back((string)topic.substr(pos + 1, topic.size() - pos - 1));
    return result;
}

