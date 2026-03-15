#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <set>
#include <iterator>

using namespace std;

// 生成随机IP地址 - 按值传递分布
string generateRandomIP(mt19937& gen, uniform_int_distribution<> ip_part_dist) {
	int a = ip_part_dist(gen);
	int b = ip_part_dist(gen);
	int c = ip_part_dist(gen);
	int d = ip_part_dist(gen);
	
	ostringstream oss;
	oss << a << "." << b << "." << c << "." << d;
	return oss.str();
}

// 生成随机后缀表达式 - 按值传递分布
string generateRandomPostfix(mt19937& gen, uniform_int_distribution<> len_dist,
							 uniform_int_distribution<> num_dist,
							 uniform_int_distribution<> op_dist) {
	int target_len = len_dist(gen);  // 目标长度
	int num_count = 0;  // 当前数字数量
	int op_count = 0;   // 当前运算符数量
	
	vector<char> operators = {'+', '-', '*'};
	string expr;
	
	for(int i=0;i<target_len;i++){
		expr+=(num_dist(gen)+'0');
	}
	
	for(int i=0;i<target_len-1;i++){
		expr+=operators[len_dist(gen)%3];
	}
	
	return expr;
}

// 检查IP是否有效
bool isValidIPFormat(const string& ip) {
	stringstream ss(ip);
	int a, b, c, d;
	char dot1, dot2, dot3;
	char extra;
	
	if (ss >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d) {
		if (dot1 == '.' && dot2 == '.' && dot3 == '.' && 
			a >= 0 && a <= 255 && b >= 0 && b <= 255 &&
			c >= 0 && c <= 255 && d >= 0 && d <= 255) {
			// 检查是否有额外字符
			if (!(ss >> extra)) {
				return true;
			}
		}
	}
	return false;
}

int main() {
	// 随机数生成器
	random_device rd;
	mt19937 gen(rd());
	
	freopen("./in.in","w",stdout);
	
	
	// 各种分布
	uniform_int_distribution<> op_count_dist(1, 15);  // 操作数量
	uniform_int_distribution<> ip_part_dist(0, 255);  // IP各部分
	uniform_int_distribution<> expr_len_dist(3, 15);  // 表达式长度（较小以生成有效表达式）
	uniform_int_distribution<> num_dist(0, 9);        // 数字0-9
	uniform_int_distribution<> op_type_dist(0, 3);    // 操作类型
	
	vector<string> ops = {"create", "connect", "delete", "modify"};
	
	// 生成操作数量
	int n;cin>>n;
	cout << n << endl;
	
	// 存储已创建的IP
	set<string> created_ips;
	
	for (int i = 0; i < n; i++) {
		// 随机选择操作类型
		int op_choice = op_type_dist(gen);
		string op = ops[op_choice];
		
		// 生成随机IP
		string ip = generateRandomIP(gen, ip_part_dist);
		
		if (op == "create") {
			// 随机决定是否使用已存在的IP（30%概率）
			if (!created_ips.empty() && ip_part_dist(gen) % 100 < 5) {
				auto it = created_ips.begin();
				advance(it, op_type_dist(gen) % created_ips.size());
				ip = *it;
			}
			
			// 生成随机后缀表达式
			string expr = generateRandomPostfix(gen, expr_len_dist, num_dist, 
												uniform_int_distribution<>(0, 3));
			
			cout << op << " " << ip << " " << expr << endl;
			
			// 记录成功创建的IP
			if (isValidIPFormat(ip)) {
				created_ips.insert(ip);
			}
		} 
		else if (op == "modify") {
			// 随机决定是否使用存在的IP（70%概率）
			if (!created_ips.empty() && ip_part_dist(gen) % 10 < 5) {
				auto it = created_ips.begin();
				advance(it, op_type_dist(gen) % created_ips.size());
				ip = *it;
			}
			
			// 生成新的后缀表达式
			string expr = generateRandomPostfix(gen, expr_len_dist, num_dist,
												uniform_int_distribution<>(0, 3));
			cout << op << " " << ip << " " << expr << endl;
		}
		else { // connect 或 delete
			// 随机决定是否使用存在的IP（60%概率）
			if (!created_ips.empty() && ip_part_dist(gen) % 10 < 5) {
				auto it = created_ips.begin();
				advance(it, op_type_dist(gen) % created_ips.size());
				ip = *it;
			}
			
			cout << op << " " << ip << endl;
		}
	}
	

	
	return 0;
}
