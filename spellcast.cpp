#include <bits/stdc++.h>
using namespace std;


char grid[5][5];
bool dbl[5][5];
bool vis[5][5];
int pts[26] = {1, 4, 5, 3, 1, 5, 3, 4, 1, 7, 6, 3, 4, 2, 1, 4, 8, 2, 2, 2, 4, 5, 5, 7, 4, 8};
int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

struct result {
    string s;
    int score;
    char replaced, old;
};

struct TrieNode {
    TrieNode* children[26];
    bool isEnd = false;
};

TrieNode* root = new TrieNode();

void add(string& word) {
    TrieNode* curr = root;
    for (auto x : word) {
        if (curr->children[x - 'a'] == nullptr) curr->children[x - 'a'] = new TrieNode();
        curr = curr->children[x - 'a'];
    }
    curr->isEnd = true;
}

bool valid(int i, int j, TrieNode* node) {
    return i >= 0 && i < 5 && j >= 0 && j < 5 && !vis[j][i] && node->children[grid[j][i] - 'a'] != nullptr;
}

bool validw(int i, int j) {
    return i >= 0 && i < 5 && j >= 0 && j < 5 && !vis[j][i];
}

vector<result> res;
char old;

void dfsw(int i, int j, TrieNode* curr, string& path, int score, int replaced, bool doubled) {
    if (curr->isEnd) res.push_back({path, score * (doubled ? 2 : 1) + (path.size() >= 6 ? 10 : 0), replaced, old});
    vis[j][i] = true;

    for (int dir = 0; dir < 8; ++dir) {
        int nx = i + dx[dir];
        int ny = j + dy[dir];
        if (grid[ny][nx] == '.') {
            if (validw(nx, ny)) for (int c = 0; c < 26; ++c) if (curr->children[c] != nullptr) {
                path.push_back((char) ('a' + c));
                dfsw(nx, ny, curr->children[c], path, score + pts[c], c, dbl[j][i] || doubled);
                path.pop_back();
            }
        }
        else if (valid(nx, ny, curr)) {
            path.push_back(grid[ny][nx]);
            dfsw(nx, ny, curr->children[grid[ny][nx] - 'a'], path, score + pts[grid[ny][nx] - 'a'], replaced, dbl[j][i] || doubled);
            path.pop_back();
        }
    }

    vis[j][i] = false;
}

int main(void) {

    ifstream fin("wordlist.txt");

    string s;
    while (fin >> s) {
        add(s);
    }


    while (true) {
        cout << "\nInput 5x5 board (capital letter for double word tile):\n";
        char newC;
        for (int j = 0; j < 5; ++j) {
            for (int i = 0; i < 5; ++i) {
                cin >> newC;
                dbl[j][i] = false;
                if (newC >= 'A' && newC <= 'Z') {
                    dbl[j][i] = true;
                    newC = newC - 'A' + 'a';
                }
                grid[j][i] = newC;
            }
        }
        cout << "\nInput Received!\n\n";

        for (int j = 0; j < 5; ++j) {
            for (int i = 0; i < 5; ++i) {
                string path(1, grid[j][i]);
                dfsw(i, j, root->children[grid[j][i] - 'a'], path, pts[grid[j][i] - 'a'], -1, dbl[j][i]);
            }
        }

        sort(res.begin(), res.end(), [](const result& a, const result& b) {
            return a.score > b.score;
        });

        cout << "Best words without swapping:\n";
        set<string> st;
        int idx = 0;
        while (idx < res.size() && st.size() != 10) {
            if (st.count(res[idx].s)) {
                ++idx;
                continue;
            }
            cout << res[idx].score << ' ' << res[idx].s << '\n';
            st.insert(res[idx].s);
        }
        res.clear();

        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 5; ++x) {
                old = grid[y][x];
                grid[y][x] = '.';
                
                for (int j = 0; j < 5; ++j) {
                    for (int i = 0; i < 5; ++i) {

                        if (grid[j][i] == '.') for (int c = 0; c < 26; ++c) if (root->children[c] != nullptr) {
                            string path(1, c + 'a');
                            dfsw(i, j, root->children[c], path, pts[c], c, dbl[j][i]); 
                        }
                        else {
                            string path(1, grid[j][i]);
                            dfsw(i, j, root->children[grid[j][i] - 'a'], path, pts[grid[j][i] - 'a'], -1, dbl[j][i]);
                        }
                    }
                }
                grid[y][x] = old;
            }
        }
        sort(res.begin(), res.end(), [](const result& a, const result& b) {
            return a.score > b.score;
        });

        cout << "Best words with swapping:\n";
        for (int i = 0; i < 10; ++i) {
            if (res[i].replaced == -1 || (res[i].replaced == res[i].old)) cout << res[i].score << ' ' << res[i].s << " replacing nothing\n";
            else cout << res[i].score << ' ' << res[i].s << " replacing " << res[i].old << " with " << (char) (res[i].replaced + 'a') << '\n';
        }
        
        res.clear();
    }

}



// tl - triple letter
// dl - double letter
// 2x - double word




// [1, 4, 5, 3, 1, 5, 3, 4, 1, 7, 6, 3, 4, 2, 1, 4, 8, 2, 2, 2, 4, 5, 5, 7, 4, 8]

// hsagoveooaitluiwneynmnrro