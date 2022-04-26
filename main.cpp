#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <cmath>
#include <string>


using namespace std;

struct Node {
	char v;
	Node * l, *r, *p;
	int freq;
	bool left_sun;
};

pair<string, map<char, string>>  Huffman(const string &str);
string  Huffman_decode(const string &str, map<char, string> hcode);
map<char, string> huffmanCode(const vector<char> &, const vector<int> &);
string makehuffmancode(const string &, map<char, string>);
Node * make_node(char key, int pr);

string makehammingcode(const string &);
string decodehammingcode(const string &);

int main() {
	string a;
	cin >> a;
	cout << decodehammingcode(a) << endl;
	return 0;
}

pair<string, map<char, string>> Huffman(const string &str) {
	map<char, int> frequency;
	for (int i = 0; i < str.size(); ++i) {
		frequency[str[i]]++;
	}
	vector<char> alphbet;
	vector<int> counter;
	for (auto x : frequency) {
		alphbet.push_back(x.first);
		counter.push_back(x.second);
	}
	map<char, string> tmp = huffmanCode(alphbet, counter);
	return {makehuffmancode(str, tmp), tmp};
}


map<char, string> huffmanCode(const vector<char> & alphabet, const vector<int> &counter){
	vector<Node*> veretex;
	priority_queue<pair<int, Node*>> queue_freq;
	for (int i = 0; i < alphabet.size(); ++i) {
		veretex.push_back(make_node(alphabet[i], counter[i]));
		queue_freq.push(make_pair(-counter[i], veretex[i]));
	}
	while (queue_freq.size() != 1) {
		Node *tmp1 = queue_freq.top().second;
		queue_freq.pop();
		Node *tmp2 = queue_freq.top().second;
		queue_freq.pop();
		Node * new_node = make_node('#', tmp1->freq + tmp2->freq);
		tmp1->left_sun = true;
		tmp2->left_sun = false;
		new_node->l = tmp1;
		new_node->r = tmp2;
		tmp1->p = new_node;
		tmp2->p = new_node;
		queue_freq.push(make_pair(-(tmp1->freq + tmp2->freq), new_node));
	}
	map<char, string> ans;
	for (auto x : veretex) {
		Node *tmp = x;
		while (tmp->p != NULL) {
			if (tmp->left_sun) {
				ans[x->v] = "0" + ans[x->v];
			} else {
				ans[x->v] = "1" + ans[x->v];
			}
			tmp = tmp->p;
		}
	}
	return ans;
}

string makehuffmancode(const string & str, map<char, string> hcode){
	string ans = "";
	for (int i = 0; i < str.size(); ++i) {
		ans = ans + hcode[str[i]];
	}
	return ans;
}

string Huffman_decode(const string &str, map<char, string> hcode) {
	string ans = "";
	string tmp = str;
	while (tmp.size() != 0) {
		for (auto x : hcode) {
			if (tmp.find(x.second) == 0) {
				ans += x.first;
				string tmp2;
				for (int i = x.second.size(); i < tmp.size(); ++i) {
					tmp2 = tmp2 + tmp[i];
				}
				tmp = tmp2;
			}
		}
	}
	return ans;
}

Node * make_node(char key, int pr) {
	Node * ans = new Node;
	ans->v = key;
	ans->l = ans->r = ans->p = NULL;
	ans->freq = pr;
	ans->left_sun = NULL;
	return ans;
}

string makehammingcode(const string & str){
	vector<int> message;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == '0') {
			message.push_back(0);
			continue;
		}
		message.push_back(1);
	}
	int max_pow = 1;
	while (pow(2, max_pow) < (max_pow + 1 + message.size())) {
		max_pow++;
	}
	vector<int> ans_code(message.size() + max_pow);
	vector<int> pows;
	for (int i = 0; i < max_pow; ++i) {
		ans_code[pow(2, i) - 1] = -100;
		pows.push_back(pow(2, i) - 1);
	}
	int inde_in_ord = 0;
	for (int i = 0; i < ans_code.size(); ++i) {
		if (ans_code[i] != -100) {
			ans_code[i] = message[inde_in_ord];
			inde_in_ord++;
		}
	}
	for (int i = 0; i < max_pow; ++i) {
		ans_code[pow(2, i) - 1] = 0;
	}
	int two_pow = 0;
	for (int i = 0; i < ans_code.size(); ++i) {
		if (find(pows.begin(), pows.end(), i) != pows.end()) {
			int counter = 0;
			for (int j = i; j < ans_code.size(); j += (pow(2, two_pow)) * 2) {
				for (int h = j; h < ans_code.size() && h < j + (pow(2, two_pow)); ++h) {
					counter += ans_code[h];
				}
			}
			ans_code[i] = counter % 2;
			two_pow++;
		}
	}
	string answer_string = "";
	for (auto x : ans_code) {
		answer_string = answer_string + to_string(x);
	}
	return answer_string;
}

string decodehammingcode(const string & str){
	vector<int> message, message2;
	for (int i = 0; i < str.size(); ++i) {
		if (str[i] == '0') {
			message.push_back(0);
			message2.push_back(0);
			continue;
		}
		message.push_back(1);
		message2.push_back(1);
	}
	vector<int> pows;
	for (int i = 0; pow(2, i) <= message.size(); ++i) {
		pows.push_back(pow(2, i) - 1);
	}
	for (auto x : pows) {
		if (x < message.size()) {
			message[x] = 0;
		}
	}
	int two_pow = 0;
	for (int i = 0; i < message.size(); ++i) {
		if (find(pows.begin(), pows.end(), i) != pows.end()) {
			int counter = 0;
			for (int j = i; j < message.size(); j += (pow(2, two_pow)) * 2) {
				for (int h = j; h < message.size() && h < j + (pow(2, two_pow)); ++h) {
					counter += message[h];
				}
			}
			message[i] = counter % 2;
			two_pow++;
		}
	}
	int position = 0;
	for (auto x : pows) {
		if (x < message.size()) {
			if (message[x] != message2[x]) {
				position += x + 1;
			}
		}
	}
	string ans = "";
	for (int i = 0; i < message2.size(); ++i) {
		if (i != position - 1) {
			ans += to_string(message2[i]);
		} else {
			ans += to_string((message2[i] + 1) % 2);
		}
	}
	return ans;
}


