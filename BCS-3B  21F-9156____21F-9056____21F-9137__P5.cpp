# include<iostream>
# include<fstream>
# include<string>
# include<cstring>
#include <Windows.h>
using namespace std;

char insert[200];
int count11 = 0;
int** ptr;
char* str_root;

static int codees = 0;
static int arr00[26];

ifstream Fileinput;
ofstream newFile, inputFile;
string temp;

//Creating class of TrieNode for implementing Nodes of Trie Tree
class TrieNode {
public:
	char data;
	TrieNode* Child[26];
	bool isterminal;

	TrieNode(char data) {
		this->data = data;
		for (int i = 0; i < 26; i++) {
			Child[i] = NULL;
		}
		isterminal = false;
	}
};
const int MAX_TREE_HT = 100;

// A Huffman tree node
struct MinHeapNode {

	// One of the input characters
	char data;

	// Frequency of the character
	unsigned freq;

	// Left and right child of this node
	struct MinHeapNode* left, * right;
};

string load, text;
// A Min Heap: Collection of
// min-heap (or Huffman tree) nodes
struct MinHeap {

	// Current size of min heap
	unsigned size;

	// capacity of min heap
	unsigned capacity;

	// Array of minheap node pointers
	struct MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
	struct MinHeapNode* temp
		= (struct MinHeapNode*)malloc
		(sizeof(struct MinHeapNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity){

	struct MinHeap* minHeap = new MinHeap;

	// current size is 0
	minHeap->size = 0;

	minHeap->capacity = capacity;

	minHeap->array = new MinHeapNode * [minHeap->capacity];
	return minHeap;
}

// A utility function to
// swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a,
	struct MinHeapNode** b)

{

	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)

{

	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->
		freq < minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->
		freq < minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swapMinHeapNode(&minHeap->array[smallest],
			&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

// A utility function to check
// if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{

	return (minHeap->size == 1);
}

// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)

{

	struct MinHeapNode* temp = minHeap->array[0];
	minHeap->array[0]
		= minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)

{

	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {

		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap)

{

	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

// A utility function to print an array of size n

// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)

{

	return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size)

{

	struct MinHeap* minHeap = createMinHeap(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], freq[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size)

{
	struct MinHeapNode* left, * right, * top;

	// Step 1: Create a min heap of capacity
	// equal to size. Initially, there are
	// modes equal to size.
	struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

	// Iterate while size of heap doesn't become 1
	while (!isSizeOne(minHeap)) {

		// Step 2: Extract the two minimum
		// freq items from min heap
		left = extractMin(minHeap);
		right = extractMin(minHeap);

		// Step 3: Create a new internal
		// node with frequency equal to the
		// sum of the two nodes frequencies.
		// Make the two extracted node as
		// left and right children of this new node.
		// Add this node to the min heap
		// '$' is a special value for internal nodes, not used
		top = newNode('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	// Step 4: The remaining node is the
	// root node and the tree is complete.
	return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
void printCodes(struct MinHeapNode* root, int arr[], int top, char str[])

{
	// Assign 0 to left edge and recur
	if (root->left) {

		arr[top] = 0;
		printCodes(root->left, arr, top + 1, str);
	}

	// Assign 1 to right edge and recur
	if (root->right) {

		arr[top] = 1;
		
		printCodes(root->right, arr, top + 1, str);
	}
	// If this is a leaf node, then
	// it contains one of the input
	// characters, print the character
	// and its code from arr[]
	static int j = 0;
	static int k = 0;
	if (isLeaf(root))
	{

		for (int i = 0; i < top; ++i)
		{
			ptr[j][i] = arr[i];
			str_root[j] = root->data;
		}
		j++;
		//cout << "fuckoffman" << endl;
		cout << endl;
	}
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
MinHeapNode* HuffmanCodes(char data[], int freq[], int size, char str[])
{
	// Construct Huffman Tree
	struct MinHeapNode* root
		= buildHuffmanTree(data, freq, size);

	// Print Huffman codes using
	// the Huffman tree built above
	int arr[MAX_TREE_HT], top = 0;

	printCodes(root, arr, top, str);

	
	for (int i = 0;insert[i] != '\0';i++)
	{
		int kl = 0;
		inputFile.open("Text.txt", ios::app);
		for (int l = 0;l < count11;l++)
		{
			if (insert[i] == str_root[l])
			{
				while (ptr[l][kl] != -1)
				{
					inputFile << ptr[l][kl];
					kl++;
				}
			}
		}
		inputFile.close();
	}
	return root;
}


//Creating class of TrieNode for implementing Trie Tree
class TrieTree {
private:
	TrieNode* root;
public:
	TrieTree() 
	{
		root = new TrieNode('\0');
	}
	//Setter for root
	void setRoot(TrieNode* root) {
		this->root = root;
	}
	//Getter for root
	TrieNode* getRoot() {
		return root;
	}

	//Function to implement insertion algo in trie tree
	void NodeForInsert(TrieNode* root, string word) {

		//For base case
		if (word.length() == 0) {
			root->isterminal = 1;
			return;
		}


		int i = word[0] - 'a';	// Capital Letters
		TrieNode* child = new TrieNode('\0');
		if (root->Child[i] != NULL) {
			child = root->Child[i];		//If the Character is already present
		}
		else {
			child = new TrieNode(word[0]);
			root->Child[i] = child;		//If the Character is not present so create a node with that charater of word
		}
		NodeForInsert(child, word.substr(1));
	}

	// Function to be called for insertion
	void Insert(string word) {
		NodeForInsert(root, word);
	}

	//-----------Searching for a word in TrieTree--------------
	bool searching(TrieNode* root, string word) {
		//Base Case
		if (word.length() == 0) {
			return root->isterminal;
		}

		TrieNode* child;
		int index = word[0] - 'a';
		//Checking if the word is present or not
		if (root->Child[index] != NULL) {
			child = root->Child[index];
		}

		// When word is not present
		else {
			return 0;
		}

		return searching(child, word.substr(1));
	}

	void Swap(int* freq_count, char* chr)						//sorting
	{
		for (int i = 0; i < 26; i++)
		{
			for (int j = i + 1; j < 26; j++)
			{
				if (freq_count[i] > freq_count[j])
				{
					swap(freq_count[i], freq_count[j]);
					swap(chr[i], chr[j]);

				}

			}
		}
	}

	void decode(MinHeapNode* root)						//Decode
	{
		Fileinput.open("Text.txt");
		MinHeapNode* temp = root;
		char ch;
		while (Fileinput)
		{
			ch = Fileinput.get();
			if (ch == '0')
			{
				if (root != NULL)
				{
					if (root->left != NULL)
					{
						root = root->left;
						if (root != NULL)
						{
							if (root->left == NULL && root->right == NULL)
							{
								cout << root->data;
								root = temp;
							}
						}
					}
				}
			}
			if (ch == '1')
			{
				if (root != NULL)
				{
					if (root->right != NULL)
						root = root->right;
					if (root != NULL)
					{
						if (root->left == NULL && root->right == NULL)
						{
							cout << root->data;
							root = temp;
						}
					}
				}
			}
		}
	}

	MinHeapNode* frequency(char freq[])								//frequency of the string
	{
		int* freq_count = new int[26];
		int count = 0, temp = 0;
		char* chr = new char[26], temp1 = ' ';

		for (int i = 0; i < 26; i++)
		{
			freq_count[i] = 0;
			chr[i] = '~';
		}

		for (int i = 0; freq[i] != '\0'; i++)					//calculating frequency
		{
			if (freq[i] >= 'a' && freq[i] <= 'z')
			{
				freq_count[freq[i] - 'a']++;
				chr[freq[i] - 'a'] = freq[i];

			}
		}
		for (int i = 0;i < 26;i++)							//for some reason
		{
			if (chr[i] != '~')
			{
				count11++;
			}
		}
		str_root = new char[count11];
		ptr = new int* [count11];
		for (int i = 0;i < count11;i++)
		{
			ptr[i] = new int[8];
		}
		for (int i = 0;i < count11;i++)
		{
			for (int j = 0;j < 8;j++)
			{
				ptr[i][j] = -1;
			}
		}
		for (int i = 0; i < 26; i++)
		{
			if (freq_count[i] == 0)
			{
				freq_count[i] = INT_MAX;
			}
			else
			{
				count++;
			}
		}
		Swap(freq_count, chr);
		int* freq1 = new int[count];
		char* arr = new char[count];
		for (int i = 0; i < count; i++)
		{
			freq1[i] = freq_count[i];
			arr[i] = chr[i];
		}

		MinHeapNode* root = HuffmanCodes(arr, freq1, count, freq);
		return root;
	}

	//Function to called for searching
	bool searchWord(string word) {
		return searching(root, word);
	}

	//-----------Searching Prefix------------
	bool PrefixSearching(TrieNode* root, string word) {
		//Base Case
		if (word.length() == 0) {
			return 1;
		}

		TrieNode* child;
		int index = word[0] - 'a';
		//Checking if the word is present or not
		if (root->Child[index] != NULL) {
			child = root->Child[index];
		}

		// When word is not present
		else {
			return 0;
		}

		return PrefixSearching(child, word.substr(1));
	}

	//Function to be called for PrefixSearching
	bool PrefixSearchWord(string word) {
		return PrefixSearching(root, word);
	}


	//------------------Suggestions Generation-------------------
	void suggestionsRec(struct TrieNode* root, string EnteredPrefix, string* suggestions, int& suggCount) {
		// found a string in Trie with the given prefix
		if (root->isterminal && suggCount < 10) {
			cout << EnteredPrefix << endl;
			suggestions[suggCount] = EnteredPrefix;
			suggCount++;
		}
		for (int i = 0; i < 26; i++)
			if (root->Child[i]) {
				// child node character value
				char child = 'a' + i;
				suggestionsRec(root->Child[i], EnteredPrefix + child, suggestions, suggCount);
			}
	}

	// To check if last node is terminating node or node
	bool last_node(TrieNode* root) {
		for (int i = 0; i < 26; i++) {
			if (root->Child[i] != NULL) {
				return false;
			}
		}
		return true;
	}

	// print suggestions for given query prefix.
	int printAutoSuggestions(TrieNode* root, const string query, string* suggestions, int& suggCount) {
		TrieNode* temp = root;
		for (int i = 0; i < query.length(); i++) {
			int ind = query[i] - 'a';

			// no string in the Trie has this prefix
			if (!temp->Child[ind])
				return 0;

			temp = temp->Child[ind];
		}
		// If prefix is present as a word, but
		// there is no subtree below the last
		// matching node.

		if (last_node(temp)) {
			cout << query << endl;
			return -1;
		}
		suggestionsRec(temp, query, suggestions, suggCount);
		return 1;
	}

};

int main()
{
	MinHeapNode* rootMHN = new MinHeapNode;
	TrieTree* obj = new TrieTree;
	string in;
	ifstream Fileinput;
	char f[100] = "teaannneeeessss";
	Fileinput.open("outfile.txt");
	if (!Fileinput.is_open()) {
		cout << "File does not exsist!" << endl;
	}
	else {
		while (!Fileinput.eof()) {
			Fileinput >> in;
			obj->Insert(in);

		}
	}

	for (int ab = 0; ab < 26; ab++)
	{
		arr00[ab] = INT_MAX;
	}



		//-----------------Main Menu----------------------
	bool chk = 1;
	bool contWrite, tempchk = 0;
	int choice;
	string temp;
	ofstream newFile, inputFile;
	ifstream exsistingFile;
	string load, text;
	string* suggestions = new string[10];
	int chooseSugg = 0;
	const int size = 200;
	cout << "---------------------Text Editor 2.0----------------------" << endl;
	while (chk) {
		cout << "Enter 1: To Create a new Text File" << endl;
		cout << "Enter 2: To Load data from an exsisting Text File and update data" << endl;
		cout << "Enter 3: To encode" << endl;
		cout << "Enter 4: To decode" << endl;
		cout << "Enter 9: To exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;
		switch (choice) {
		case 1:
			cout << "\n\nEnter the name of the file: ";
			cin >> temp;
			newFile.open(temp + ".txt");
			if (newFile.is_open()) {
				cout << "\n\nSuccessfully created a new file!" << endl;
				newFile.close();
			}
			else {
				cout << "\n\nUnable to create a new file!" << endl;
			}
			break;
		case 2:
			cout << "\n\nEnter the name of the file to load its contents!" << endl;
			cin >> temp;

			exsistingFile.open(temp + ".txt");
			text = "";

			contWrite = 1;

			if (!exsistingFile.is_open()) {
				cout << "\n\nUnable to create a new file!" << endl;
				newFile.close();
			}
			else {

				system("cls");
				cout << "\n\n\tSuccessfully loaded the file!" << endl;
				cout << "\t\t" << temp << ".txt" << endl;
				while (!exsistingFile.eof()) {
					getline(exsistingFile, load);
					cout << " " << load << " ";

				}
				exsistingFile.close();

				cin.ignore();
				cin.getline(insert, size);
				while (contWrite) {

					//---
					text = "";

					tempchk = 0;
					for (int i = 0; insert[i] != '\0'; i++) {
						text += insert[i];
					}
					if (text == "End") {
						contWrite = 0;
						break;
					}
					else if (text != "End") {		// We can write until End is written
						int suggCount = 0;
						for (int j = 0; j < 10; j++) {
							suggestions[j] = "";		//Initializing the suggestions array
						}
						int comp = obj->printAutoSuggestions(obj->getRoot(), text, suggestions, suggCount);
						if (comp == -1) {
							cout << "No other strings found with this prefix\n";
						}
						else if (comp == 0) {
							cout << "No string found with this prefix\n";
							inputFile.open(temp + ".txt", ios::app);
							//inputFile << text;
							//inputFile << " ";
							inputFile.close();
							tempchk = 1;
						}
						else if (comp == -1 || comp == 1) {
							cin >> chooseSugg;
							if (chooseSugg == 0) {
								inputFile.open(temp + ".txt", ios::app);
								//inputFile << text;
								//inputFile << " ";
								inputFile.close();
							}
							else {
								text = suggestions[chooseSugg - 1];
								inputFile.open(temp + ".txt", ios::app);
								//inputFile << text;
								//inputFile << " ";
								inputFile.close();
							}

						}
						exsistingFile.open(temp + ".txt");
						system("cls");
						cout << "\n\n\tSuccessfully loaded the file!" << endl;
						cout << "\t\t" << temp << ".txt" << endl;
						while (!exsistingFile.eof()) {
							getline(exsistingFile, load);
							cout << " " << load << " ";
						}
						exsistingFile.close();

						if (tempchk == 0) {
							cin.ignore();
							cin.getline(insert, size);
						}
						else {
							cin.getline(insert, size);
						}
					}

				}
			}
			break;
		case 3:
			rootMHN = obj->frequency(f);
			cout << endl;
			break;
		case 4:
			obj->decode(rootMHN);
			cout << endl;
			break;
		case 9:
			chk = 0;
			break;
		}
	}

	cout << endl;
	system("pause");
	return 0;
}