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

void inMotSach(Sach s) {
    cout << "ISBN: " << s.isbn << '\n';
    cout << "Ten sach: " << s.ten << '\n';
    cout << "Tac gia: " << s.tacGia << '\n';
    cout << "Nha xuat ban: " << s.nhaXuatBan << '\n';
    cout << "Nam xuat ban: " << s.namXuatBan << '\n';
    cout << "So luong: " << s.soLuong << '\n';
    cout << "-----------------------------\n";
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
 

