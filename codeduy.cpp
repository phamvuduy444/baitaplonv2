#include <iostream>
#include <string>
#include <limits>
using namespace std;
 
struct Sach {
    string isbn;
    string ten;
    string tacGia;
    string nhaXuatBan;
    int namXuatBan;
    int soLuong;
};
 
struct Node {
    Sach data;
    Node* trai;
    Node* phai;
};
 
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
 
string nhapChuoi(string thongBao) {
    string s;
    cout << thongBao;
    getline(cin, s);
    return s;
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

