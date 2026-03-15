#include<bits/stdc++.h>

using namespace std;

// 检查 IP 地址格式是否正确
bool isValidIP(const string& ip) {
	int a, b, c, d;
	char dot1, dot2, dot3;
	istringstream iss(ip);
	if (iss >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) {
		if (dot1 == '.' && dot2 == '.' && dot3 == '.' &&
			a >= 0 && a <= 255 &&
			b >= 0 && b <= 255 &&
			c >= 0 && c <= 255 &&
			d >= 0 && d <= 255) {
			// 确保后面没有多余字符
			char ch;
			if (!(iss >> ch)) {
				return true;
			}
		}
	}
	return false;
}

// 计算后缀表达式
int evalPostfix(const string& expr) {
	stack<int> st;
	for (char ch : expr) {
		if (isdigit(ch)) {
			st.push(ch - '0');
		} else {
			int b = st.top(); st.pop();
			int a = st.top(); st.pop();
			int res;
			switch (ch) {
				case '+': res = a + b; break;
				case '-': res = a - b; break;
				case '*': res = a * b; break;
			}
			st.push(res);
		}
	}
	return st.top();
}

int main() {
	freopen("./in.in","r",stdin);
	freopen("./out.out","w",stdout);
	
	int n;
	cin >> n;
	
	unordered_map<string, string> servers; // IP -> 后缀表达式
	
	for (int i = 0; i < n; ++i) {
		string op;
		cin >> op;
		
		if (op == "create") {
			string ip, expr;
			cin >> ip >> expr;
			if (!isValidIP(ip) || servers.find(ip) != servers.end()) {
				cout << "failed\n";
			} else {
				servers[ip] = expr;
				cout << "success\n";
			}
		} else if (op == "connect") {
			string ip;
			cin >> ip;
			auto it = servers.find(ip);
			if (!isValidIP(ip) || it == servers.end()) {
				cout << "failed\n";
			} else {
				int result = evalPostfix(it->second);
				cout << result << "\n";
			}
		} else if (op == "delete") {
			string ip;
			cin >> ip;
			if (!isValidIP(ip) || servers.find(ip) == servers.end()) {
				cout << "failed\n";
			} else {
				servers.erase(ip);
				cout << "success\n";
			}
		} else if (op == "modify") {
			string ip, expr;
			cin >> ip >> expr;
			auto it = servers.find(ip);
			if (!isValidIP(ip) || it == servers.end()) {
				cout << "failed\n";
			} else {
				it->second = expr;
				cout << "success\n";
			}
		}
	}
	
	return 0;
}
