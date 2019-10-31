#include <bits/stdc++.h>

using namespace std;

Token getToken();	//phân tích từ vựng
void error (string message);//Báo lỗi
void factor();//phân tích nhân tử
void term();//phân tích số hạng
void expression(); // phân tích biểu thức
void condition(); // phân tích điều kiện
void statement(); // phân tích câu lệnh
void block(); // phân tích các khối câu lệnh
void program(); //Phân tích chương trình