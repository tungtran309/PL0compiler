#include <bits/stdc++.h>

using namespace std;

Token getToken();	//phân tích từ vựng
void error (string message);//Báo lỗi
bool factor();//phân tích nhân tử
bool term();//phân tích số hạng
bool expression(); // phân tích biểu thức
void condition(); // phân tích điều kiện
void statement(); // phân tích câu lệnh
void block(); // phân tích các khối câu lệnh
void program(); //Phân tích chương trình