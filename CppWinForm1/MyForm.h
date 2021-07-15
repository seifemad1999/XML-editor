#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <sstream>
#include <msclr\marshal_cppstd.h>
namespace CppWinForm1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Xml;
	using namespace msclr::interop;
	using namespace std;

	vector <string> lines;
	vector <string> tags;
	vector <string> tags_lines;
	vector <string> pureTags;
	vector <string> puretags_w;
	int spaces = 0;
	bool flag = false;
	vector <string> lines_adel;
	vector <string> tags_adel;
	vector <string> pure_tags_adel;


	struct Node {
		string data;
		Node* parent;
		vector<Node *> children;
		string internalData;
	};

	struct Node_x {
		string data;
		Node_x* parent;
		vector<Node_x *> children;
	};
	
	string json = "{";

	void readFile(string fileName) {
		string line;
		ifstream myfile(fileName);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				lines.push_back(line);
			}
			myfile.close();
		}
	}

	void readFile_adel(string fileName)
	{
		string line;
		ifstream myfile(fileName);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{

				if (line[1] == '?' || line[1] == '!')
					continue;
				if (line[0] == ' ')
				{
					int j;
					for (int i = 0; i<line.size(); i++)
					{
						if (line[i] == ' ')
							continue;
						else
						{
							j = i;
							break;
						}
					}
					if (line[line.length() - 1] == ' ' && line[line.length() - 2] != ' ')
						line = line.substr(j, line.length() - j - 1);
					else if (line[line.length() - 1] == ' ' && line[line.length() - 2] == ' ' && line[line.length() - 3] != ' ')
						line = line.substr(j, line.length() - j - 2);
					else if (line[line.length() - 1] == ' ' && line[line.length() - 2] == ' ' && line[line.length() - 3] == ' '&& line[line.length() - 4] != ' ')
						line = line.substr(j, line.length() - j - 3);
					else if (line[line.length() - 1] == ' ' && line[line.length() - 2] == ' ' && line[line.length() - 3] == ' '&& line[line.length() - 4] == ' ')
						line = line.substr(j, line.length() - j - 4);
					else
						line = line.substr(j, line.length() - j);
				}
				if (line[0] == '-')
				{
					line = line.substr(1, line.length() - 1);
				}
				lines_adel.push_back(line);
			}
			myfile.close();
		}
	}

	void gettags_adelAndItems() 
	{
		for (int x = 0; x<lines_adel.size(); x++)
		{
			int tagCounter = count(lines_adel[x].begin(), lines_adel[x].end(), '<'); // count the number of tags_adel in line
			if (tagCounter != 0)
			{
				int place1 = lines_adel[x].find('<');
				int place2 = lines_adel[x].find('>');
				for (int m = 0; m<tagCounter; m++)
				{
					if (lines_adel[x][0] != '<') // data + closed tag
					{
						string d_new = lines_adel[x].substr(0, lines_adel[x].find('<'));
						string t_new = lines_adel[x].substr(lines_adel[x].find('<') + 1, lines_adel[x].length() - (lines_adel[x].find('<') + 2)); // string of data in this location
						if (tags_adel[tags_adel.size() - 1].find("~") != string::npos)
							tags_adel[tags_adel.size() - 1] = tags_adel[tags_adel.size() - 1] + ' ' + d_new;
						else
							tags_adel[tags_adel.size() - 1] = tags_adel[tags_adel.size() - 1] + '~' + d_new;
						tags_adel.push_back(t_new);
					}
					else
					{
						if (m == tagCounter - 1) // one tag only exist
						{
							int temp = lines_adel[x].length() - 1;
							if (place2 != temp) // tag - data
								tags_adel.push_back(lines_adel[x].substr(place1 + 1, place2 - place1 - 1) + '~' + lines_adel[x].substr(place2 + 1, lines_adel[x].length() - 1 - place2));
							else //tag only
							{
								tags_adel.push_back(lines_adel[x].substr(place1 + 1, place2 - place1 - 1));
							}
						}
						else // tag - data - tag
						{
							int q = lines_adel[x].find('/');
							tags_adel.push_back(lines_adel[x].substr(place1 + 1, place2 - place1 - 1) + "~" + lines_adel[x].substr(place2 + 1, (q - 2) - place2));
						}

					}
					int place3 = lines_adel[x].find('<', place1 + 1);
					int place4 = lines_adel[x].find('>', place2 + 1);
					place1 = place3;
					place2 = place4;
				}
			}
			else
			{
				if (tags_adel[tags_adel.size() - 1].find('~') != string::npos) // data after data
				{
					int r = tags_adel[tags_adel.size() - 1].find('~');
					string d = tags_adel[tags_adel.size() - 1].substr(r + 1, tags_adel[tags_adel.size() - 1].length() - r - 1); // string of data in this location
					string t = tags_adel[tags_adel.size() - 1].substr(0, r + 1); // string of tag which take the place of our tag which will inserted
					tags_adel[tags_adel.size() - 1] = t + d + ' ' + lines_adel[x];
				}
				else // data after tag
				{
					tags_adel[tags_adel.size() - 1] = tags_adel[tags_adel.size() - 1] + '~' + lines_adel[x];
				}
			}
		}
	}
	
	void makePuretags_adel() 
	{                  
		for (int x = 0; x<tags_adel.size(); x++)
		{
			if (!tags_adel[x].empty())
			{
				string z;
				if (tags_adel[x].find('~') != string::npos)
				{
					z = tags_adel[x].substr(0, tags_adel[x].find('~'));
				}
				else
				{
					z = tags_adel[x];
				}
				if (z.find(' ') != string::npos)
				{
					string s = z.substr(0, z.find(' '));
					pure_tags_adel.push_back(s);
				}
				else
				{
					pure_tags_adel.push_back(z);
				}
			}
		}
	}
	
	bool Is_Consistent() 
	{
		stack <string> st;
		for (int x = 0; x<pure_tags_adel.size(); x++)
		{
			if (pure_tags_adel[x].find('/') == string::npos)
			{
				st.push(pure_tags_adel[x]);
			}
			else
			{
				if (st.empty())
					return false;
				string s = pure_tags_adel[x].substr(1, pure_tags_adel[x].length() - 1);   //string without /
				if (s == st.top())
				{
					st.pop();
					continue;
				}
				else
				{
					return false;
				}
			}
		}
		if (st.size() == 0) { return true; }
		else { return false; }
	}
	
	Node_x* makeNewNode_x(string data) {
		Node_x* newNode_x = new Node_x;
		newNode_x->data = data;
		return newNode_x;
	}

	Node_x* addChildren(Node_x* root, string data) {
		Node_x* child = makeNewNode_x(data);
		root->children.push_back(child);
		child->parent = root;
		return child;
	}

	void addChildren(Node_x* root, Node_x* child) {
		root->children.push_back(child);
	}

	void showChildren(Node_x* root) {
		int size = root->children.size();
		for (int x = 0; x<size; x++) {
			cout << root->children[x]->data << "\t";
		}
		cout << "\n";
	}
	
	Node_x* getParent(Node_x* root) {

		Node_x* temp = root;
		if (temp->parent != NULL) {
			root = temp->parent;
		}
		return root;
	}
	
	Node_x* getMainParent(Node_x* root) {

		Node_x* temp = root;
		while (1) {
			if (temp->parent == NULL) {
				break;
			}
			else {
				temp = temp->parent;
			}
		}
		return temp;
	}

	Node_x* getLastChild(Node_x* root) {

		if (root->children.size() == 0) {
			return root;
		}
		int last = root->children.size() - 1;
		getLastChild(root->children[last]);
	}

	Node_x* makeTree(vector<string> puretags_adellines_adelWithoutSlash, Node_x* current_root) {
		stack<string> temp;
		for (int x = 0; x<puretags_adellines_adelWithoutSlash.size(); x++) //makes tree
		{
			if (x == 0) // root of tree
			{
				current_root = makeNewNode_x(puretags_adellines_adelWithoutSlash[x]);
				current_root->parent = NULL;
				temp.push(puretags_adellines_adelWithoutSlash[x]);
				continue;
			}
			if (puretags_adellines_adelWithoutSlash[x][0] != '/') // we now on open tag
			{
				if (puretags_adellines_adelWithoutSlash[x].find('~') != string::npos) // we now on the tag which is the leaf
				{
					int r = puretags_adellines_adelWithoutSlash[x].find('~');
					string d = puretags_adellines_adelWithoutSlash[x].substr(r + 1, puretags_adellines_adelWithoutSlash[x].length() - 1); // string of data in this location
					string t = puretags_adellines_adelWithoutSlash[x].substr(0, r); // string of tag which take the place of our tag which will inserted

					if (puretags_adellines_adelWithoutSlash[x - 1].find('~') == string::npos) // not the data of the same previous tag
					{
						current_root = addChildren(current_root, t);
						current_root = addChildren(current_root, d);
						current_root = getParent(current_root);
						current_root = getParent(current_root);
					}
					else // the data of the same previous leaf
					{
						current_root = current_root->children[current_root->children.size() - 1];
						current_root = current_root->children[current_root->children.size() - 1];
						current_root->data = current_root->data + " " + d;
						current_root = getParent(current_root);
						current_root = getParent(current_root);
					}
				}
				else // we now on the tag which is normal tag not the leaf
				{
					current_root = addChildren(current_root, puretags_adellines_adelWithoutSlash[x]);
					temp.push(puretags_adellines_adelWithoutSlash[x]);
				}
			}
			else // we now on closed tag
			{
				if (puretags_adellines_adelWithoutSlash[x - 1].find('~') == string::npos) // pop only of the current closed tag isn't closed tag of leaf
				{                                                          // as we doesn't push the leaf into stack
					temp.pop();
					current_root = getParent(current_root);
				}
			}
		}
		return getMainParent(current_root);
	}
	
	void printXMLFromTree(Node_x*rootptr, ofstream &s) // function to print xml format from tree
	{
		//string line;
		//if(rootptr->parent==NULL)
		//ofstream MyFile("output xml.txt");
		// Write to the file
		//MyFile << "Files can be tricky, but it is fun enough!";

		if (rootptr->children.size() != 0)
		{
			for (int i = 0; i<spaces; i++)
			{
				s << ' ';
			}
			spaces++;
			s << "<" << rootptr->data << ">" << "\n";
		}
		for (int i = 0; i<rootptr->children.size(); i++)
		{
			printXMLFromTree(rootptr->children[i], s);
			s << endl;
			Node_x *parent = getParent(rootptr);
			if (i == rootptr->children.size() - 1 && rootptr->children[i]->children.size() != 0)
			{
				spaces--;
			}
		}
		if (rootptr->children.size() != 0)
		{
			for (int i = 0; i<spaces; i++)
			{
				s << ' ';
			}
			if (rootptr->data.find(' ') == string::npos)
				s << "</" << rootptr->data << ">";
			else
				s << "</" << rootptr->data.substr(0, rootptr->data.find(' ')) << ">";
		}
		else
		{
			for (int i = 0; i<spaces; i++)
			{
				s << ' ';
			}
			spaces--;
			s << rootptr->data;
		}
	}
	
	void LevelOrderTraversal(Node_x * root)
	{
		if (root == NULL)
			return;

		// Standard level order traversal code
		// using queue
		queue<Node_x *> q;  // Create a queue
		q.push(root); // Enqueue root
		while (!q.empty())
		{
			int n = q.size();

			// If this Node_x has children
			while (n > 0)
			{
				// Dequeue an item from queue and print it
				Node_x * p = q.front();
				q.pop();
				cout << p->data << " ";

				// Enqueue all children of the dequeued item
				for (int i = 0; i<p->children.size(); i++)
					q.push(p->children[i]);
				n--;
			}

			cout << endl; // Print new line between two levels
		}
	}

	void gettages_lines() {
		vector<string> t_l;
		tags.resize(0);
		tags_lines.resize(0);
		for (unsigned long long x = 0; x<lines.size(); x++)
		{

			int tag_counter = count(lines[x].begin(), lines[x].end(), '<');
			int p1 = lines[x].find('<');
			int p2 = lines[x].find('>');

			for (int i = 0; i<tag_counter; i++)
			{

				tags.push_back(lines[x].substr(p1 + 1, p2 - p1 - 1));
				t_l.push_back(lines[x].substr(p1 + 1, p2 - p1 - 1));

				if (lines[x][p2 + 1] != '<') {
					int temp = lines[x].find('<', p1 + 1);

					t_l.push_back("~" + lines[x].substr(p2 + 1, temp - p2 - 1));
				}

				int p3 = lines[x].find('<', p1 + 1);
				int p4 = lines[x].find('>', p2 + 1);
				p1 = p3;
				p2 = p4;
			}
		}
		for (unsigned long long x = 0; x<t_l.size(); x++)
		{
			if (!((t_l[x][0] == '~') && (t_l[x].length() == 1))) {
				tags_lines.push_back(t_l[x]);
			}
		}
	}
	
	void make_p_tags() {
		pureTags.resize(0);
		for (unsigned long long x = 0; x<tags.size(); x++)
		{
			if (!tags[x].empty())
			{
				if (tags[x].find(' ') != string::npos)
				{
					string s = tags[x].substr(0, tags[x].find(' '));
					pureTags.push_back(s);
				}
				else
				{
					pureTags.push_back(tags[x]);
				}
			}
		}
	}
	
	void make_p_tags_w()
	{
		puretags_w.resize(0);
		for (unsigned int x = 0; x<tags_lines.size(); x++) {
			if (tags_lines[x][0] == '/') {
				puretags_w.push_back(tags_lines[x].substr(1, tags_lines[x].length() - 1));
			}
			else if (tags_lines[x][0] != '~') {

				puretags_w.push_back(tags_lines[x]);
			}
			else {
				puretags_w.push_back(tags_lines[x]);
			}
		}
	}

	Node* makenode(string data) {
		string mainTag, internalData;
		if (data.find('=') == -1) {
			Node* newNode = new Node;
			newNode->data = data;
			return newNode;
		}
		else {
			int index = data.find(' ');
			mainTag = data.substr(0, index);
			internalData = data.substr(index + 1, data.length() - 1);
			Node* newNode = new Node;
			newNode->data = mainTag;
			newNode->internalData = internalData;
			return newNode;
		}
	}

	Node* add_child(Node* root, string data) {
		Node* child = makenode(data);
		root->children.push_back(child);
		child->parent = root;
		return child;
	}

	void add_child(Node* root, Node* child) {
		root->children.push_back(child);
		return;
	}

	void sh_child(Node* root) {
		int size = root->children.size();
		for (int x = 0; x<size; x++) {
			cout << root->children[x]->data << "\t";
		}
		cout << "\n";
	}

	Node* getp(Node* root) {

		Node* temp = root;
		if (temp->parent != NULL) {
			root = temp->parent;
		}
		return root;
	}

	Node* getmain(Node* root) {

		Node* temp = root;
		while (1) {
			if (temp->parent == NULL) {
				break;
			}
			else {
				temp = temp->parent;
			}
		}
		return temp;
	}

	Node* getLastChild(Node* root) {

		if (root->children.size() == 0) {
			return root;
		}
		else return getLastChild(root->children[root->children.size() - 1]);

	}

	Node* makeTree(vector<string> puretags_w, Node* current_root)
	{

		stack <string> temp;

		for (unsigned int x = 0; x<puretags_w.size(); x++) {
			if (x == 0) {
				current_root = makenode(puretags_w[x]);
				current_root->parent = NULL;
				stringstream check1(puretags_w[x]);
				string s;
				getline(check1, s, ' ');
				temp.push(s);

				continue;
			}

			if (puretags_w[x][0] != '~') {
				stringstream check1(puretags_w[x]);
				string s;
				getline(check1, s, ' ');
				if (s == temp.top()) {
					temp.pop();
					current_root = getp(current_root);
				}
				else {
					current_root = add_child(current_root, puretags_w[x]);
					temp.push(s);
				}
			}
			else {
				current_root = add_child(current_root, puretags_w[x].substr(1, puretags_w[x].length() - 1));
				current_root = getp(current_root);
			}
		}
		return getmain(current_root);
	}

	void makeOneNodeForRepeatedChild(Node* root) {

		if (root->children.size() < 1) {
			return;
		}
		vector<string>temp;
		vector<string>names;

		for (unsigned int x = 0; x<root->children.size(); x++) {
			if (root->children[x]->data != "*") {
				temp.push_back(root->children[x]->data);
			}
		}

		for (unsigned int x = 0; x<temp.size(); x++) {
			if (count(temp.begin(), temp.end(), temp[x]) > 1 && count(names.begin(), names.end(), temp[x]) == 0) {
				names.push_back(temp[x]);
			}
		}

		for (unsigned int x = 0; x<names.size(); x++) {
			Node* simp = makenode(names[x]);
			for (unsigned int y = 0; y<root->children.size(); y++) {
				if (root->children[y]->data == names[x]) {
					root->children[y]->data = '*';
					root->children[y]->parent = simp;
					add_child(simp, root->children[y]);
					root->children.erase(root->children.begin() + y);
					y--;
				}

			}
			add_child(root, simp);
			simp->parent = root;
		}
		return;
	}

	void organizeTree(Node* root) {
		if (root == NULL) { return; }
		makeOneNodeForRepeatedChild(root);
		for (unsigned int x = 0; x<root->children.size(); x++) {
			organizeTree(root->children[x]);
		}
		return;
	}

	void makeQutation(Node* root) {

		for (unsigned int x = 0; x<root->children.size(); x++) {
			if (root->children[x]->data != "*") {
				root->children[x]->data = "\"" + root->children[x]->data + "\"";
			}
			makeQutation(root->children[x]);
		}
	}

	void makeBrackets(Node* root) {

		for (unsigned int x = 0; x<root->children.size(); x++) {

			bool case2 = (root->children.size() >= 1) && (root->children[0]->data != "*") && (root->children[x]->children.size() != 0) && (x == root->children.size() - 1);

			bool case3 = (root->children.size() > 1) && (root->children[0]->data == "*") && (x == root->children.size() - 1);

			if (case2) {
				Node* temp = getLastChild(root);
				if (temp->data[temp->data.length() - 1] == ']' || (temp->data[temp->data.length() - 1] == '}')) {
					int s1 = count(temp->data.begin(), temp->data.end(), ']');
					int s2 = count(temp->data.begin(), temp->data.end(), '}');
					if (s1<0) { s1 = 0; }
					if (s2<0) { s2 = 0; }
					int sum = s1 + s2;
					temp->data.insert(temp->data.length() - sum, "}");
				}
				else {
					temp->data = temp->data + "}";
				}
			}
			else if (case3) {
				Node* temp = getLastChild(root->children[x]);
				if (temp->data[temp->data.length() - 1] == ']' || (temp->data[temp->data.length() - 1] == '}')) {
					int s1 = count(temp->data.begin(), temp->data.end(), ']');
					int s2 = count(temp->data.begin(), temp->data.end(), '}');
					if (s1<0) { s1 = 0; }
					if (s2<0) { s2 = 0; }
					int sum = s1 + s2;
					temp->data.insert(temp->data.length() - sum, "]");
				}
				else {
					temp->data = temp->data + "]";
				}
			}

			makeBrackets(root->children[x]);
		}
	}

	void printNode(Node* root) {


		if (root->data == "\*") {
			json += root->internalData;
		}

		else if (root->children.size() == 0) {
			if (root->parent->internalData.empty() && (root->data[root->data.length() - 1] != '}' || root->data[root->data.length() - 1] != ']')) {
				json += root->data + ",";
			}
			else if (root->parent->internalData.empty()) {
				json += root->data;
			}
			else if (root->parent->internalData.empty() && root->parent->children.size() == 1 && (root->data[root->data.length() - 1] != '}' || root->data[root->data.length() - 1] != ']')) {
				json += root->data;
			}
			else if (!root->parent->internalData.empty() && root->parent->children.size() == 1 && (root->data[root->data.length() - 1] != '}' || root->data[root->data.length() - 1] != ']')) {
				json += "\"text\":" + root->data + "},";
			}
		}


		else if (root->children.size() >0 && root->children[0]->data == "\*") {
			json += root->data + ":[";
		}


		else if (root->children.size() >= 1) {
			if (root->internalData.empty()) {
				json += root->data + ":";
			}
			else if (!root->internalData.empty() && root->children.size() == 1) {
				json += root->data + ":" + root->internalData;
			}
			else if (!root->internalData.empty() && root->children.size()>1) {
				json += root->data + ":" + root->internalData;
			}
		}

	}

	void print(Node* root) {

		printNode(root);
		for (unsigned int x = 0; x<root->children.size(); x++) {
			print(root->children[x]);
		}
		return;
	}

	void makeJson(Node* root) {

		root->data = "\"" + root->data + "\"";

		makeQutation(root);

		makeBrackets(root);

		print(root);

		json[json.length() - 1] = '}';
		return;
	}

	void regulateHyberTags(Node* root) {
		if (!root->internalData.empty()) {
			for (unsigned int x = 0; x<root->internalData.length(); x++) {
				if (x == 0) {
					root->internalData = "{\"" + root->internalData;
				}
				if (root->internalData[x] == ' ' && root->internalData[x - 1] == '"') {
					root->internalData[x] = ',';
				}
				else if (root->internalData[x] == '=') {
					root->internalData[x] = ':';
				}
				if (root->internalData[x] == ',') {
					root->internalData.insert(x + 1, "\"");
				}
				else if (root->internalData[x] == ':') {
					root->internalData.insert(x, "\"");
					x++;
				}
			}
			root->internalData += ",";
		}
	}

	void passRoot(Node* root) {

		regulateHyberTags(root);
		for (unsigned int x = 0; x<root->children.size(); x++) {
			passRoot(root->children[x]);
		}

	}

	string convertedJSON;

	class Stack
	{
		char arr[1000];
		int size;

	public:
		Stack()
		{
			size = 0;
		}
		char top()
		{
			if (size>0)
			{
				return arr[size - 1];
			}

		}
		void push(char v)
		{
			if (size<1000)
			{
				arr[size] = v;
				size++;
			}

		}
		void pop()
		{
			if (size > 0)
			{
				size--;
			}

		}
		bool isempty()
		{
			return size == 0;
		}

		bool is_equal(Stack s)
		{
			Stack c = *this;
			while (!c.isempty() && !s.isempty())
			{
				if (c.top() != s.top())
					return false;
				c.pop();
				s.pop();
			}
			return c.isempty() && s.isempty();
		}

		void display()
		{
			for (int i = size; i >= 0; i--)
			{
				cout << arr[i] << endl;// " | " << flush;
			}
			cout << endl;
		}
	};  
	

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected:
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: FastColoredTextBoxNS::FastColoredTextBox^  fastColoredTextBox1;
	private: FastColoredTextBoxNS::FastColoredTextBox^  fastColoredTextBox2;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::ComponentModel::IContainer^  components;
	public:
	private:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->fastColoredTextBox1 = (gcnew FastColoredTextBoxNS::FastColoredTextBox());
			this->fastColoredTextBox2 = (gcnew FastColoredTextBoxNS::FastColoredTextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fastColoredTextBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fastColoredTextBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Tahoma", 8, System::Drawing::FontStyle::Bold));
			this->button1->Location = System::Drawing::Point(677, 304);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(108, 40);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Import";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Tahoma", 8, System::Drawing::FontStyle::Bold));
			this->label1->Location = System::Drawing::Point(295, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(48, 17);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Input";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Tahoma", 8, System::Drawing::FontStyle::Bold));
			this->label2->Location = System::Drawing::Point(1126, 20);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(58, 17);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Output";
			// 
			// fastColoredTextBox1
			// 
			this->fastColoredTextBox1->AutoCompleteBracketsList = gcnew cli::array< System::Char >(10) {
				'(', ')', '{', '}', '[', ']', '\"',
					'\"', '\'', '\''
			};
			this->fastColoredTextBox1->AutoScrollMinSize = System::Drawing::Size(31, 18);
			this->fastColoredTextBox1->BackBrush = nullptr;
			this->fastColoredTextBox1->CharHeight = 18;
			this->fastColoredTextBox1->CharWidth = 10;
			this->fastColoredTextBox1->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->fastColoredTextBox1->DisabledColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)),
				static_cast<System::Int32>(static_cast<System::Byte>(180)), static_cast<System::Int32>(static_cast<System::Byte>(180)), static_cast<System::Int32>(static_cast<System::Byte>(180)));
			this->fastColoredTextBox1->Font = (gcnew System::Drawing::Font(L"Courier New", 9.75F));
			this->fastColoredTextBox1->IsReplaceMode = false;
			this->fastColoredTextBox1->Location = System::Drawing::Point(12, 40);
			this->fastColoredTextBox1->Name = L"fastColoredTextBox1";
			this->fastColoredTextBox1->Paddings = System::Windows::Forms::Padding(0);
			this->fastColoredTextBox1->SelectionColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->fastColoredTextBox1->Size = System::Drawing::Size(659, 764);
			this->fastColoredTextBox1->TabIndex = 5;
			this->fastColoredTextBox1->Zoom = 100;
			this->fastColoredTextBox1->Load += gcnew System::EventHandler(this, &MyForm::fastColoredTextBox1_Load);
			// 
			// fastColoredTextBox2
			// 
			this->fastColoredTextBox2->AutoCompleteBracketsList = gcnew cli::array< System::Char >(10) {
				'(', ')', '{', '}', '[', ']', '\"',
					'\"', '\'', '\''
			};
			this->fastColoredTextBox2->AutoScrollMinSize = System::Drawing::Size(31, 18);
			this->fastColoredTextBox2->BackBrush = nullptr;
			this->fastColoredTextBox2->CharHeight = 18;
			this->fastColoredTextBox2->CharWidth = 10;
			this->fastColoredTextBox2->Cursor = System::Windows::Forms::Cursors::IBeam;
			this->fastColoredTextBox2->DisabledColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)),
				static_cast<System::Int32>(static_cast<System::Byte>(180)), static_cast<System::Int32>(static_cast<System::Byte>(180)), static_cast<System::Int32>(static_cast<System::Byte>(180)));
			this->fastColoredTextBox2->IsReplaceMode = false;
			this->fastColoredTextBox2->Location = System::Drawing::Point(791, 40);
			this->fastColoredTextBox2->Name = L"fastColoredTextBox2";
			this->fastColoredTextBox2->Paddings = System::Windows::Forms::Padding(0);
			this->fastColoredTextBox2->SelectionColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->fastColoredTextBox2->Size = System::Drawing::Size(654, 764);
			this->fastColoredTextBox2->TabIndex = 6;
			this->fastColoredTextBox2->Zoom = 100;
			this->fastColoredTextBox2->Load += gcnew System::EventHandler(this, &MyForm::fastColoredTextBox2_Load);
			// 
			// button2
			// 
			this->button2->Font = (gcnew System::Drawing::Font(L"Tahoma", 8, System::Drawing::FontStyle::Bold));
			this->button2->Location = System::Drawing::Point(677, 385);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(108, 50);
			this->button2->TabIndex = 7;
			this->button2->Text = L"Convert to Json";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Font = (gcnew System::Drawing::Font(L"Tahoma", 8, System::Drawing::FontStyle::Bold));
			this->button3->Location = System::Drawing::Point(677, 481);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(108, 54);
			this->button3->TabIndex = 8;
			this->button3->Text = L"Check correctness";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// button4
			// 
			this->button4->Font = (gcnew System::Drawing::Font(L"Tahoma", 8, System::Drawing::FontStyle::Bold));
			this->button4->Location = System::Drawing::Point(677, 578);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(108, 54);
			this->button4->TabIndex = 9;
			this->button4->Text = L"Fix consistency";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1474, 833);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->fastColoredTextBox2);
			this->Controls->Add(this->fastColoredTextBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"XML editor";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fastColoredTextBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->fastColoredTextBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		
		OpenFileDialog^ openfileDialog1 = gcnew OpenFileDialog;
		openfileDialog1->ShowDialog();
		String^ filepath = openfileDialog1->FileName;
		
		StreamReader^ x = gcnew StreamReader(filepath);
		String^ text = x->ReadToEnd();
		fastColoredTextBox1->Text = text;
			
	}
	private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void fastColoredTextBox1_Load(System::Object^  sender, System::EventArgs^  e)
	{
		fastColoredTextBox1->Language = FastColoredTextBoxNS::Language::XML;
	}
private: System::Void fastColoredTextBox2_Load(System::Object^  sender, System::EventArgs^  e) 
{
	fastColoredTextBox2->Language = FastColoredTextBoxNS::Language::XML;
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
{
	
	readFile("data-sample.xml");

	gettages_lines();

	make_p_tags();


	make_p_tags_w();


	Node* current_root = NULL;

	current_root = makeTree(puretags_w, current_root);

	organizeTree(current_root);

	passRoot(current_root);

	makeJson(current_root);
	bool bracketclosed = false;
	int k = 0;
	Stack stchar;
	Stack stnum;
	int numberOFspaces = 0;

	for (int i = 0; i < json.size(); i++)
	{
		if (json[i] == '{')
		{
			// push character to stack
			stchar.push(json[i]);

			convertedJSON += json[i];
			convertedJSON += "\n";
			numberOFspaces++;
			for (int j = 0; j < numberOFspaces; j++)
			{
				convertedJSON += " ";
			}
		}

		else if (json[i] == '[')
		{
			bracketclosed = false;
			convertedJSON += json[i];
			convertedJSON += "\n";
			numberOFspaces++;
			for (int j = 0; j < numberOFspaces; j++)
			{
				convertedJSON += " ";
			}
			stnum.push(numberOFspaces);
			k++;
		}

		else if (json[i] == ']')
		{
			bracketclosed = true;
			convertedJSON += "\n";
			for (int j = 0; j < stnum.top() - k; j++)
			{
				convertedJSON += " ";
			}
			convertedJSON += json[i];
			k++;
		}

		else if (json[i] == ':')
		{
			convertedJSON += json[i];
			convertedJSON += " ";
		}
		else if (json[i] == ',')
		{

			convertedJSON += json[i];
			convertedJSON += "\n";

			for (int j = 0; j < numberOFspaces; j++)
			{
				convertedJSON += " ";
			}


		}
		else if (json[i] == '}')
		{
			if (stchar.top() == '{')
			{
				if ((bracketclosed == true))
				{
					numberOFspaces--;
					stchar.pop();
					convertedJSON += "\n";
					for (int j = 0; j < stnum.top() - k; j++)
					{
						convertedJSON += " ";
					}
					k++;
					convertedJSON += json[i];
				}
				else
				{
					numberOFspaces--;
					stchar.pop();
					convertedJSON += "\n";
					for (int j = 0; j < stnum.top(); j++)
					{
						convertedJSON += " ";
					}
					convertedJSON += json[i];
				}
			}


		}
		else
		{
			convertedJSON += json[i];
		}

	}

	String^ result ;
	result = marshal_as<String^>(convertedJSON);
	fastColoredTextBox2->Text = result;
	

}

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) 
{
	
	readFile_adel("datax.txt");    

	gettages_lines();

	make_p_tags();

	if (Is_Consistent() && flag == false)
	{
		String^ ss = "Is Consistent";
		MessageBox::Show(ss);
		flag = true;
	}
	else
	{
		String^ s = "Not Consistent";
		MessageBox::Show(s);
		
	}
		
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) 
{
	
	OpenFileDialog^ openfileDialog1 = gcnew OpenFileDialog;
	openfileDialog1->ShowDialog();
	String^ filepath = openfileDialog1->FileName;

	StreamReader^ x = gcnew StreamReader(filepath);
	String^ text = x->ReadToEnd();
	fastColoredTextBox2->Text = text;

	//Node_x*current_root = makeTree(tags_adel, current_root);  //make Tree, return main Node_x
	//														  //LevelOrderTraversal(current_root);
	//ofstream MyFile("output.txt");
	//printXMLFromTree(current_root, MyFile);
	////Close the file
	//MyFile.close();
}
};

/////////////////////////
/*class Stack
{
	char arr[1000];
	int size;

public:
	Stack()
	{
		size = 0;
	}
	char top()
	{
		if (size>0)
		{
			return arr[size - 1];
		}

	}
	void push(char v)
	{
		if (size<1000)
		{
			arr[size] = v;
			size++;
		}

	}
	void pop()
	{
		if (size > 0)
		{
			size--;
		}

	}
	bool isempty()
	{
		return size == 0;
	}

	bool is_equal(Stack s)
	{
		Stack c = *this;
		while (!c.isempty() && !s.isempty())
		{
			if (c.top() != s.top())
				return false;
			c.pop();
			s.pop();
		}
		return c.isempty() && s.isempty();
	}

	void display()
	{
		for (int i = size; i >= 0; i--)
		{
			cout << arr[i] << endl;// " | " << flush;
		}
		cout << endl;
	}
};*/
////////////////////////
}
