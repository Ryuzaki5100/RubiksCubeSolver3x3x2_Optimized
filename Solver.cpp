#include <bits/stdc++.h>
using namespace std;
typedef vector<int> edgePos;
typedef vector<int> cornerPos;
typedef pair<edgePos, cornerPos> Cube;
map<char, vector<int>> nextCornerPos = {{'R', {0, 6, 5, 3, 4, 2, 1, 7}},
                                        {'U', {1, 2, 3, 0, 4, 5, 6, 7}},
                                        {'F', {0, 1, 7, 6, 4, 5, 3, 2}},
                                        {'B', {5, 4, 2, 3, 1, 0, 6, 7}},
                                        {'L', {7, 1, 2, 4, 3, 5, 6, 0}},
                                        {'D', {0, 1, 2, 3, 7, 4, 5, 6}}};
map<char, vector<int>> nextEdgePos = {{'R', {0, 5, 2, 3, 4, 1, 6, 7}},
                                      {'U', {1, 2, 3, 0, 4, 5, 6, 7}},
                                      {'F', {0, 1, 6, 3, 4, 5, 2, 7}},
                                      {'B', {4, 1, 2, 3, 0, 5, 6, 7}},
                                      {'L', {0, 1, 2, 7, 4, 5, 6, 3}},
                                      {'D', {0, 1, 2, 3, 7, 4, 5, 6}}};
vector<int> SOLVED_EDGE_POS = {0, 1, 2, 3, 4, 5, 6, 7}, SOLVED_CORNER_POS = {0, 1, 2, 3, 4, 5, 6, 7};
void printStatus(Cube c)
{
    cout << "{" << c.first[0];
    for (int i = 1; i < 8; i++)
        cout << "," << c.first[i];
    cout << "}\t";
    cout << "{" << c.second[0];
    for (int i = 1; i < 8; i++)
        cout << "," << c.second[i];
    cout << "}\n";
}
Cube Execute(Cube c, string steps)
{
    Cube temp = c;
    for (auto i : steps)
        for (int j = 0; j < 8; j++)
            temp.first[j] = nextEdgePos[i][temp.first[j]],
            temp.second[j] = nextCornerPos[i][temp.second[j]];
    return temp;
}
vector<string> alg(string s)
{
    vector<string> result, prefix = {"", "", "2", "'"};
    stack<pair<char, int>> st;
    for (auto i : s)
    {
        if (st.empty() || st.top().first != i)
            st.push({i, 1});
        else
        {
            auto temp = st.top();
            st.pop();
            if (i == 'U' || i == 'D')
            {
                if (temp.second != 3)
                    st.push({temp.first, temp.second + 1});
            }
            else if (temp.second != 1)
                st.push({temp.first, temp.second + 1});
        }
    }
    while (!st.empty())
    {
        result.push_back(st.top().first + prefix[st.top().second]);
        st.pop();
    }
    reverse(result.begin(), result.end());
    return result;
}
vector<int> conv(Cube c)
{
    vector<int> v;
    for (auto i : c.first)
        v.push_back(i);
    for (auto i : c.second)
        v.push_back(i);
    return v;
}
string revAlgo(string s)
{
    string result = "";
    for (auto i : s)
    {
        if (i == 'U')
            result += "UUU";
        else if (i == 'D')
            result += "DDD";
        else
            result += i;
    }
    reverse(result.begin(), result.end());
    return result;
}
vector<string> solve(Cube c)
{
    Cube source = c, destination = {SOLVED_EDGE_POS, SOLVED_CORNER_POS};
    if (c == destination)
        return {};
    vector<string> op = {"R", "U", "F", "B", "L", "UUU"};
    queue<pair<Cube, string>> forward, backward;
    map<vector<int>, string> f_sol, b_sol;
    f_sol[conv(c)] = "", b_sol[conv(destination)] = "";
    forward.push({c, ""});
    backward.push({destination, ""});
    while (true)
    {
        auto f = forward.front();
        forward.pop();
        for (auto i : op)
        {
            Cube temp = Execute(f.first, i);
            vector<int> x = conv(temp);
            if (b_sol.count(x))
                return alg(f.second + i + revAlgo(b_sol[x]));
            if (!f_sol.count(x))
            {
                f_sol[x] = f.second + i;
                forward.push({temp, f.second + i});
            }
        }
        auto b = backward.front();
        backward.pop();
        for (auto i : op)
        {
            Cube temp = Execute(b.first, i);
            vector<int> x = conv(temp);
            if (f_sol.count(x))
                return alg(f_sol[x] + revAlgo(b.second + i));
            if (!b_sol.count(x))
            {
                b_sol[x] = b.second + i;
                backward.push({temp, b.second + i});
            }
        }
    }
}
int main()
{
    string input;
    cin >> input;
    Cube c = Execute({SOLVED_EDGE_POS, SOLVED_CORNER_POS}, input);
    // Cube c = {{5,2,7,6,3,4,1,0}, {1,3,6,2,7,5,4,0}};
    for (auto i : alg(input))
        cout << i << " ";
    cout << endl;
    vector<string> result = solve(c);
    for (auto i : result)
        cout << i << " ";
}