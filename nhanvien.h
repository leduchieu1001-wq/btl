#ifndef NHANVIEN1_H
#define NHANVIEN1_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

// ============================================================
// Helper noi bo
// ============================================================
static vector<string> _tachChuoi(const string& line, char delim = '|') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) tokens.push_back(token);
    return tokens;
}
static string _xoaXuongDong(string s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back();
    return s;
}
static string _chuThuong(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}
static bool _ngayHopLe(const string& s) {
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}

// ============================================================
// Lop co so: NhanVien
// Nguoi code: Ngo Van Thinh
// Trat tu luu file: loai|ma|ho ten|SDT|ngay sinh|luong co ban|thong so rieng
// ============================================================
class NhanVien {
protected:
    string ma;
    string hoTen;
    string sdt;
    string ngaySinh;
    double luongCoBan;

public:
    NhanVien() : ma(""), hoTen(""), sdt(""), ngaySinh(""), luongCoBan(0) {}
    NhanVien(string ma, string hoTen, string sdt, string ngaySinh, double luongCoBan)
        : ma(ma), hoTen(hoTen), sdt(sdt), ngaySinh(ngaySinh), luongCoBan(luongCoBan) {}
    virtual ~NhanVien() {}

    string getMa()         const { return ma; }
    string getHoTen()      const { return hoTen; }
    string getSdt()        const { return sdt; }
    string getNgaySinh()   const { return ngaySinh; }
    double getLuongCoBan() const { return luongCoBan; }

    void setMa(string m)           { ma = m; }
    void setHoTen(string ht)       { hoTen = ht; }
    void setSdt(string s)          { sdt = s; }
    void setNgaySinh(string ns)    { ngaySinh = ns; }
    void setLuongCoBan(double lcb) { luongCoBan = lcb; }

    virtual double tinhLuong()               const = 0;
    virtual string getLoai()                 const = 0;
    virtual string ghiRaDong()               const = 0;
    virtual bool   docTuDong(const string&)        = 0;

    virtual void nhap() {
        cout << "  Ma (NVxxx)    : "; getline(cin, ma);
        cout << "  Ho ten        : "; getline(cin, hoTen);
        cout << "  So dien thoai : "; getline(cin, sdt);
        string ns;
        do {
            cout << "  Ngay sinh (dd/mm/yyyy): ";
            getline(cin, ns);
            if (!_ngayHopLe(ns)) cout << "  [LOI] Dinh dang sai. Nhap lai.\n";
        } while (!_ngayHopLe(ns));
        ngaySinh = ns;
        cout << "  Luong co ban  : ";
        string tmp; getline(cin, tmp);
        luongCoBan = stod(tmp);
    }

    virtual void xuat() const {
        cout << "  Ma       : " << ma << "\n";
        cout << "  Ho ten   : " << hoTen << "\n";
        cout << "  SDT      : " << sdt << "\n";
        cout << "  Ngay sinh: " << ngaySinh << "\n";
        cout << "  Luong CB : " << (long long)luongCoBan << "\n";
    }
};


// ============================================================
// HDV - Huong Dan Vien
// Luong = luong co ban + 200000 x so tour da dan
// File: HDV|ma|ho ten|SDT|ngay sinh|luong co ban|so tour da dan
// ============================================================
class HDV : public NhanVien {
private:
    int soTourDaDan;
public:
    HDV() : NhanVien(), soTourDaDan(0) {}
    HDV(string ma, string hoTen, string sdt, string ngaySinh,
        double luongCoBan, int soTourDaDan)
        : NhanVien(ma, hoTen, sdt, ngaySinh, luongCoBan),
          soTourDaDan(soTourDaDan) {}

    int  getSoTourDaDan() const        { return soTourDaDan; }
    void setSoTourDaDan(int st)        { soTourDaDan = st; }
    double tinhLuong()    const override { return luongCoBan + 200000.0 * soTourDaDan; }
    string getLoai()      const override { return "HDV"; }

    void nhap() override {
        cout << "=== Nhap Huong Dan Vien ===\n";
        NhanVien::nhap();
        cout << "  So tour da dan: "; string tmp; getline(cin, tmp);
        soTourDaDan = stoi(tmp);
    }
    void xuat() const override {
        cout << "--- [HDV] ---\n";
        NhanVien::xuat();
        cout << "  So tour dan: " << soTourDaDan << "\n";
        cout << "  Luong thuc : " << (long long)tinhLuong() << "\n";
    }
    string ghiRaDong() const override {
        return "HDV|" + ma + "|" + hoTen + "|" + sdt + "|" + ngaySinh
             + "|" + to_string((long long)luongCoBan)
             + "|" + to_string(soTourDaDan);
    }
    bool docTuDong(const string& line) override {
        string s = _xoaXuongDong(line);
        vector<string> t = _tachChuoi(s);
        if (t.size() < 7 || t[0] != "HDV") return false;
        if (t[1].size() < 3 || t[1].substr(0,2) != "NV") return false;
        if (!_ngayHopLe(t[4])) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = stod(t[5]); soTourDaDan = stoi(t[6]);
        return true;
    }
};


// ============================================================
// SALES - Nhan Vien Kinh Doanh
// Luong = luong co ban + hoaHong% x tong doanh so hoa don da lap
// File: SALES|ma|ho ten|SDT|ngay sinh|luong co ban|hoa hong%
// (tongDoanhSo khong luu file, cap nhat runtime tu module HoaDon)
// ============================================================
class SALES : public NhanVien {
private:
    double phanTramHoaHong;
    double tongDoanhSo;
public:
    SALES() : NhanVien(), phanTramHoaHong(0), tongDoanhSo(0) {}
    SALES(string ma, string hoTen, string sdt, string ngaySinh,
          double luongCoBan, double phanTramHoaHong)
        : NhanVien(ma, hoTen, sdt, ngaySinh, luongCoBan),
          phanTramHoaHong(phanTramHoaHong), tongDoanhSo(0) {}

    double getPhanTramHoaHong() const      { return phanTramHoaHong; }
    void   setPhanTramHoaHong(double p)    { phanTramHoaHong = p; }
    double getTongDoanhSo()     const      { return tongDoanhSo; }
    void   setTongDoanhSo(double d)        { tongDoanhSo = d; }
    void   themDoanhSo(double soTien)      { tongDoanhSo += soTien; } // goi tu HoaDon

    double tinhLuong() const override {
        return luongCoBan + (phanTramHoaHong / 100.0) * tongDoanhSo;
    }
    string getLoai() const override { return "SALES"; }

    void nhap() override {
        cout << "=== Nhap SALES ===\n";
        NhanVien::nhap();
        cout << "  Phan tram hoa hong (%): "; string tmp; getline(cin, tmp);
        phanTramHoaHong = stod(tmp); tongDoanhSo = 0;
    }
    void xuat() const override {
        cout << "--- [SALES] ---\n";
        NhanVien::xuat();
        cout << "  Hoa hong   : " << phanTramHoaHong << "%\n";
        cout << "  Doanh so   : " << (long long)tongDoanhSo << "\n";
        cout << "  Luong thuc : " << (long long)tinhLuong() << "\n";
    }
    string ghiRaDong() const override {
        return "SALES|" + ma + "|" + hoTen + "|" + sdt + "|" + ngaySinh
             + "|" + to_string((long long)luongCoBan)
             + "|" + to_string(phanTramHoaHong);
    }
    bool docTuDong(const string& line) override {
        string s = _xoaXuongDong(line);
        vector<string> t = _tachChuoi(s);
        if (t.size() < 7 || t[0] != "SALES") return false;
        if (t[1].size() < 3 || t[1].substr(0,2) != "NV") return false;
        if (!_ngayHopLe(t[4])) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = stod(t[5]); phanTramHoaHong = stod(t[6]);
        tongDoanhSo = 0;
        return true;
    }
};


// ============================================================
// TN - Truong Nhom
// Luong = luong co ban
// File: TN|ma|ho ten|SDT|ngay sinh|luong co ban
// ============================================================
class TN : public NhanVien {
public:
    TN() : NhanVien() {}
    TN(string ma, string hoTen, string sdt, string ngaySinh, double luongCoBan)
        : NhanVien(ma, hoTen, sdt, ngaySinh, luongCoBan) {}

    double tinhLuong() const override { return luongCoBan; }
    string getLoai()   const override { return "TN"; }

    void nhap() override {
        cout << "=== Nhap Truong Nhom ===\n";
        NhanVien::nhap();
    }
    void xuat() const override {
        cout << "--- [TN] ---\n";
        NhanVien::xuat();
        cout << "  Luong thuc : " << (long long)tinhLuong() << "\n";
    }
    string ghiRaDong() const override {
        return "TN|" + ma + "|" + hoTen + "|" + sdt + "|" + ngaySinh
             + "|" + to_string((long long)luongCoBan);
    }
    bool docTuDong(const string& line) override {
        string s = _xoaXuongDong(line);
        vector<string> t = _tachChuoi(s);
        if (t.size() < 6 || t[0] != "TN") return false;
        if (t[1].size() < 3 || t[1].substr(0,2) != "NV") return false;
        if (!_ngayHopLe(t[4])) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = stod(t[5]);
        return true;
    }
};


// ============================================================
// QT - Quan Tri
// Luong = luong co ban
// File: QT|ma|ho ten|SDT|ngay sinh|luong co ban
// ============================================================
class QT : public NhanVien {
public:
    QT() : NhanVien() {}
    QT(string ma, string hoTen, string sdt, string ngaySinh, double luongCoBan)
        : NhanVien(ma, hoTen, sdt, ngaySinh, luongCoBan) {}

    double tinhLuong() const override { return luongCoBan; }
    string getLoai()   const override { return "QT"; }

    void nhap() override {
        cout << "=== Nhap Quan Tri ===\n";
        NhanVien::nhap();
    }
    void xuat() const override {
        cout << "--- [QT] ---\n";
        NhanVien::xuat();
        cout << "  Luong thuc : " << (long long)tinhLuong() << "\n";
    }
    string ghiRaDong() const override {
        return "QT|" + ma + "|" + hoTen + "|" + sdt + "|" + ngaySinh
             + "|" + to_string((long long)luongCoBan);
    }
    bool docTuDong(const string& line) override {
        string s = _xoaXuongDong(line);
        vector<string> t = _tachChuoi(s);
        if (t.size() < 6 || t[0] != "QT") return false;
        if (t[1].size() < 3 || t[1].substr(0,2) != "NV") return false;
        if (!_ngayHopLe(t[4])) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = stod(t[5]);
        return true;
    }
};


// ============================================================
// DanhSachNhanVien - Quan ly toan bo nhan vien
// ============================================================
class DanhSachNhanVien {
private:
    vector<NhanVien*> ds;

    bool maTonTai(const string& ma) const {
        for (const NhanVien* nv : ds)
            if (nv->getMa() == ma) return true;
        return false;
    }

    NhanVien* taoTuDong(const string& line) const {
        string loai = line.substr(0, line.find('|'));
        NhanVien* nv = nullptr;
        if      (loai == "HDV")   nv = new HDV();
        else if (loai == "SALES") nv = new SALES();
        else if (loai == "TN")    nv = new TN();
        else if (loai == "QT")    nv = new QT();
        else return nullptr;
        if (!nv->docTuDong(line)) { delete nv; return nullptr; }
        return nv;
    }

public:
    DanhSachNhanVien() {}
    ~DanhSachNhanVien() {
        for (NhanVien* nv : ds) delete nv;
        ds.clear();
    }

    bool them(NhanVien* nv) {
        if (!nv) return false;
        if (maTonTai(nv->getMa())) {
            cout << "  [LOI] Ma '" << nv->getMa() << "' da ton tai!\n";
            return false;
        }
        ds.push_back(nv); return true;
    }

    bool xoa(const string& ma) {
        for (auto it = ds.begin(); it != ds.end(); ++it) {
            if ((*it)->getMa() == ma) { delete *it; ds.erase(it); return true; }
        }
        return false;
    }

    NhanVien* timTheoMa(const string& ma) const {
        for (NhanVien* nv : ds) if (nv->getMa() == ma) return nv;
        return nullptr;
    }

    vector<NhanVien*> timTheoTen(const string& tuKhoa) const {
        vector<NhanVien*> kq;
        string tk = _chuThuong(tuKhoa);
        for (NhanVien* nv : ds)
            if (_chuThuong(nv->getHoTen()).find(tk) != string::npos)
                kq.push_back(nv);
        return kq;
    }

    void hienThi() const {
        if (ds.empty()) { cout << "  Danh sach trong.\n"; return; }
        cout << "====== DANH SACH NHAN VIEN (" << ds.size() << " nguoi) ======\n";
        for (const NhanVien* nv : ds) { nv->xuat(); cout << "\n"; }
    }

    void hienThiTheoLoai(const string& loai) const {
        bool co = false;
        cout << "=== Nhan vien loai [" << loai << "] ===\n";
        for (const NhanVien* nv : ds)
            if (nv->getLoai() == loai) { nv->xuat(); cout << "\n"; co = true; }
        if (!co) cout << "  Khong co nhan vien loai " << loai << ".\n";
    }

    void hienThiBangLuong() const {
        if (ds.empty()) { cout << "  Danh sach trong.\n"; return; }
        cout << left
             << setw(8)  << "Ma"
             << setw(22) << "Ho Ten"
             << setw(7)  << "Loai"
             << setw(14) << "Luong CB"
             << setw(14) << "Luong Thuc" << "\n"
             << string(65, '-') << "\n";
        for (const NhanVien* nv : ds)
            cout << left
                 << setw(8)  << nv->getMa()
                 << setw(22) << nv->getHoTen()
                 << setw(7)  << nv->getLoai()
                 << setw(14) << (long long)nv->getLuongCoBan()
                 << setw(14) << (long long)nv->tinhLuong() << "\n";
    }

    int soLuong() const { return (int)ds.size(); }
    const vector<NhanVien*>& getDanhSach() const { return ds; }

    bool luuFile(const string& tenFile) const {
        ofstream f(tenFile);
        if (!f.is_open()) { cout << "  [LOI] Khong the mo file: " << tenFile << "\n"; return false; }
        for (const NhanVien* nv : ds) f << nv->ghiRaDong() << "\n";
        f.close();
        cout << "  Da luu " << ds.size() << " nhan vien -> " << tenFile << "\n";
        return true;
    }

    bool docFile(const string& tenFile) {
        ifstream f(tenFile);
        if (!f.is_open()) { cout << "  [LOI] Khong the mo file: " << tenFile << "\n"; return false; }
        for (NhanVien* nv : ds) delete nv; ds.clear();
        string line; int dem = 0;
        while (getline(f, line)) {
            if (line.empty() || line[0] == '\r') continue;
            NhanVien* nv = taoTuDong(line);
            if (nv) { ds.push_back(nv); dem++; }
            else cout << "  [CANH BAO] Bo qua dong loi: " << line << "\n";
        }
        f.close();
        cout << "  Da doc " << dem << " nhan vien tu " << tenFile << "\n";
        return true;
    }

    // Goi tu module HoaDon sau khi tao hoa don
    void capNhatDoanhSoSales(const string& maNV, double soTien) {
        NhanVien* nv = timTheoMa(maNV);
        if (!nv) return;
        SALES* s = dynamic_cast<SALES*>(nv);
        if (s) s->themDoanhSo(soTien);
    }
};


// ============================================================
// Menu quan ly nhan vien - Ngo Van Thinh
// File du lieu: data/nhanvien.txt
// ============================================================
const string FILE_NV = "data/nhanvien.txt";

static int _docLuaChon() {
    int n = 0; cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}

static void _luuNV(const DanhSachNhanVien& ds) { ds.luuFile(FILE_NV); }
static void _docNV(DanhSachNhanVien& ds)        { ds.docFile(FILE_NV); }

static void _themNhanVien(DanhSachNhanVien& ds) {
    cout << "\n--- THEM NHAN VIEN ---\n";
    cout << "  1.HDV  2.SALES  3.TN  4.QT\n";
    cout << "  Chon loai: ";
    int lc = _docLuaChon();
    NhanVien* nv = nullptr;
    switch (lc) {
        case 1: nv = new HDV();   break;
        case 2: nv = new SALES(); break;
        case 3: nv = new TN();    break;
        case 4: nv = new QT();    break;
        default: cout << "  Loai khong hop le!\n"; return;
    }
    nv->nhap();
    if (ds.them(nv)) { cout << "  Them thanh cong!\n"; _luuNV(ds); }
    else delete nv;
}

static void _timKiemNhanVien(const DanhSachNhanVien& ds) {
    cout << "\n--- TIM KIEM ---\n";
    cout << "  1. Theo ma   2. Theo ten\n  Lua chon: ";
    int lc = _docLuaChon();
    if (lc == 1) {
        cout << "  Nhap ma: "; string ma; getline(cin, ma);
        NhanVien* nv = ds.timTheoMa(ma);
        if (nv) { cout << "\n"; nv->xuat(); }
        else cout << "  Khong tim thay ma: " << ma << "\n";
    } else if (lc == 2) {
        cout << "  Nhap ten: "; string ten; getline(cin, ten);
        vector<NhanVien*> kq = ds.timTheoTen(ten);
        if (kq.empty()) cout << "  Khong tim thay.\n";
        else for (NhanVien* nv : kq) { nv->xuat(); cout << "\n"; }
    }
}

static void _xoaNhanVien(DanhSachNhanVien& ds) {
    cout << "\n--- XOA NHAN VIEN ---\n";
    cout << "  Nhap ma: "; string ma; getline(cin, ma);
    NhanVien* nv = ds.timTheoMa(ma);
    if (!nv) { cout << "  Khong tim thay ma: " << ma << "\n"; return; }
    nv->xuat();
    cout << "  Xac nhan xoa? (y/n): "; string xn; getline(cin, xn);
    if (xn == "y" || xn == "Y") { ds.xoa(ma); cout << "  Da xoa!\n"; _luuNV(ds); }
    else cout << "  Huy xoa.\n";
}

static void _capNhatNhanVien(DanhSachNhanVien& ds) {
    cout << "\n--- CAP NHAT ---\n";
    cout << "  Nhap ma: "; string ma; getline(cin, ma);
    NhanVien* nv = ds.timTheoMa(ma);
    if (!nv) { cout << "  Khong tim thay ma: " << ma << "\n"; return; }
    nv->xuat();
    cout << "\n  Nhap moi (Enter = giu nguyen):\n";

    cout << "  Ho ten [" << nv->getHoTen() << "]: ";
    string ht; getline(cin, ht); if (!ht.empty()) nv->setHoTen(ht);

    cout << "  SDT [" << nv->getSdt() << "]: ";
    string sdt; getline(cin, sdt); if (!sdt.empty()) nv->setSdt(sdt);

    cout << "  Ngay sinh [" << nv->getNgaySinh() << "]: ";
    string ns; getline(cin, ns); if (!ns.empty()) nv->setNgaySinh(ns);

    cout << "  Luong co ban [" << (long long)nv->getLuongCoBan() << "]: ";
    string lcb; getline(cin, lcb); if (!lcb.empty()) nv->setLuongCoBan(stod(lcb));

    if (nv->getLoai() == "HDV") {
        HDV* h = dynamic_cast<HDV*>(nv);
        cout << "  So tour da dan [" << h->getSoTourDaDan() << "]: ";
        string st; getline(cin, st); if (!st.empty()) h->setSoTourDaDan(stoi(st));
    } else if (nv->getLoai() == "SALES") {
        SALES* s = dynamic_cast<SALES*>(nv);
        cout << "  Hoa hong% [" << s->getPhanTramHoaHong() << "]: ";
        string ph; getline(cin, ph); if (!ph.empty()) s->setPhanTramHoaHong(stod(ph));
    }
    cout << "  Cap nhat thanh cong!\n"; _luuNV(ds);
}

void chayMenuNhanVien(DanhSachNhanVien& ds) {
    _docNV(ds);
    int lc;
    do {
        cout << "\n========================================\n";
        cout <<   "        QUAN LY NHAN VIEN              \n";
        cout <<   "========================================\n";
        cout <<   "  1. Them nhan vien moi\n";
        cout <<   "  2. Hien thi danh sach\n";
        cout <<   "  3. Tim kiem nhan vien\n";
        cout <<   "  4. Xoa nhan vien\n";
        cout <<   "  5. Cap nhat thong tin\n";
        cout <<   "  6. Hien thi theo loai\n";
        cout <<   "  7. Bang luong\n";
        cout <<   "  8. Luu file\n";
        cout <<   "  9. Doc file\n";
        cout <<   "  0. Quay lai\n";
        cout <<   "  Lua chon: ";
        lc = _docLuaChon();
        switch (lc) {
            case 1: _themNhanVien(ds);                              break;
            case 2: cout << "\n"; ds.hienThi();                     break;
            case 3: _timKiemNhanVien(ds);                           break;
            case 4: _xoaNhanVien(ds);                               break;
            case 5: _capNhatNhanVien(ds);                           break;
            case 6: {
                cout << "  1.HDV 2.SALES 3.TN 4.QT | Chon: ";
                int x = _docLuaChon();
                string loai[] = {"HDV","SALES","TN","QT"};
                if (x >= 1 && x <= 4) ds.hienThiTheoLoai(loai[x-1]);
                break;
            }
            case 7: cout << "\n"; ds.hienThiBangLuong();            break;
            case 8: _luuNV(ds);                                     break;
            case 9: _docNV(ds);                                     break;
            case 0: cout << "  Quay lai.\n";                        break;
            default: cout << "  Lua chon khong hop le!\n";
        }
    } while (lc != 0);
}

#endif // NHANVIEN1_H
