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

int demSach(Node* root) {
    if (root == NULL) return 0;
    return 1 + demSach(root->trai) + demSach(root->phai);
}

int tongSoLuong(Node* root) {
    if (root == NULL) return 0;
    return root->data.soLuong + tongSoLuong(root->trai) + tongSoLuong(root->phai);
}

int tongLuotMuon(Node* root) {
    if (root == NULL) return 0;
    return root->data.luotMuon + tongLuotMuon(root->trai) + tongLuotMuon(root->phai);
}

int demSapHet(Node* root, int nguong) {
    if (root == NULL) return 0;
    int dem = (root->data.soLuong <= nguong) ? 1 : 0;
    return dem + demSapHet(root->trai, nguong) + demSapHet(root->phai, nguong);
}

void timSachMoiNhatCuNhat(Node* root, Sach*& moiNhat, Sach*& cuNhat) {
    if (root == NULL) return;
    if (moiNhat == NULL || root->data.namXuatBan > moiNhat->namXuatBan) moiNhat = &root->data;
    if (cuNhat == NULL || root->data.namXuatBan < cuNhat->namXuatBan) cuNhat = &root->data;
    timSachMoiNhatCuNhat(root->trai, moiNhat, cuNhat);
    timSachMoiNhatCuNhat(root->phai, moiNhat, cuNhat);
}

void thongKeTheoTacGiaFull(Node* root, map<string, vector<int> >& tk) {
    if (root == NULL) return;
    thongKeTheoTacGiaFull(root->trai, tk);
    tk[root->data.tacGia].resize(3, 0);
    tk[root->data.tacGia][0]++;
    tk[root->data.tacGia][1] += root->data.soLuong;
    tk[root->data.tacGia][2] += root->data.luotMuon;
    thongKeTheoTacGiaFull(root->phai, tk);
}

void thongKeTheoNXB(Node* root, map<string, vector<int> >& tk) {
    if (root == NULL) return;
    thongKeTheoNXB(root->trai, tk);
    tk[root->data.nhaXuatBan].resize(3, 0);
    tk[root->data.nhaXuatBan][0]++;
    tk[root->data.nhaXuatBan][1] += root->data.soLuong;
    tk[root->data.nhaXuatBan][2] += root->data.luotMuon;
    thongKeTheoNXB(root->phai, tk);
}

int timLuotMuonLonNhat(Node* root) {
    if (root == NULL) return -1;
    int maxTrai = timLuotMuonLonNhat(root->trai);
    int maxPhai = timLuotMuonLonNhat(root->phai);
    int maxCon = max(maxTrai, maxPhai);
    return max(root->data.luotMuon, maxCon);
}

void inSachMuonNhieuNhat(Node* root, int maxMuon, int& dem) {
    if (root == NULL) return;
    inSachMuonNhieuNhat(root->trai, maxMuon, dem);
    if (root->data.luotMuon == maxMuon) {
        inMotSachBang(root->data);
        dem++;
    }
    inSachMuonNhieuNhat(root->phai, maxMuon, dem);
}

void hienThiSachMuonNhieuNhat(Node* root) {
    if (root == NULL) { cout << "Danh muc rong.\n"; return; }
    int maxMuon = timLuotMuonLonNhat(root);
    if (maxMuon == 0) {
        cout << "Chua co sach nao duoc muon.\n";
        return;
    }
    int dem = 0;
    cout << ">> Sach duoc muon nhieu nhat (" << maxMuon << " luot):\n";
    inTieuDeBang();
    inSachMuonNhieuNhat(root, maxMuon, dem);
    cout << "Tong cong: " << dem << " sach.\n";
}

void top3MuonNhieuNhat(Node* root) {
    vector<Sach> ds;
    thuThapLNR(root, ds);
    if (ds.empty()) return;
    for (size_t i = 0; i < ds.size(); ++i)
        for (size_t j = i + 1; j < ds.size(); ++j)
            if (ds[j].luotMuon > ds[i].luotMuon)
                swap(ds[i], ds[j]);
    int n = (ds.size() < 3) ? (int)ds.size() : 3;
    cout << "\n--- Top " << n << " sach duoc muon nhieu nhat ---\n";
    inTieuDeBang();
    for (int i = 0; i < n; ++i) inMotSachBang(ds[i]);
}

int timSoLuongLonNhat(Node* root) {
    if (root == NULL) return -1;
    int maxTrai = timSoLuongLonNhat(root->trai);
    int maxPhai = timSoLuongLonNhat(root->phai);
    int maxCon = max(maxTrai, maxPhai);
    return max(root->data.soLuong, maxCon);
}

void inSachSoLuongLonNhat(Node* root, int maxSL, int& dem) {
    if (root == NULL) return;
    inSachSoLuongLonNhat(root->trai, maxSL, dem);
    if (root->data.soLuong == maxSL) {
        inMotSachBang(root->data);
        dem++;
    }
    inSachSoLuongLonNhat(root->phai, maxSL, dem);
}

int timSoLuongNhoNhat(Node* root) {
    if (root == NULL) return -1;
    int minTrai = timSoLuongNhoNhat(root->trai);
    int minPhai = timSoLuongNhoNhat(root->phai);
    int minCon = -1;
    if (minTrai != -1 && minPhai != -1) minCon = min(minTrai, minPhai);
    else if (minTrai != -1) minCon = minTrai;
    else if (minPhai != -1) minCon = minPhai;
    if (minCon == -1) return root->data.soLuong;
    return min(root->data.soLuong, minCon);
}
void muonSach(Node* root, string isbn, int soLuongMuon) {
    Node* p = timKiem(root, isbn);
    if (p == NULL) { cout << "Khong tim thay sach!\n"; return; }
    if (p->data.soLuong < soLuongMuon) {
        cout << "Khong du sach de muon. Hien con: " << p->data.soLuong << '\n';
        return;
    }
    p->data.soLuong -= soLuongMuon;
    p->data.luotMuon += soLuongMuon;
    cout << "Muon sach thanh cong!\n";
    cout << "  So luong con lai: " << p->data.soLuong << '\n';
    cout << "  Tong luot muon: " << p->data.luotMuon << '\n';
}

void traSach(Node* root, string isbn, int soLuongTra) {
    Node* p = timKiem(root, isbn);
    if (p == NULL) { cout << "Khong tim thay sach!\n"; return; }
    if (soLuongTra > p->data.luotMuon) {
        cout << "So luong tra (" << soLuongTra << ") vuot qua tong luot muon ("
             << p->data.luotMuon << ").\n";
        cout << "Van tiep tuc tra? (y/n): ";
        string tl; getline(cin, tl);
        if (trim(tl) != "y" && trim(tl) != "Y") { cout << "Da huy.\n"; return; }
    }
    p->data.soLuong += soLuongTra;
    p->data.luotMuon -= soLuongTra;
    if (p->data.luotMuon < 0) p->data.luotMuon = 0;
    cout << "Tra sach thanh cong!\n";
    cout << "  So luong hien tai: " << p->data.soLuong << '\n';
    cout << "  Tong luot muon: " << p->data.luotMuon << '\n';
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
    p->data.namXuatBan = nhapNamXuatBan();
    p->data.soLuong = nhapSoLuongKhongAm();
    cout << "Cap nhat thanh cong.\n";
}

vector<string> tachCSV(const string& line) {
    vector<string> res;
    string token;
    stringstream ss(line);
    while (getline(ss, token, ',')) res.push_back(trim(token));
    return res;
}

bool docFile(Node*& root, const string& tenFile) {
    ifstream f(tenFile.c_str());
    if (!f.is_open()) return false;
    string line;
    int dem = 0, loi = 0;
    while (getline(f, line)) {
        if (trim(line).empty()) continue;
        vector<string> p = tachCSV(line);
        if (p.size() != 7 || !laSo(p[0])) { loi++; continue; }
        try {
            Sach s;
            s.isbn = p[0];
            s.ten = p[1];
            s.tacGia = p[2];
            s.nhaXuatBan = p[3];
            s.namXuatBan = stringToInt(p[4]);
            s.soLuong = stringToInt(p[5]);
            s.luotMuon = stringToInt(p[6]);
            bool ok = false;
            root = themSach(root, s, ok);
            if (ok) dem++; else loi++;
        } catch (...) { loi++; }
    }
    f.close();
    cout << "Da doc " << dem << " sach tu " << tenFile << " (" << loi << " dong loi).\n";
    return true;
}

void luuFile(Node* root, const string& tenFile) {
    ofstream f(tenFile.c_str());
    if (!f.is_open()) {
        cout << "CANH BAO: Khong mo duoc file " << tenFile << " de luu!\n";
        return;
    }
    vector<Sach> ds;
    thuThapLNR(root, ds);
    for (size_t i = 0; i < ds.size(); ++i) {
        f << ds[i].isbn << ',' << ds[i].ten << ',' << ds[i].tacGia << ','
          << ds[i].nhaXuatBan << ',' << ds[i].namXuatBan << ','
          << ds[i].soLuong << ',' << ds[i].luotMuon << '\n';
    }
    f.close();
    cout << "Da luu " << ds.size() << " sach vao " << tenFile << ".\n";
}

void khoiTaoDuLieuMau(Node*& root) {
    bool thanhCong = false;
    Sach ds[] = {
        {"5001", "Sach 50", "Nguyen Van A", "NXB Giao Duc", 2022, 5, 1},
        {"3002", "Sach 30", "Tran Thi B", "NXB Dai Hoc", 2023, 3, 1},
        {"7001", "Sach 70", "Le Van C", "NXB Tre", 2021, 4, 0},
        {"2007", "Sach 20", "Pham Thi D", "NXB Lao Dong", 2020, 7, 3},
        {"4004", "Sach 40", "Nguyen Van A", "NXB Giao Duc", 2024, 6, 2}
    };
    for (int i = 0; i < 5; ++i) {
        root = themSach(root, ds[i], thanhCong);
    }
    cout << "Da khoi tao 5 sach mau (lan dau chay).\n";
}

void nhapVaThem(Node*& root) {
    Sach s;
    while (true) {
        s.isbn = nhapChuoi("ISBN: ");
        if (!laSo(s.isbn)) { cout << "ISBN chi duoc chua chu so!\n"; continue; }
        break;
    }

    Node* existingNode = timKiem(root, s.isbn);
    if (existingNode != NULL) {
        cout << "ISBN nay da ton tai trong he thong!\n";
        inMotSach(existingNode->data);
        char choice;
        cout << "Ban co muon tang so luong cho sach nay khong? (Y/N): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == 'y' || choice == 'Y') {
            int slThem = nhapSoLuongKhongAm();
            existingNode->data.soLuong += slThem;
            cout << "Da tang so luong thanh cong! Tong so luong hien tai: " << existingNode->data.soLuong << '\n';
        } else {
            cout << "Huy thao tac them sach.\n";
        }
        return;
    }

    s.ten = nhapChuoi("Ten sach: ");

    if (daCoTenSach(root, s.ten)) {
        cout << "CANH BAO: Da co sach cung ten trong thu vien!\n";
        cout << "Van tiep tuc them? (y/n): ";
        string tl; getline(cin, tl);
        if (trim(tl) != "y" && trim(tl) != "Y") {
            cout << "Da huy them sach.\n";
            return;
        }
    }

    s.tacGia = nhapChuoi("Tac gia: ");
    s.nhaXuatBan = nhapChuoi("Nha xuat ban: ");
    s.namXuatBan = nhapNamXuatBan();
    s.soLuong = nhapSoLuongKhongAm();
    s.luotMuon = 0;
    
    bool thanhCong = false;
    root = themSach(root, s, thanhCong);
    cout << (thanhCong ? "Them sach thanh cong.\n" : "Them that bai.\n");
}

void hienThiMenu() {
    cout << "\n===== QUAN LY DANH MUC SACH THU VIEN =====\n";
    cout << " 1. Them sach\n";
    cout << " 2. Tim sach theo ISBN\n";
    cout << " 3. Tim sach theo ten\n";
    cout << " 4. Tim sach theo tac gia\n";
    cout << " 5. Cap nhat sach\n";
    cout << " 6. Xoa sach theo ISBN\n";
    cout << " 7. Muon sach\n";
    cout << " 8. Tra sach\n";
    cout << " 9. Duyet cay NLR\n";
    cout << "10. Duyet cay LRN\n";
    cout << "11. Hien thi dang bang\n";
    cout << "12. Thong ke tong quan\n";
    cout << "13. Sach moi nhat / cu nhat\n";
    cout << "14. Sach co so luong nhieu nhat\n";
    cout << "15. Sach duoc muon nhieu nhat\n";
    cout << " 0. Thoat (tu dong luu file)\n";
}

void xuLyLuaChon(int chon, Node*& root) {
    switch (chon) {
    case 1: nhapVaThem(root); break;
    case 2: {
        string isbn = nhapChuoi("Nhap ISBN can tim: ");
        Node* p = timKiem(root, isbn);
        if (p == NULL) cout << "Khong tim thay sach!\n";
        else inMotSach(p->data);
        break;
    }
    case 3: {
        string key = nhapChuoi("Nhap ten hoac tu khoa: ");
        int dem = 0; timTheoTen(root, key, dem);
        if (dem == 0) cout << "Khong co sach phu hop.\n";
        break;
    }
    case 4: {
        string key = nhapChuoi("Nhap ten tac gia hoac tu khoa: ");
        int dem = 0; timTheoTacGia(root, key, dem);
        if (dem == 0) cout << "Khong co sach phu hop.\n";
        break;
    }
    case 5: {
        string isbn = nhapChuoi("Nhap ISBN can cap nhat: ");
        capNhatSach(root, isbn);
        break;
    }
    case 6: {
        string isbn = nhapChuoi("Nhap ISBN can xoa: ");
        cout << "Xac nhan xoa? (y/n): ";
        string tl; getline(cin, tl);
        if (trim(tl) != "y" && trim(tl) != "Y") { cout << "Da huy.\n"; break; }
        bool ok = false;
        root = xoaSach(root, isbn, ok);
        cout << (ok ? "Xoa thanh cong.\n" : "Khong tim thay ISBN.\n");
        break;
    }
    case 7: {
        string isbn = nhapChuoi("Nhap ISBN can muon: ");
        int sl = nhapSoNguyen("So luong muon: ");
        if (sl <= 0) { cout << "So luong phai > 0!\n"; break; }
        muonSach(root, isbn, sl);
        break;
    }
    case 8: {
        string isbn = nhapChuoi("Nhap ISBN can tra: ");
        int sl = nhapSoNguyen("So luong tra: ");
        if (sl <= 0) { cout << "So luong phai > 0!\n"; break; }
        traSach(root, isbn, sl);
        break;
    }
    case 9:
        if (root == NULL) cout << "Danh muc rong.\n"; else duyetNLR(root);
        break;
    case 10:
        if (root == NULL) cout << "Danh muc rong.\n"; else duyetLRN(root);
        break;
    case 11:
        if (root == NULL) cout << "Danh muc rong.\n";
        else { inTieuDeBang(); duyetLNRTable(root); }
        break;
    case 12: {
        if (root == NULL) { cout << "Danh muc rong.\n"; break; }
        cout << "\n========== THONG KE TONG QUAN ==========\n";
        int soDauSach = demSach(root);
        int tongSL = tongSoLuong(root);
        int tongMuon = tongLuotMuon(root);
        cout << "\n[1] TONG QUAN\n";
        cout << "  - So dau sach:            " << soDauSach << '\n';
        cout << "  - Tong so ban sach:       " << tongSL << '\n';
        cout << "  - Tong luot muon:         " << tongMuon << '\n';
        cout << "  - So ban con trong kho:   " << tongSL << '\n';
        cout << "  - So ban dang duoc muon:  " << tongMuon << '\n';

        int soSapHet = demSapHet(root, 2);
        int soHetHan = demSapHet(root, 0);
        cout << "\n[2] TINH TRANG KHO\n";
        cout << "  - Sach sap het (SL <= 2): " << soSapHet << " dau sach\n";
        cout << "  - Sach da het (SL = 0):   " << soHetHan << " dau sach\n";
        if (soDauSach > 0 && (tongSL + tongMuon) > 0) {
            cout << fixed << setprecision(1);
            double tiLe = 100.0 * tongMuon / (tongSL + tongMuon);
            cout << "  - Ti le dang duoc muon:   " << tiLe << "%\n";
            cout.unsetf(ios::fixed);
        }

        cout << "\n[3] THONG KE THEO TAC GIA\n";
        map<string, vector<int> > tkTG;
        thongKeTheoTacGiaFull(root, tkTG);
        cout << left << setw(28) << "Tac gia" << setw(10) << "So dau" << setw(12) << "Tong SL" << "Luot muon\n";
        cout << string(60, '-') << '\n';
        for (map<string, vector<int> >::iterator it = tkTG.begin(); it != tkTG.end(); ++it) {
            cout << left << setw(28) << it->first << setw(10) << it->second[0]
                 << setw(12) << it->second[1] << it->second[2] << '\n';
        }

        cout << "\n[4] THONG KE THEO NHA XUAT BAN\n";
        map<string, vector<int> > tkNXB;
        thongKeTheoNXB(root, tkNXB);
        cout << left << setw(28) << "Nha xuat ban" << setw(10) << "So dau" << setw(12) << "Tong SL" << "Luot muon\n";
        cout << string(60, '-') << '\n';
        for (map<string, vector<int> >::iterator it = tkNXB.begin(); it != tkNXB.end(); ++it) {
            cout << left << setw(28) << it->first << setw(10) << it->second[0]
                 << setw(12) << it->second[1] << it->second[2] << '\n';
        }

        top3MuonNhieuNhat(root);

        cout << "\n[6] THONG KE NOI BAT\n";
        Sach* moiNhat = NULL; Sach* cuNhat = NULL;
        timSachMoiNhatCuNhat(root, moiNhat, cuNhat);
        if (moiNhat) cout << "  - Sach moi nhat:      " << moiNhat->ten << " (" << moiNhat->namXuatBan << ")\n";
        if (cuNhat)  cout << "  - Sach cu nhat:       " << cuNhat->ten << " (" << cuNhat->namXuatBan << ")\n";
        int maxSL = timSoLuongLonNhat(root);
        int minSL = timSoLuongNhoNhat(root);
        cout << "  - SL nhieu nhat:      " << maxSL << " cuon\n";
        cout << "  - SL it nhat:         " << minSL << " cuon\n";
        if (tongMuon == 0) cout << "  - Chua co sach nao duoc muon.\n";
        else {
            int maxMuon = timLuotMuonLonNhat(root);
            cout << "  - Luot muon cao nhat: " << maxMuon << " luot\n";
        }
        cout << "\n=========================================\n";
        break;
    }
    case 13: {
        if (root == NULL) { cout << "Danh muc rong.\n"; break; }
        Sach* moiNhat = NULL; Sach* cuNhat = NULL;
        timSachMoiNhatCuNhat(root, moiNhat, cuNhat);
        cout << ">> Sach moi nhat:\n"; inMotSach(*moiNhat);
        cout << ">> Sach cu nhat:\n"; inMotSach(*cuNhat);
        break;
    }
    case 14: {
        if (root == NULL) { cout << "Danh muc rong.\n"; break; }
        int maxSL = timSoLuongLonNhat(root);
        int dem = 0;
        cout << ">> Sach co so luong nhieu nhat (" << maxSL << " cuon):\n";
        inTieuDeBang();
        inSachSoLuongLonNhat(root, maxSL, dem);
        cout << "Tong cong: " << dem << " sach.\n";
        break;
    }
    case 15: hienThiSachMuonNhieuNhat(root); break;
    case 0: cout << "Dang luu du lieu va giai phong bo nho...\n"; break;
    default: cout << "Lua chon khong hop le!\n";
    }
}

int main() {
    Node* root = NULL;

    if (!docFile(root, TEN_FILE)) {
        khoiTaoDuLieuMau(root);
        luuFile(root, TEN_FILE);
    }

    int chon;
    do {
        hienThiMenu();
        chon = nhapSoNguyen("Nhap lua chon: ");
        xuLyLuaChon(chon, root);
    } while (chon != 0);

    luuFile(root, TEN_FILE);
    giaiPhong(root);
    cout << "Ket thuc chuong trinh.\n";
    return 0;
}
