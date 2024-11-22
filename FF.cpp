#include <bits/stdc++.h>

using namespace std;


const char * output_path = "./ff.txt";
ofstream output_file(output_path);

unordered_map<string, vector<string>> production; // 存放产生式
unordered_map<string, unordered_set<string>> FirstSet;   // 存放 FIRST 集合
unordered_map<string, unordered_set<string>> FirstSetBeta;  // 存放 β 的 FIRST 集合
unordered_map<string, unordered_set<string>> FollowSet;   // 存放 FOLLOW 集合
unordered_set<string> VnSet;	 // 存放非终结符集合
unordered_set<string> VtSet;  // 存放终结符集合
string start = "program";  // 开始符号
static string path = "grammar.txt";
vector<string> getFileContext(string path) {
    ifstream file(path);
    vector<string> content;
    string line;

    while (getline(file, line)) {
        content.push_back(line);
    }
    return content;
}


vector<string> split(const string str, const string delimiter) {
    vector<string> tokens;
    if (delimiter.empty()) {
        return tokens; // 如果分隔符为空，直接返回空向量
    }

    int start = 0;
    int end = str.find(delimiter);
    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start)); // 添加最后一个 token
    return tokens;
}

// 去除空白字符
string trim(const string& str) {
    if (str.empty()) return ""; // 如果字符串为空，直接返回空字符串

    int first = str.find_first_not_of(" \t\n\r");
    int last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1); // 提取去除空白后的子串
}
/**
 * 将产生式存储到production，划分终结符和非终结符
 */
static void dividechar() {
    vector<string> grammarStr = {};

    grammarStr = getFileContext("grammar.txt");
        
    for (string str : grammarStr) {
        vector<string> parts = split(str, "->");
        std::cout << "Read production : " << str << std::endl;

        

        string left = trim(parts[0]);
        string right = trim(parts[1]);

        // 如果 left 存在则取出其 vector，否则创建新 vector
        vector<string>& list = production[left];
        list.push_back(right);

        VnSet.insert(left); // 在产生式左边的一定是非终结符
    }

    // 寻找终结符
    for (const string& ch : VnSet) {
        for (const string& str : production[ch]) {
            vector<string> rightItems = split(str, " ");
            for (const string& c : rightItems) {
                if (VnSet.find(c) == VnSet.end()) { // 若非终结符集合中不包含该符号
                    VtSet.insert(c); // 添加到终结符集合
                }
            }
        }
    }

    output_file << "**************t*****************" << endl;
    for (string ch : VtSet) {
        output_file << ch << endl;
    }

    output_file << "**************not t*****************" << endl;
    for (string ch : VnSet) {
        output_file << ch << endl;
    }
}

void getfirst(const string& ch);

void First() {
    // 遍历求每一个非终结符的 FIRST 集
    for (const string& vn : VnSet) {
        getfirst(vn);
    }
}

void getfirst(const string& ch) {
    vector<string> ch_production = production[ch]; // 获取该符号的产生式
    unordered_set<string>& set = FirstSet[ch]; // FIRST 集合

    // 当 ch 为终结符
    if (VtSet.count(ch)) {
        set.insert(ch);
        return;
    }

    // ch 为非终结符
    for (const string& str : ch_production) {
        vector<string> temp2 = split(str, " ");
        int i = 0;
        while (i < temp2.size()) {
            string tn = temp2[i];
            getfirst(tn); // 递归调用

            unordered_set<string>& tvSet = FirstSet[tn];
            // 将 tn 的 FIRST 集加入到 ch 的 FIRST 集
            for (const string& tmp : tvSet) {
                if (tmp != "$") {
                    set.insert(tmp);
                }
            }

            // 若 tn 的 FIRST 集包含空串，处理下一个符号
            if (tvSet.count("$")) {
                i++;
            } else {
                break;
            }
        }

        // 若所有符号的 FIRST 集中均包含 "$"
        if (i == temp2.size()) {
            set.insert("$");
        }
    }
}

void getFirstBeta(const string& s) {
    unordered_set<string>& set = FirstSetBeta[s]; // 获取或初始化 FirstSetBeta[s]

    vector<string> temp3 = split(s, " ");
    int i = 0;
    while (i < temp3.size()) {
        string tn = temp3[i];

        // 如果 tn 不在 FirstSet 中，调用 getfirst 计算
        if (FirstSet.find(tn) == FirstSet.end()) {
            getfirst(tn);
        }

        // 获取 tn 的 FIRST 集合
        const unordered_set<string>& tvSet = FirstSet[tn];

        // 将非空的元素加入到 set
        for (const string& tmp : tvSet) {
            if (tmp != "$") {
                set.insert(tmp);
            }
        }

        // 如果 tvSet 包含空串，则处理下一个符号
        if (tvSet.find("$") != tvSet.end()) {
            i++;
        } else {
            break;
        }

        // 若已到达尾部且所有符号的 FIRST 集均包含空串
        if (i == temp3.size()) {
            set.insert("$");
        }
    }

    // 更新 FirstSetBeta
    FirstSetBeta[s] = set;
}


void getFollow(const string& c);

void Follow() {
    for (int i = 0; i < 100; i++) {
        for (const string& ch : VnSet) {
            getFollow(ch);
        }
    }
}

void getFollow(const string& c) {
    vector<string> list = production[c];
    unordered_set<string>& setA = FollowSet[c];

    // 如果是开始符号，添加 #
    if (c == start) {
        setA.insert("#");
    }

    // 计算初始 FOLLOW 集
    for (const string& ch : VnSet) {
        vector<string> l = production[ch];
        for (const string& s : l) {
            vector<string> rightItem = split(s, " ");
            for (int i = 0; i < rightItem.size(); i++) {
                if (rightItem[i] == c && i + 1 < rightItem.size() && VtSet.count(rightItem[i + 1])) {
                    setA.insert(rightItem[i + 1]);
                }
            }
        }
    }
    FollowSet[c] = setA;

    // 处理 c 的每个产生式，计算产生式右部各非终结符的 FOLLOW 集
    for (const string& s : list) {
        vector<string> rightItem = split(s, " ");
        int i = rightItem.size() - 1;

        // 从右向左遍历
        while (i >= 0) {
            string tn = rightItem[i];

            // 处理非终结符
            if (VnSet.count(tn)) {
                if (i < rightItem.size() - 1) {
                    string right;
                    for (int j = i + 1; j < rightItem.size(); j++) {
                        right += rightItem[j] + " ";
                    }
                    right = right.substr(0, right.size() - 1); // 去掉尾部空格

                    // 计算 FIRST(β)
                    if (!FirstSetBeta.count(right)) {
                        getFirstBeta(right);
                    }
                    unordered_set<string> setF = FirstSetBeta[right];

                    // 将 β 的非空 FIRST 集加入到 FOLLOW(tn)
                    unordered_set<string>& setX = FollowSet[tn];
                    for (const string& var : setF) {
                        if (var != "$") {
                            setX.insert(var);
                        }
                    }
                    FollowSet[tn] = setX;

                    // 若 FIRST(β) 包含空串，将 FOLLOW(c) 加入 FOLLOW(tn)
                    if (setF.count("$")) {
                        if (tn != c) {
                            unordered_set<string>& setB = FollowSet[tn];
                            setB.insert(setA.begin(), setA.end());
                            FollowSet[tn] = setB;
                        }
                    }
                }
                // 若 β 不存在，将 FOLLOW(c) 加入 FOLLOW(tn)
                else {
                    if (tn != c) {
                        unordered_set<string>& setB = FollowSet[tn];
                        setB.insert(setA.begin(), setA.end());
                        FollowSet[tn] = setB;
                    }
                }
            }
            i--;
        }
    }
}

void output() {
    
    
    

    output_file << "************* FIRST ************" << endl;
    for (const string& c : VnSet) {
        const unordered_set<string>& set = FirstSet[c];
        output_file << setw(18) << c << " -> ";
        for (const string& var : set) {
            output_file << var << " ";
        }
        output_file << endl;
    }
    output_file << endl;

    output_file << "************** FOLLOW *************" << endl;
    for (const string& c : VnSet) {
        const unordered_set<string>& set = FollowSet[c];
        output_file << setw(18) << c << " : ";
        for (const string& var : set) {
            output_file << var << " ";
        }
        output_file << endl;
    }
    output_file << endl;
    
}

int main() {
    dividechar(); // 划分终结符与非终结符
    First(); // 计算 First 集合

    // 对每个非终结符的产生式调用 getFirstBeta
    for (const string& c : VnSet) {
        vector<string> l = production[c];
        for (const string& s : l) {
            getFirstBeta(s);
        }
    }

    Follow(); // 计算 Follow 集合
    output(); // 打印结果

    output_file.close();
    
    return 0;
}
