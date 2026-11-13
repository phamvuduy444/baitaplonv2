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
Node* taoNode(Sach s) {
    Node* p = new Node;
    p->data = s;
    p->trai = NULL;
    p->phai = NULL;
    return p;
}

Node* timKiem(Node* root, string isbn) {
    if (root == NULL) return NULL;
    if (isbn == root->data.isbn) return root;
    if (isbn < root->data.isbn)
        return timKiem(root->trai, isbn);
    return timKiem(root->phai, isbn);
}

Node* themSach(Node* root, Sach s, bool& thanhCong) {
    if (root == NULL) {
        thanhCong = true;
        return taoNode(s);
    }
    if (s.isbn < root->data.isbn)
        root->trai = themSach(root->trai, s, thanhCong);
    else if (s.isbn > root->data.isbn)
        root->phai = themSach(root->phai, s, thanhCong);
    else
        thanhCong = false;
    return root;
}

Node* nutNhoNhat(Node* root) {
    while (root != NULL && root->trai != NULL)
        root = root->trai;
    return root;
}

Node* xoaSach(Node* root, string isbn, bool& daXoa) {
    if (root == NULL) return NULL;
    if (isbn < root->data.isbn) {
        root->trai = xoaSach(root->trai, isbn, daXoa);
    } else if (isbn > root->data.isbn) {
        root->phai = xoaSach(root->phai, isbn, daXoa);
    } else {
        daXoa = true;
        if (root->trai == NULL) {
            Node* tam = root->phai;
            delete root;
            return tam;
        }
        if (root->phai == NULL) {
            Node* tam = root->trai;
            delete root;
            return tam;
        }
        Node* keTiep = nutNhoNhat(root->phai);
        root->data = keTiep->data;
        bool xoaPhu = false;
        root->phai = xoaSach(root->phai, keTiep->data.isbn, xoaPhu);
    }
    return root;
}

void giaiPhong(Node* root) {
    if (root == NULL) return;
    giaiPhong(root->trai);
    giaiPhong(root->phai);
    delete root;
}

bool daCoTenSach(Node* root, const string& ten) {
    if (root == NULL) return false;
    if (toLowerStr(root->data.ten) == toLowerStr(ten)) return true;
    return daCoTenSach(root->trai, ten) || daCoTenSach(root->phai, ten);
}

void inMotSach(Sach s) {
    cout << "ISBN: " << s.isbn << '\n';
    cout << "Ten sach: " << s.ten << '\n';
    cout << "Tac gia: " << s.tacGia << '\n';
    cout << "Nha xuat ban: " << s.nhaXuatBan << '\n';
    cout << "Nam xuat ban: " << s.namXuatBan << '\n';
    cout << "So luong: " << s.soLuong << '\n';
    cout << "Luot muon: " << s.luotMuon << '\n';
    cout << "-----------------------------\n";
}

void inTieuDeBang() {
    cout << left << setw(12) << "ISBN"
         << setw(22) << "Ten sach"
         << setw(18) << "Tac gia"
         << setw(18) << "Nha xuat ban"
         << setw(6)  << "Nam"
         << setw(6)  << "SL"
         << setw(6)  << "Muon" << '\n';
    cout << string(88, '-') << '\n';
}

void inMotSachBang(Sach s) {
    cout << left << setw(12) << s.isbn
         << setw(22) << s.ten
         << setw(18) << s.tacGia
         << setw(18) << s.nhaXuatBan
         << setw(6)  << s.namXuatBan
         << setw(6)  << s.soLuong
         << setw(6)  << s.luotMuon << '\n';
}

void duyetNLR(Node* root) {
    if (root == NULL) return;
    inMotSach(root->data);
    duyetNLR(root->trai);
    duyetNLR(root->phai);
}

void duyetLRN(Node* root) {
    if (root == NULL) return;
    duyetLRN(root->trai);
    duyetLRN(root->phai);
    inMotSach(root->data);
}

void duyetLNRTable(Node* root) {
    if (root == NULL) return;
    duyetLNRTable(root->trai);
    inMotSachBang(root->data);
    duyetLNRTable(root->phai);
}

void thuThapLNR(Node* root, vector<Sach>& ds) {
    if (root == NULL) return;
    thuThapLNR(root->trai, ds);
    ds.push_back(root->data);
    thuThapLNR(root->phai, ds);
}

void timTheoTen(Node* root, string tuKhoa, int& dem) {
    if (root == NULL) return;
    timTheoTen(root->trai, tuKhoa, dem);
    if (toLowerStr(root->data.ten).find(toLowerStr(tuKhoa)) != string::npos) {
        inMotSach(root->data);
        dem++;
    }
    timTheoTen(root->phai, tuKhoa, dem);
}

void timTheoTacGia(Node* root, string tuKhoa, int& dem) {
    if (root == NULL) return;
    timTheoTacGia(root->trai, tuKhoa, dem);
    if (toLowerStr(root->data.tacGia).find(toLowerStr(tuKhoa)) != string::npos) {
        inMotSach(root->data);
        dem++;
    }
    timTheoTacGia(root->phai, tuKhoa, dem);
}


