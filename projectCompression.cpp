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
    cout << "the file is compressed by " << compression_percentage << "%" << endl;
    string enccodedContent = readFile(encodedFilename);
    string decodedString = decodeString(root, enccodedContent);
    writefile(decodedFilename, decodedString);
    cout << endl
         << "Decoded string: " << decodedString << endl;
    // The sample output in compiler looks like
    // Initially it prints the code correspnding to each charecter
    //  ->100
    // ,->110101
    // .->101000
    // L->101001
    // a->01010
    // c->10101
    // d->1011
    // e->000
    // g->110100
    // i->001
    // l->01011
    // m->0110
    // n->01111
    // o->1110
    // p->01110
    // r->0100
    // s->1111
    // t->1100
    // u->11011
    // encoded string:101001111001000000110100001011101111110110110100101111100101111100100100111100111001000101001100001100110101100101011110011111111000101011100000110011011010010001010101100101110001111110101001011111101001000000101100111001101011001111000101110010111110100000001110111111011011101011101000
    // length of original string (in bits) is 576
    // length of newString is 288
    // the file is compressed by 50%
    // Decoded string: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod.
}
