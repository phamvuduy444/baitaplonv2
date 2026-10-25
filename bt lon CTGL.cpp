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
 
void duyetNLR(Node* root) {
    if (root == NULL) return;
    inMotSach(root->data);
    duyetNLR(root->trai);
    duyetNLR(root->phai);
}
 
void duyetLNR(Node* root) {
    if (root == NULL) return;
    duyetLNR(root->trai);
    inMotSach(root->data);
    duyetLNR(root->phai);
}
 
void duyetLRN(Node* root) {
    if (root == NULL) return;
    duyetLRN(root->trai);
    duyetLRN(root->phai);
    inMotSach(root->data);
}
 
void timTheoTen(Node* root, string tuKhoa, int& dem) {
    if (root == NULL) return;
    timTheoTen(root->trai, tuKhoa, dem);
    if (root->data.ten.find(tuKhoa) != string::npos) {
        inMotSach(root->data);
        dem++;
    }
    timTheoTen(root->phai, tuKhoa, dem);
}
 
void timTheoTacGia(Node* root, string tuKhoa, int& dem) {
    if (root == NULL) return;
    timTheoTacGia(root->trai, tuKhoa, dem);
    if (root->data.tacGia.find(tuKhoa) != string::npos) {
        inMotSach(root->data);
        dem++;
    }
    timTheoTacGia(root->phai, tuKhoa, dem);
}
 
int demSach(Node* root) {
    if (root == NULL) return 0;
    return 1 + demSach(root->trai) + demSach(root->phai);
}
 
int tongSoLuong(Node* root) {
    if (root == NULL) return 0;
    return root->data.soLuong
         + tongSoLuong(root->trai)
         + tongSoLuong(root->phai);
}
 
void capNhatSach(Node* root, string isbn) {
    Node* p = timKiem(root, isbn);
    if (p == NULL) {
        cout << "Khong tim thay sach!\n";
        return;
    }
    cout << "Nhap thong tin moi (ISBN duoc giu nguyen).\n";
    p->data.ten = nhapChuoi("Ten sach: ");
    p->data.tacGia = nhapChuoi("Tac gia: ");
    p->data.nhaXuatBan = nhapChuoi("Nha xuat ban: ");
    p->data.namXuatBan = nhapSoNguyen("Nam xuat ban: ");
    p->data.soLuong = nhapSoNguyen("So luong: ");
    if (p->data.soLuong < 0) {
        cout << "So luong am khong hop le, dat ve 0.\n";
        p->data.soLuong = 0;
    }
    cout << "Cap nhat thanh cong.\n";
}
 
void giaiPhong(Node* root) {
    if (root == NULL) return;
    giaiPhong(root->trai);
    giaiPhong(root->phai);
    delete root;
}
 
void nhapVaThem(Node*& root) {
    Sach s;
    s.isbn = nhapChuoi("ISBN: ");
    if (s.isbn.empty()) {
        cout << "ISBN khong duoc de trong!\n";
        return;
    }
    if (timKiem(root, s.isbn) != NULL) {
        cout << "ISBN da ton tai, khong the them!\n";
        return;
    }
    s.ten = nhapChuoi("Ten sach: ");
    s.tacGia = nhapChuoi("Tac gia: ");
    s.nhaXuatBan = nhapChuoi("Nha xuat ban: ");
    s.namXuatBan = nhapSoNguyen("Nam xuat ban: ");
    s.soLuong = nhapSoNguyen("So luong: ");
    if (s.soLuong < 0) {
        cout << "So luong am khong hop le!\n";
        return;
    }
    bool thanhCong = false;
    root = themSach(root, s, thanhCong);
    if (thanhCong) cout << "Them sach thanh cong.\n";
    else cout << "ISBN da ton tai.\n";
}
 
void hienThiMenu() {
    cout << "\n===== QUAN LY DANH MUC SACH THU VIEN =====\n";
    cout << "1. Them sach\n";
    cout << "2. Tim sach theo ISBN\n";
    cout << "3. Tim sach theo ten\n";
    cout << "4. Tim sach theo tac gia\n";
    cout << "5. Cap nhat sach\n";
    cout << "6. Xoa sach\n";
    cout << "7. Hien thi danh sach theo ISBN tang dan (LNR)\n";
    cout << "8. Duyet cay NLR\n";
    cout << "9. Duyet cay LRN\n";
    cout << "10. Thong ke\n";
    cout << "0. Thoat\n";
}
 
int main() {
    Node* root = NULL;
    int chon;
    do {
        hienThiMenu();
        chon = nhapSoNguyen("Nhap lua chon: ");
        if (chon == 1) {
            nhapVaThem(root);
        } else if (chon == 2) {
            string isbn = nhapChuoi("Nhap ISBN can tim: ");
            Node* p = timKiem(root, isbn);
            if (p == NULL) cout << "Khong tim thay sach!\n";
            else inMotSach(p->data);
        } else if (chon == 3) {
            string key = nhapChuoi("Nhap ten hoac tu khoa: ");
            int dem = 0;
            timTheoTen(root, key, dem);
            if (dem == 0) cout << "Khong co sach phu hop.\n";
        } else if (chon == 4) {
            string key = nhapChuoi("Nhap ten tac gia hoac tu khoa: ");
            int dem = 0;
            timTheoTacGia(root, key, dem);
            if (dem == 0) cout << "Khong co sach phu hop.\n";
        } else if (chon == 5) {
            string isbn = nhapChuoi("Nhap ISBN can cap nhat: ");
            capNhatSach(root, isbn);
        } else if (chon == 6) {
            string isbn = nhapChuoi("Nhap ISBN can xoa: ");
            bool daXoa = false;
            root = xoaSach(root, isbn, daXoa);
            if (daXoa) cout << "Xoa sach thanh cong.\n";
            else cout << "Khong tim thay ISBN can xoa.\n";
        } else if (chon == 7) {
            if (root == NULL) cout << "Danh muc rong.\n";
            else duyetLNR(root);
        } else if (chon == 8) {
            if (root == NULL) cout << "Danh muc rong.\n";
            else duyetNLR(root);
        } else if (chon == 9) {
            if (root == NULL) cout << "Danh muc rong.\n";
            else duyetLRN(root);
        } else if (chon == 10) {
            cout << "So dau sach: " << demSach(root) << '\n';
            cout << "Tong so ban sach: " << tongSoLuong(root) << '\n';
        } else if (chon == 0) {
            cout << "Dang giai phong bo nho...\n";
        } else {
            cout << "Lua chon khong hop le!\n";
        }
    } while (chon != 0);
    
    giaiPhong(root);
    cout << "Ket thuc chuong trinh.\n";
    return 0;
}
 giaiPhong(root);
    cout << "Ket thuc chuong trinh.\n";
    return 0;
}
