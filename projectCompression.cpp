#include <bits/stdc++.h>
using namespace std;
class HuffmanTreeNode
{
public:
    char charec;
    long long freeq;
    HuffmanTreeNode *left, *right;
    HuffmanTreeNode(char c, int f, HuffmanTreeNode *Left, HuffmanTreeNode *Right)
    {
        this->charec = c;
        this->freeq = f;
        this->left = Left;
        this->right = Right;
    }
};
class Comapre
{
public:
    bool operator()(HuffmanTreeNode *a, HuffmanTreeNode *b)
    {
        return a->freeq > b->freeq;
    }
};
HuffmanTreeNode *createTree(priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, Comapre> pq)
{
    while (pq.size() != 1)
    {
        HuffmanTreeNode *least_freqnode = pq.top();
        pq.pop();
        HuffmanTreeNode *second_least_freqnode = pq.top();
        pq.pop();
        int new_freq = least_freqnode->freeq + second_least_freqnode->freeq;
        HuffmanTreeNode *node = new HuffmanTreeNode('$', new_freq, least_freqnode, second_least_freqnode);
        pq.push(node);
    }
    return pq.top();
}
void encodeStringtoHuffMan(HuffmanTreeNode *root, string str, map<char, string> &HuffmanCodes)
{

    if (root == NULL)
        return;
    if (root->left == NULL && root->right == NULL)
    {
        HuffmanCodes[root->charec] = str;
    }
    encodeStringtoHuffMan(root->left, str + '0', HuffmanCodes);
    encodeStringtoHuffMan(root->right, str + '1', HuffmanCodes);
}
pair<HuffmanTreeNode *, string> formBasicNodes(string s, map<char, int> &freequency)
{
    priority_queue<HuffmanTreeNode *, vector<HuffmanTreeNode *>, Comapre> pq;
    for (auto it : freequency)
    {
        HuffmanTreeNode *newNode = new HuffmanTreeNode(it.first, it.second, NULL, NULL);
        pq.push(newNode);
    }
    HuffmanTreeNode *root = createTree(pq);
    map<char, string> HuffmanCodes;
    encodeStringtoHuffMan(root, "", HuffmanCodes);
    for (auto it : HuffmanCodes)
    {
        cout << it.first << "->";
        for (auto ix : it.second)
        {
            cout << ix;
        }
        cout << endl;
    }
    string encodedstring = "";
    for (auto it : s)
    {
        encodedstring += HuffmanCodes[it];
    }
    return {root, encodedstring};
}
void calcFreeq(string s, map<char, int> &freequency)
{
    for (auto it : s)
    {
        freequency[it]++;
    }
}
string decodeString(HuffmanTreeNode *root, string encodedString)
{
    string ans = "";
    HuffmanTreeNode *curr = root;
    for (int i = 0; i < encodedString.size(); i++)
    {
        if (encodedString[i] == '0')
        {
            curr = curr->left;
        }
        else
        {
            curr = curr->right;
        }
        if (curr->left == NULL && curr->right == NULL)
        {
            ans += curr->charec;
            curr = root;
        }
    }
    return ans;
}
string readFile(const string &filename)
{
    ifstream inputfile(filename);
    stringstream buffer;
    buffer << inputfile.rdbuf();
    return buffer.str();
}
void writefile(const string &filename, const string &content)
{
    ofstream outputfile(filename);
    outputfile << content;
}

int main()
{

    string inputFilename = "input.txt";
    string encodedFilename = "encoded.txt";
    string decodedFilename = "decoded.txt";

    string s = readFile(inputFilename);

    map<char, int> freequency;
    calcFreeq(s, freequency);
    auto it = formBasicNodes(s, freequency);
    string encodedString = it.second;
    HuffmanTreeNode *root = it.first;
    writefile(encodedFilename, encodedString);

    cout << "encoded string:" << encodedString << endl;
    cout << "length of original string (in bits) is " << 8 * s.size() << endl;
    int newlen = encodedString.size();
    cout << "length of newString is " << newlen << endl;
    float compression_ratio = (float)(newlen) / (float)(8 * s.size());
    float compression_percentage = (compression_ratio) * 100;
    cout << "the file is compressed by " << compression_percentage << endl;
    string enccodedContent = readFile(encodedFilename);
    string decodedString = decodeString(root, enccodedContent);
    writefile(decodedFilename, decodedString);
    cout << endl
         << "Decoded string: " << decodedString << endl;
}
