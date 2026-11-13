#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

struct Sach {
    string isbn;
    string ten;
    string tacGia;
    string nhaXuatBan;
    int namXuatBan;
    int soLuong;
    int luotMuon;  
};

struct Node {
    Sach data;
    Node* trai;
    Node* phai;
};

const string TEN_FILE = "sach.txt";

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool laSo(const string& s) {
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ++i)
        if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

int stringToInt(const string& s) {
    int kq = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] >= '0' && s[i] <= '9')
            kq = kq * 10 + (s[i] - '0');
    }
    return kq;
}

int nhapSoNguyen(string thongBao) {
    int x;
    while (true) {
        cout << thongBao;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cout << "Du lieu khong hop le. Vui long nhap so nguyen!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string nhapChuoi(string thongBao, bool batBuoc = true) {
    string s;
    while (true) {
        cout << thongBao;
        getline(cin, s);
        s = trim(s);
        if (!batBuoc || !s.empty()) return s;
        cout << "Khong duoc de trong. Nhap lai!\n";
    }
}

int nhapNamXuatBan() {
    int namHienTai = 2026;
    time_t t = time(NULL);
    tm* now = localtime(&t);
    if (now) namHienTai = now->tm_year + 1900;
    while (true) {
        int nam = nhapSoNguyen("Nam xuat ban: ");
        if (nam >= 1900 && nam <= namHienTai) return nam;
        cout << "Nam phai trong khoang 1900 - " << namHienTai << ". Nhap lai!\n";
    }
}

int nhapSoLuongKhongAm() {
    while (true) {
        int sl = nhapSoNguyen("So luong: ");
        if (sl >= 0) return sl;
        cout << "So luong khong duoc am. Nhap lai!\n";
    }
}

