#ifndef KHACHHANG2_H
#define KHACHHANG2_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <map>
using namespace std;

// ============================================================
// PHAN 0: DUONG DAN FILE DU LIEU
// Neu ten file trong thu muc data khac, sua o day
// ============================================================
const string DATA_DIR        = "data/";
const string FILE_KHACHHANG  = DATA_DIR + "khachhang.txt";
const string FILE_NHANVIEN   = DATA_DIR + "nhanvien.txt";
const string FILE_TOUR       = DATA_DIR + "tour.txt";
const string FILE_HOADON     = DATA_DIR + "hoadon.txt";

// ============================================================
// PHAN 1: HAM TIEN ICH DUNG CHUNG
// ============================================================

// Kiem tra ngay dd/mm/yyyy: dung dinh dang, dung so ngay cua thang, co tinh nam nhuan
inline bool ngayHopLe(const string& s) {
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit((unsigned char)s[i])) return false;
    }
    int ngay  = stoi(s.substr(0, 2));
    int thang = stoi(s.substr(3, 2));
    int nam   = stoi(s.substr(6, 4));
    if (nam < 1 || thang < 1 || thang > 12) return false;
    int soNgay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool nhuan = (nam % 400 == 0) || (nam % 4 == 0 && nam % 100 != 0);
    if (nhuan) soNgay[1] = 29;
    return ngay >= 1 && ngay <= soNgay[thang - 1];
}

// Chuyen chuoi thanh so thuc/nguyen an toan, khong nem exception
inline bool chuoiRaSoThuc(const string& s, double& out) {
    try { size_t pos = 0; out = stod(s, &pos); return pos == s.size(); }
    catch (...) { return false; }
}
inline bool chuoiRaSoNguyen(const string& s, int& out) {
    try { size_t pos = 0; out = stoi(s, &pos); return pos == s.size(); }
    catch (...) { return false; }
}

// Chuyen so thuc thanh chuoi gon de ghi file
inline string soThanhChuoi(double x) {
    ostringstream o;
    o << setprecision(15) << x;
    return o.str();
}

// Doc so thuc tu getline, lap lai den khi hop le. Het du lieu thi tra ve minVal.
inline double docSoThucDong(const string& nhan, double minVal) {
    while (true) {
        cout << nhan;
        string s;
        if (!getline(cin, s)) return minVal;
        double v;
        if (chuoiRaSoThuc(s, v) && v >= minVal) return v;
        cout << "  [LOI] Gia tri khong hop le (phai la so >= " << minVal << ").\n";
    }
}

// Doc so nguyen tu getline, lap lai den khi hop le. Het du lieu thi tra ve minVal.
inline int docSoNguyenDong(const string& nhan, int minVal) {
    while (true) {
        cout << nhan;
        string s;
        if (!getline(cin, s)) return minVal;
        int v;
        if (chuoiRaSoNguyen(s, v) && v >= minVal) return v;
        cout << "  [LOI] Gia tri khong hop le (phai la so nguyen >= " << minVal << ").\n";
    }
}

// Doc so nguyen tu luong nhap (cin), lap lai khi sai. Het du lieu thi tra ve minVal.
int docSoNguyen(istream& in, const string& nhan, int minVal) {
    int x;
    while (true) {
        cout << nhan;
        if (in >> x && x >= minVal) return x;
        if (in.eof()) return minVal;
        cout << "Gia tri khong hop le (phai la so >= " << minVal << ")!\n";
        in.clear();
        in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ============================================================
// PHAN 2: KHACH HANG
// ============================================================
class KhachHang {
private:
    string ma;
    string hoTen;
    string sdt;
    string ngaySinh;
    string diaChi;
    string loai;

public:
    KhachHang() {}

    string getMa() const { return ma; }
    string getHoTen() const { return hoTen; }
    string getSDT() const { return sdt; }
    string getNgaySinh() const { return ngaySinh; }
    string getDiaChi() const { return diaChi; }
    string getLoai() const { return loai; }

    bool laVIP() const { return loai == "VIP"; }

    bool docTuDong(string dong) {
        while (!dong.empty() && (dong[dong.size() - 1] == '\r' || dong[dong.size() - 1] == '\n'))
            dong.erase(dong.size() - 1);

        stringstream ss(dong);
        string m, t, s, n, d, l;
        if (!(getline(ss, m, '|') && getline(ss, t, '|') && getline(ss, s, '|') &&
              getline(ss, n, '|') && getline(ss, d, '|') && getline(ss, l)))
            return false;

        if (m.size() < 3 || m.substr(0, 2) != "KH") return false;
        if (!ngayHopLe(n)) return false;
        if (l != "Thuong" && l != "VIP") return false;

        ma = m; hoTen = t; sdt = s; ngaySinh = n; diaChi = d; loai = l;
        return true;
    }

    string ghiRaDong() const {
        return ma + "|" + hoTen + "|" + sdt + "|" + ngaySinh + "|" + diaChi + "|" + loai;
    }

    void hienThi() const {
        cout << ghiRaDong() << endl;
    }
};

inline string chuThuong(string s) {
    for (size_t i = 0; i < s.size(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

inline int timKhachTheoMa(const vector<KhachHang>& ds, const string& ma) {
    for (size_t i = 0; i < ds.size(); i++)
        if (ds[i].getMa() == ma) return (int)i;
    return -1;
}

inline vector<KhachHang> timKhachTheoTen(const vector<KhachHang>& ds, const string& tuKhoa) {
    vector<KhachHang> kq;
    string k = chuThuong(tuKhoa);
    for (size_t i = 0; i < ds.size(); i++)
        if (chuThuong(ds[i].getHoTen()).find(k) != string::npos)
            kq.push_back(ds[i]);
    return kq;
}

inline int timKhachTheoSDT(const vector<KhachHang>& ds, const string& sdt) {
    for (size_t i = 0; i < ds.size(); i++)
        if (ds[i].getSDT() == sdt) return (int)i;
    return -1;
}

inline vector<KhachHang> timKhachTheoLoai(const vector<KhachHang>& ds, const string& loai) {
    vector<KhachHang> kq;
    for (size_t i = 0; i < ds.size(); i++)
        if (ds[i].getLoai() == loai) kq.push_back(ds[i]);
    return kq;
}

// Ghi toan bo danh sach khach hang ra file
inline bool luuKhachHang(const vector<KhachHang>& ds, const string& tenFile) {
    ofstream f(tenFile);
    if (!f) return false;
    for (const KhachHang& kh : ds) f << kh.ghiRaDong() << "\n";
    return static_cast<bool>(f);
}

// Doc danh sach khach hang tu file. Tra ve so ban ghi doc duoc, hoac -1 neu file khong ton tai
inline int docKhachHang(vector<KhachHang>& ds, const string& tenFile) {
    ifstream f(tenFile);
    if (!f) return -1;
    ds.clear();
    string dong;
    while (getline(f, dong)) {
        if (dong.empty()) continue;
        KhachHang kh;
        if (kh.docTuDong(dong) && timKhachTheoMa(ds, kh.getMa()) == -1)
            ds.push_back(kh);
    }
    return (int)ds.size();
}

// ============================================================
// PHAN 3: HAM HO TRO CHO NHAN VIEN
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

// ============================================================
// PHAN 4: NHAN VIEN (NhanVien, HDV, SALES, TN, QT)
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
        string tmp;
        cout << "  Ma (NVxxx)    : ";
        while (true) {
            if (!getline(cin, tmp)) { ma = tmp; return; }
            if (tmp.size() >= 3 && tmp.substr(0, 2) == "NV") break;
            cout << "  [LOI] Ma phai bat dau bang NV (vd: NV001). Nhap lai: ";
        }
        ma = tmp;

        cout << "  Ho ten        : "; getline(cin, hoTen);
        cout << "  So dien thoai : "; getline(cin, sdt);

        while (true) {
            cout << "  Ngay sinh (dd/mm/yyyy): ";
            if (!getline(cin, ngaySinh)) return;
            if (ngayHopLe(ngaySinh)) break;
            cout << "  [LOI] Ngay khong hop le (vd: 05/11/1990). Nhap lai.\n";
        }

        luongCoBan = docSoThucDong("  Luong co ban  : ", 0);
    }

    virtual void xuat() const {
        cout << "  Ma       : " << ma << "\n";
        cout << "  Ho ten   : " << hoTen << "\n";
        cout << "  SDT      : " << sdt << "\n";
        cout << "  Ngay sinh: " << ngaySinh << "\n";
        cout << "  Luong CB : " << (long long)luongCoBan << "\n";
    }
};


// ------------------------------------------------------------
// HDV - Huong Dan Vien
// Luong = luong co ban + 200000 x so tour da dan
// File: HDV|ma|ho ten|SDT|ngay sinh|luong co ban|so tour da dan
// ------------------------------------------------------------
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
        soTourDaDan = docSoNguyenDong("  So tour da dan: ", 0);
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
        if (t[1].size() < 3 || t[1].substr(0, 2) != "NV") return false;
        if (!ngayHopLe(t[4])) return false;
        double lcb; int st;
        if (!chuoiRaSoThuc(t[5], lcb) || lcb < 0) return false;
        if (!chuoiRaSoNguyen(t[6], st) || st < 0) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = lcb; soTourDaDan = st;
        return true;
    }
};


// ------------------------------------------------------------
// SALES - Nhan Vien Kinh Doanh
// Luong = luong co ban + hoaHong% x tong doanh so hoa don da lap
// File: SALES|ma|ho ten|SDT|ngay sinh|luong co ban|hoa hong%
// (tongDoanhSo khong luu file, cap nhat runtime tu module HoaDon)
// ------------------------------------------------------------
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
    void   themDoanhSo(double soTien)      { tongDoanhSo += soTien; }

    double tinhLuong() const override {
        return luongCoBan + (phanTramHoaHong / 100.0) * tongDoanhSo;
    }
    string getLoai() const override { return "SALES"; }

    void nhap() override {
        cout << "=== Nhap SALES ===\n";
        NhanVien::nhap();
        double ph;
        do {
            ph = docSoThucDong("  Phan tram hoa hong (%) [0-100]: ", 0);
            if (ph > 100) cout << "  [LOI] Hoa hong toi da 100%.\n";
        } while (ph > 100);
        phanTramHoaHong = ph;
        tongDoanhSo = 0;
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
        if (t[1].size() < 3 || t[1].substr(0, 2) != "NV") return false;
        if (!ngayHopLe(t[4])) return false;
        double lcb, ph;
        if (!chuoiRaSoThuc(t[5], lcb) || lcb < 0) return false;
        if (!chuoiRaSoThuc(t[6], ph) || ph < 0 || ph > 100) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = lcb; phanTramHoaHong = ph; tongDoanhSo = 0;
        return true;
    }
};


// ------------------------------------------------------------
// TN - Truong Nhom
// Luong = luong co ban
// File: TN|ma|ho ten|SDT|ngay sinh|luong co ban
// ------------------------------------------------------------
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
        if (t[1].size() < 3 || t[1].substr(0, 2) != "NV") return false;
        if (!ngayHopLe(t[4])) return false;
        double lcb;
        if (!chuoiRaSoThuc(t[5], lcb) || lcb < 0) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = lcb;
        return true;
    }
};


// ------------------------------------------------------------
// QT - Quan Tri
// Luong = luong co ban
// File: QT|ma|ho ten|SDT|ngay sinh|luong co ban
// ------------------------------------------------------------
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
        if (t[1].size() < 3 || t[1].substr(0, 2) != "NV") return false;
        if (!ngayHopLe(t[4])) return false;
        double lcb;
        if (!chuoiRaSoThuc(t[5], lcb) || lcb < 0) return false;
        ma = t[1]; hoTen = t[2]; sdt = t[3]; ngaySinh = t[4];
        luongCoBan = lcb;
        return true;
    }
};

// Tao dung lop con theo tien to loai o dau dong file
static NhanVien* taoNhanVienTuLoai(const string& dong) {
    string loai = dong.substr(0, dong.find('|'));
    if (loai == "HDV")   return new HDV();
    if (loai == "SALES") return new SALES();
    if (loai == "TN")    return new TN();
    if (loai == "QT")    return new QT();
    return nullptr;
}


// ------------------------------------------------------------
// DanhSachNhanVien - Quan ly toan bo nhan vien
// ------------------------------------------------------------
class DanhSachNhanVien {
private:
    vector<NhanVien*> ds;

    bool maTonTai(const string& ma) const {
        for (const NhanVien* nv : ds)
            if (nv->getMa() == ma) return true;
        return false;
    }

public:
    DanhSachNhanVien() {}
    ~DanhSachNhanVien() {
        for (NhanVien* nv : ds) delete nv;
        ds.clear();
    }

    // Chan sao chep de tranh giai phong bo nho hai lan
    DanhSachNhanVien(const DanhSachNhanVien&) = delete;
    DanhSachNhanVien& operator=(const DanhSachNhanVien&) = delete;

    bool them(NhanVien* nv) {
        if (!nv) return false;
        if (maTonTai(nv->getMa())) {
            cout << "  [LOI] Ma '" << nv->getMa() << "' da ton tai!\n";
            return false;
        }
        ds.push_back(nv);
        return true;
    }

    bool xoa(const string& ma) {
        for (auto it = ds.begin(); it != ds.end(); ++it) {
            if ((*it)->getMa() == ma) {
                delete *it;
                ds.erase(it);
                return true;
            }
        }
        return false;
    }

    NhanVien* timTheoMa(const string& ma) const {
        for (NhanVien* nv : ds)
            if (nv->getMa() == ma) return nv;
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

    // Goi tu module HoaDon sau khi tao hoa don (hien chua duoc noi vao menu)
    void capNhatDoanhSoSales(const string& maNV, double soTien) {
        NhanVien* nv = timTheoMa(maNV);
        if (!nv) return;
        SALES* s = dynamic_cast<SALES*>(nv);
        if (s) s->themDoanhSo(soTien);
    }

    // Ghi danh sach nhan vien ra file
    bool luuFile(const string& tenFile) const {
        ofstream f(tenFile);
        if (!f) return false;
        for (const NhanVien* nv : ds) f << nv->ghiRaDong() << "\n";
        return static_cast<bool>(f);
    }

    // Doc danh sach nhan vien tu file. Tra ve so ban ghi, hoac -1 neu file khong ton tai
    int docFile(const string& tenFile) {
        ifstream f(tenFile);
        if (!f) return -1;
        for (NhanVien* nv : ds) delete nv;
        ds.clear();
        string dong;
        while (getline(f, dong)) {
            if (dong.empty()) continue;
            NhanVien* nv = taoNhanVienTuLoai(dong);
            if (!nv) continue;
            if (!nv->docTuDong(dong) || maTonTai(nv->getMa())) {
                delete nv;
                continue;
            }
            ds.push_back(nv);
        }
        return (int)ds.size();
    }
};


// ------------------------------------------------------------
// Menu quan ly nhan vien
// ------------------------------------------------------------
static int _docLuaChon() {
    int n = 0;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return n;
}

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
    if (ds.them(nv)) cout << "  Them thanh cong!\n";
    else delete nv;
}

static void _timKiemNhanVien(const DanhSachNhanVien& ds) {
    cout << "\n--- TIM KIEM ---\n";
    cout << "  1. Theo ma   2. Theo ten\n  Lua chon: ";
    int lc = _docLuaChon();
    if (lc == 1) {
        cout << "  Nhap ma: ";
        string ma; getline(cin, ma);
        NhanVien* nv = ds.timTheoMa(ma);
        if (nv) { cout << "\n"; nv->xuat(); }
        else cout << "  Khong tim thay ma: " << ma << "\n";
    } else if (lc == 2) {
        cout << "  Nhap ten: ";
        string ten; getline(cin, ten);
        vector<NhanVien*> kq = ds.timTheoTen(ten);
        if (kq.empty()) cout << "  Khong tim thay.\n";
        else for (NhanVien* nv : kq) { nv->xuat(); cout << "\n"; }
    }
}

static void _xoaNhanVien(DanhSachNhanVien& ds) {
    cout << "\n--- XOA NHAN VIEN ---\n";
    cout << "  Nhap ma: ";
    string ma; getline(cin, ma);
    NhanVien* nv = ds.timTheoMa(ma);
    if (!nv) { cout << "  Khong tim thay ma: " << ma << "\n"; return; }
    nv->xuat();
    cout << "  Xac nhan xoa? (y/n): ";
    string xn; getline(cin, xn);
    if (xn == "y" || xn == "Y") { ds.xoa(ma); cout << "  Da xoa!\n"; }
    else cout << "  Huy xoa.\n";
}

static void _capNhatNhanVien(DanhSachNhanVien& ds) {
    cout << "\n--- CAP NHAT ---\n";
    cout << "  Nhap ma: ";
    string ma; getline(cin, ma);
    NhanVien* nv = ds.timTheoMa(ma);
    if (!nv) { cout << "  Khong tim thay ma: " << ma << "\n"; return; }
    nv->xuat();
    cout << "\n  Nhap moi (Enter = giu nguyen):\n";

    cout << "  Ho ten [" << nv->getHoTen() << "]: ";
    string ht; getline(cin, ht);
    if (!ht.empty()) nv->setHoTen(ht);

    cout << "  SDT [" << nv->getSdt() << "]: ";
    string sdt; getline(cin, sdt);
    if (!sdt.empty()) nv->setSdt(sdt);

    cout << "  Ngay sinh [" << nv->getNgaySinh() << "]: ";
    string ns; getline(cin, ns);
    if (!ns.empty()) {
        if (ngayHopLe(ns)) nv->setNgaySinh(ns);
        else cout << "  [LOI] Ngay khong hop le, giu nguyen gia tri cu.\n";
    }

    cout << "  Luong co ban [" << (long long)nv->getLuongCoBan() << "]: ";
    string lcb; getline(cin, lcb);
    if (!lcb.empty()) {
        double vLuong;
        if (chuoiRaSoThuc(lcb, vLuong) && vLuong >= 0) nv->setLuongCoBan(vLuong);
        else cout << "  [LOI] Luong khong hop le, giu nguyen.\n";
    }

    if (nv->getLoai() == "HDV") {
        HDV* h = dynamic_cast<HDV*>(nv);
        cout << "  So tour da dan [" << h->getSoTourDaDan() << "]: ";
        string st; getline(cin, st);
        if (!st.empty()) {
            int vSt;
            if (chuoiRaSoNguyen(st, vSt) && vSt >= 0) h->setSoTourDaDan(vSt);
            else cout << "  [LOI] So tour khong hop le, giu nguyen.\n";
        }
    } else if (nv->getLoai() == "SALES") {
        SALES* s = dynamic_cast<SALES*>(nv);
        cout << "  Hoa hong% [" << s->getPhanTramHoaHong() << "]: ";
        string ph; getline(cin, ph);
        if (!ph.empty()) {
            double vPh;
            if (chuoiRaSoThuc(ph, vPh) && vPh >= 0 && vPh <= 100) s->setPhanTramHoaHong(vPh);
            else cout << "  [LOI] Hoa hong khong hop le (0-100), giu nguyen.\n";
        }
    }
    cout << "  Cap nhat thanh cong!\n";
}

void chayMenuNhanVien(DanhSachNhanVien& ds) {
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
        cout <<   "  0. Quay lai\n";
        cout <<   "  Lua chon: ";
        lc = _docLuaChon();
        switch (lc) {
            case 1: _themNhanVien(ds);    break;
            case 2: cout << "\n"; ds.hienThi(); break;
            case 3: _timKiemNhanVien(ds); break;
            case 4: _xoaNhanVien(ds);     break;
            case 5: _capNhatNhanVien(ds); break;
            case 6: {
                cout << "  1.HDV 2.SALES 3.TN 4.QT | Chon: ";
                int x = _docLuaChon();
                string loai[] = {"HDV", "SALES", "TN", "QT"};
                if (x >= 1 && x <= 4) ds.hienThiTheoLoai(loai[x - 1]);
                break;
            }
            case 7: cout << "\n"; ds.hienThiBangLuong(); break;
            case 0: cout << "  Quay lai.\n"; break;
            default: cout << "  Lua chon khong hop le!\n";
        }
    } while (lc != 0);
}

// ============================================================
// PHAN 5: TOUR
// File: loai|ma|ten|diem den|ngay khoi hanh|so ngay|gia|so cho
// ============================================================
class Tour {
protected:
    string maTour, tenTour, diemDen, ngayKhoiHanh;
    int soNgay, giaTour, soCho;

public:
    Tour() : soNgay(0), giaTour(0), soCho(0) {}

    Tour(string ma, string ten, string diem,
         string ngay, int sn, int gia, int cho)
        : maTour(ma), tenTour(ten), diemDen(diem), ngayKhoiHanh(ngay),
          soNgay(sn), giaTour(gia), soCho(cho) {}

    virtual ~Tour() {}

    string getMaTour() const { return maTour; }
    void setMaTour(const string& ma) { maTour = ma; }

    virtual string loaiTour() const { return "Thuong"; }

    // Ham nhap thong tin (tru ma) dung chung cho ca them va sua
    void nhapThongTinMoi(istream& in) {
        cout << "Ten tour: ";
        getline(in >> ws, tenTour);

        cout << "Diem den: ";
        getline(in, diemDen);

        cout << "Ngay khoi hanh: ";
        getline(in, ngayKhoiHanh);

        soNgay  = docSoNguyen(in, "So ngay: ", 1);
        giaTour = docSoNguyen(in, "Gia tour: ", 0);
        soCho   = docSoNguyen(in, "So cho: ", 0);
    }

    friend istream& operator>>(istream& in, Tour& t) {
        cout << "Ma tour: ";
        getline(in >> ws, t.maTour);
        t.nhapThongTinMoi(in);
        return in;
    }

    friend ostream& operator<<(ostream& out, const Tour& t) {
        out << "\nMa tour: " << t.maTour;
        out << "\nTen tour: " << t.tenTour;
        out << "\nDiem den: " << t.diemDen;
        out << "\nNgay khoi hanh: " << t.ngayKhoiHanh;
        out << "\nSo ngay: " << t.soNgay;
        out << "\nGia tour: " << t.giaTour;
        out << "\nSo cho: " << t.soCho;
        out << "\nLoai tour: " << t.loaiTour();
        return out;
    }

    bool operator==(const Tour& t) const { return maTour == t.maTour; }

    virtual void hienThi() const { cout << *this << endl; }

    // Chuyen tour thanh mot dong de ghi file
    string ghiRaDong() const {
        return loaiTour() + "|" + maTour + "|" + tenTour + "|" + diemDen + "|"
             + ngayKhoiHanh + "|" + to_string(soNgay) + "|"
             + to_string(giaTour) + "|" + to_string(soCho);
    }

    // Doc mot dong tu file. Tra ve false neu dong sai dinh dang
    bool docTuDong(string dong) {
        while (!dong.empty() && (dong.back() == '\r' || dong.back() == '\n')) dong.pop_back();
        stringstream ss(dong);
        string loai, ma, ten, diem, ngay, sn, gia, cho;
        if (!(getline(ss, loai, '|') && getline(ss, ma, '|') && getline(ss, ten, '|') &&
              getline(ss, diem, '|') && getline(ss, ngay, '|') && getline(ss, sn, '|') &&
              getline(ss, gia, '|') && getline(ss, cho)))
            return false;
        int a, b, c;
        if (!chuoiRaSoNguyen(sn, a) || !chuoiRaSoNguyen(gia, b) || !chuoiRaSoNguyen(cho, c)) return false;
        if (ma.empty() || a < 1 || b < 0 || c < 0) return false;
        maTour = ma; tenTour = ten; diemDen = diem; ngayKhoiHanh = ngay;
        soNgay = a; giaTour = b; soCho = c;
        return true;
    }
};

class TourNoiDia : public Tour {
public:
    TourNoiDia() : Tour() {}
    string loaiTour() const override { return "Noi dia"; }
};

class TourQuocTe : public Tour {
public:
    TourQuocTe() : Tour() {}
    string loaiTour() const override { return "Quoc te"; }
};

// Tao dung lop tour theo loai o dau dong file
static Tour* taoTourTuLoai(const string& dong) {
    string loai = dong.substr(0, dong.find('|'));
    if (loai == "Noi dia") return new TourNoiDia();
    if (loai == "Quoc te") return new TourQuocTe();
    return nullptr;
}

class QuanLyTour {
private:
    vector<Tour*> dsTour;

    // Tim vi tri tour theo ma, khong thay tra ve -1
    int timViTri(const string& ma) const {
        for (size_t i = 0; i < dsTour.size(); i++)
            if (dsTour[i]->getMaTour() == ma) return (int)i;
        return -1;
    }

    // Luon doc ma bang getline de thong nhat
    string nhapMa(const string& nhan) const {
        string ma;
        cout << nhan;
        getline(cin >> ws, ma);
        return ma;
    }

public:
    QuanLyTour() {}

    // Cam sao chep de tranh delete 2 lan
    QuanLyTour(const QuanLyTour&) = delete;
    QuanLyTour& operator=(const QuanLyTour&) = delete;

    ~QuanLyTour() {
        for (Tour* t : dsTour) delete t;
    }

    void themTour() {
        cout << "\n1. Tour noi dia";
        cout << "\n2. Tour quoc te\n";
        int loai = docSoNguyen(cin, "Chon loai tour: ", 0);

        Tour* t = nullptr;
        if (loai == 1)      t = new TourNoiDia();
        else if (loai == 2) t = new TourQuocTe();
        else {
            cout << "Loai tour khong hop le!\n";
            return;
        }

        // Kiem tra trung ma TRUOC khi nhap cac truong con lai
        string ma = nhapMa("Ma tour: ");
        if (timViTri(ma) != -1) {
            cout << "Ma tour da ton tai!\n";
            delete t;
            return;
        }
        t->setMaTour(ma);
        t->nhapThongTinMoi(cin);

        dsTour.push_back(t);
        cout << "Them tour thanh cong!\n";
    }

    void hienThi() const {
        if (dsTour.empty()) {
            cout << "Danh sach tour trong!\n";
            return;
        }
        for (const Tour* t : dsTour) t->hienThi();
    }

    void timKiem() const {
        string ma = nhapMa("Nhap ma tour can tim: ");
        int vt = timViTri(ma);
        if (vt == -1) cout << "Khong tim thay tour!\n";
        else          cout << *dsTour[vt] << endl;
    }

    void suaTour() {
        string ma = nhapMa("Nhap ma tour can sua: ");
        int vt = timViTri(ma);
        if (vt == -1) {
            cout << "Khong tim thay tour!\n";
            return;
        }
        cout << "Nhap thong tin moi:\n";
        dsTour[vt]->nhapThongTinMoi(cin);
        cout << "Sua tour thanh cong!\n";
    }

    void xoaTour() {
        string ma = nhapMa("Nhap ma tour can xoa: ");
        int vt = timViTri(ma);
        if (vt == -1) {
            cout << "Khong tim thay tour!\n";
            return;
        }
        delete dsTour[vt];
        dsTour.erase(dsTour.begin() + vt);
        cout << "Xoa tour thanh cong!\n";
    }

    // Ghi danh sach tour ra file
    bool luuFile(const string& tenFile) const {
        ofstream f(tenFile);
        if (!f) return false;
        for (const Tour* t : dsTour) f << t->ghiRaDong() << "\n";
        return static_cast<bool>(f);
    }

    // Doc danh sach tour tu file. Tra ve so ban ghi, hoac -1 neu file khong ton tai
    int docFile(const string& tenFile) {
        ifstream f(tenFile);
        if (!f) return -1;
        for (Tour* t : dsTour) delete t;
        dsTour.clear();
        string dong;
        while (getline(f, dong)) {
            if (dong.empty()) continue;
            Tour* t = taoTourTuLoai(dong);
            if (!t) continue;
            if (!t->docTuDong(dong) || timViTri(t->getMaTour()) != -1) {
                delete t;
                continue;
            }
            dsTour.push_back(t);
        }
        return (int)dsTour.size();
    }
};

// ============================================================
// PHAN 6: HOA DON VA THONG KE
// File: MaHD|MaKH|MaTour|NgayLap|SLNL|SLTE|GiaNL|GiaTE|GiamGia|PTTT|TrangThai(0-3)
// ============================================================
enum TrangThaiThanhToan {
    CHUA_THANH_TOAN,
    DA_DAT_COC,
    DA_THANH_TOAN,
    DA_HUY
};

// Ham ho tro chuyen enum thanh chuoi de hien thi
string trangThaiToString(TrangThaiThanhToan trangThai) {
    switch (trangThai) {
        case CHUA_THANH_TOAN: return "Chua thanh toan";
        case DA_DAT_COC:      return "Da dat coc";
        case DA_THANH_TOAN:   return "Da thanh toan";
        case DA_HUY:          return "Da huy";
        default:              return "Khong xac dinh";
    }
}

// Ham nhap so nguyen/so thuc an toan
template <class T>
T nhapSo(const string& loiNhac) {
    T v;
    while (true) {
        cout << loiNhac;
        if (cin >> v) return v;
        if (cin.eof()) return T();
        cout << "  Gia tri khong hop le, vui long nhap lai!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Kiem tra ngay theo dung dinh dang DD/MM/YYYY va tach ra ngay, thang, nam
bool tachNgayHopLe(const string& s, int& ngay, int& thang, int& nam) {
    if (!ngayHopLe(s)) return false;
    ngay  = stoi(s.substr(0, 2));
    thang = stoi(s.substr(3, 2));
    nam   = stoi(s.substr(6, 4));
    return true;
}

class HoaDon {
private:
    static int autoId;
    string MaHD, MaKH, MaNV, MaT, NgayLap;
    int SoLuongNguoiLon, SoLuongTreEm;
    double GiaNguoiLon, GiaTreEm, GiamGia, TongTien;
    string PTThanhToan;
    TrangThaiThanhToan TrangThai;

public:
    HoaDon()
        : SoLuongNguoiLon(0), SoLuongTreEm(0),
          GiaNguoiLon(0), GiaTreEm(0), GiamGia(0), TongTien(0),
          TrangThai(CHUA_THANH_TOAN) {}

    HoaDon(string maKH, string maT, string ngayLap, int slNL, int slTE,
           double giaNL, double giaTE, string ptThanhToan)
        : MaKH(maKH), MaT(maT), NgayLap(ngayLap),
          SoLuongNguoiLon(slNL), SoLuongTreEm(slTE),
          GiaNguoiLon(giaNL), GiaTreEm(giaTE), GiamGia(0), TongTien(0),
          PTThanhToan(ptThanhToan), TrangThai(CHUA_THANH_TOAN) {
        autoId++;
        MaHD = "HD" + to_string(autoId);
        TinhTongTien();
    }

    double TinhTongTien() {
        return TongTien = SoLuongNguoiLon * GiaNguoiLon + SoLuongTreEm * GiaTreEm - GiamGia;
    }

    void setTrangThai(TrangThaiThanhToan tt) { TrangThai = tt; }

    void hienThiDongDong() const {
        cout << left << setw(10) << MaHD
             << setw(12) << MaKH
             << setw(10) << MaT
             << setw(12) << NgayLap
             << setw(8)  << SoLuongNguoiLon
             << setw(8)  << SoLuongTreEm
             << setw(16) << fixed << setprecision(0) << TongTien
             << setw(18) << PTThanhToan
             << setw(16) << trangThaiToString(TrangThai) << endl;
    }

    void inChiTiet() const {
        cout << "\n==============================================" << endl;
        cout << "               HOA DON DU LICH                " << endl;
        cout << "==============================================" << endl;
        cout << "Ma hoa don     : " << MaHD << endl;
        cout << "Ma khach hang  : " << MaKH << endl;
        cout << "Ma tour        : " << MaT << endl;
        cout << "Ngay lap       : " << NgayLap << endl;
        cout << "----------------------------------------------" << endl;
        cout << fixed << setprecision(0);
        cout << "So nguoi lon   : " << SoLuongNguoiLon << " x " << GiaNguoiLon << " VND" << endl;
        cout << "So tre em      : " << SoLuongTreEm << " x " << GiaTreEm << " VND" << endl;
        cout << "Giam gia       : " << GiamGia << " VND" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "TONG TIEN      : " << TongTien << " VND" << endl;
        cout << "PT thanh toan  : " << PTThanhToan << endl;
        cout << "Trang thai     : " << trangThaiToString(TrangThai) << endl;
        cout << "==============================================\n" << endl;
    }

    // Getters phuc vu thong ke
    string getMaHD() const { return MaHD; }
    string getMaKH() const { return MaKH; }
    string getMaT() const { return MaT; }
    string getNgayLap() const { return NgayLap; }
    int getSoLuongNguoiLon() const { return SoLuongNguoiLon; }
    int getSoLuongTreEm() const { return SoLuongTreEm; }
    double getTongTien() const { return TongTien; }
    TrangThaiThanhToan getTrangThai() const { return TrangThai; }

    // Chuyen hoa don thanh mot dong de ghi file
    string ghiRaDong() const {
        return MaHD + "|" + MaKH + "|" + MaT + "|" + NgayLap + "|"
             + to_string(SoLuongNguoiLon) + "|" + to_string(SoLuongTreEm) + "|"
             + soThanhChuoi(GiaNguoiLon) + "|" + soThanhChuoi(GiaTreEm) + "|"
             + soThanhChuoi(GiamGia) + "|" + PTThanhToan + "|"
             + to_string((int)TrangThai);
    }

    // Doc mot dong tu file. Tong tien duoc tinh lai, khong doc tu file.
    bool docTuDong(string dong) {
        while (!dong.empty() && (dong.back() == '\r' || dong.back() == '\n')) dong.pop_back();
        stringstream ss(dong);
        string hd, kh, t, ngay, snl, ste, gnl, gte, gg, pt, tt;
        if (!(getline(ss, hd, '|') && getline(ss, kh, '|') && getline(ss, t, '|') &&
              getline(ss, ngay, '|') && getline(ss, snl, '|') && getline(ss, ste, '|') &&
              getline(ss, gnl, '|') && getline(ss, gte, '|') && getline(ss, gg, '|') &&
              getline(ss, pt, '|') && getline(ss, tt)))
            return false;
        if (hd.size() < 3 || hd.substr(0, 2) != "HD") return false;
        int id, a, b, c;
        double x, y, z;
        if (!chuoiRaSoNguyen(hd.substr(2), id)) return false;
        if (!ngayHopLe(ngay)) return false;
        if (!chuoiRaSoNguyen(snl, a) || !chuoiRaSoNguyen(ste, b) || !chuoiRaSoNguyen(tt, c)) return false;
        if (!chuoiRaSoThuc(gnl, x) || !chuoiRaSoThuc(gte, y) || !chuoiRaSoThuc(gg, z)) return false;
        if (a < 0 || b < 0 || c < 0 || c > 3) return false;

        MaHD = hd; MaKH = kh; MaT = t; NgayLap = ngay;
        SoLuongNguoiLon = a; SoLuongTreEm = b;
        GiaNguoiLon = x; GiaTreEm = y; GiamGia = z;
        PTThanhToan = pt;
        TrangThai = static_cast<TrangThaiThanhToan>(c);
        TinhTongTien();
        // Cap nhat bo dem de ma hoa don moi khong trung ma cu
        if (id >= autoId) autoId = id + 1;
        return true;
    }
};
int HoaDon::autoId = 1000;

class Quanlyhoadon {
private:
    vector<HoaDon> dsHoaDon;

    HoaDon* timHoaDon(const string& maHD) {
        for (size_t i = 0; i < dsHoaDon.size(); i++)
            if (dsHoaDon[i].getMaHD() == maHD) return &dsHoaDon[i];
        return nullptr;
    }

public:
    void LapHoaDon() {
        string maKH, maT, ngayLap, phuongThucTT;
        int slNL, slTE, chonPTTT;
        double giaNL, giaTE;

        cout << "\n--- TAO HOA DON DAT TOUR MOI ---" << endl;
        cout << "Nhap ma khach hang: "; cin >> maKH;
        cout << "Nhap ma tour: ";      cin >> maT;
        int ngay, thang, nam;
        do {
            cout << "Nhap ngay lap (DD/MM/YYYY): ";
            cin >> ngayLap;
            if (cin.eof()) return;
            if (!tachNgayHopLe(ngayLap, ngay, thang, nam))
                cout << "Ngay khong hop le. Vi du dung: 05/11/2026. Vui long nhap lai!\n";
        } while (!tachNgayHopLe(ngayLap, ngay, thang, nam));

        slNL = nhapSo<int>("Nhap so nguoi lon: ");
        giaNL = nhapSo<double>("Nhap gia ve nguoi lon: ");
        slTE = nhapSo<int>("Nhap so tre em: ");
        giaTE = nhapSo<double>("Nhap gia ve tre em: ");

        if (slNL < 0 || slTE < 0 || giaNL < 0 || giaTE < 0 || slNL + slTE == 0) {
            cout << "So luong/gia khong hop le, huy thao tac!" << endl;
            return;
        }

        do {
            cout << "Chon phuong thuc thanh toan:\n" << "1. Tien mat\n" << "2. Chuyen khoan\n";
            chonPTTT = nhapSo<int>("Lua chon: ");
            if (cin.eof()) return;
            switch (chonPTTT) {
                case 1: phuongThucTT = "Tien mat"; break;
                case 2: phuongThucTT = "Chuyen khoan"; break;
                default: cout << "Lua chon khong hop le!\n";
            }
        } while (chonPTTT != 1 && chonPTTT != 2);

        HoaDon hd(maKH, maT, ngayLap, slNL, slTE, giaNL, giaTE, phuongThucTT);
        dsHoaDon.push_back(hd);
        cout << "Lap hoa don thanh cong! Ma hoa don: " << hd.getMaHD()
             << " | Tong tien: " << fixed << setprecision(0) << hd.getTongTien() << " VND" << endl;
    }

    void HienThiHoaDon() const {
        if (dsHoaDon.empty()) {
            cout << "Danh sach hoa don dang trong!" << endl;
            return;
        }
        cout << "\n" << string(120, '=') << endl;
        cout << left << setw(10) << "Ma HD"
             << setw(12) << "Ma KH"
             << setw(10) << "Ma Tour"
             << setw(12) << "Ngay Lap"
             << setw(8)  << "SL NL"
             << setw(8)  << "SL TE"
             << setw(16) << "Tong Tien (VND)"
             << setw(18) << "Hinh Thuc TT"
             << setw(16) << "Trang Thai" << endl;
        cout << string(120, '-') << endl;
        for (const auto& hd : dsHoaDon) hd.hienThiDongDong();
        cout << string(120, '=') << "\n" << endl;
    }

    void CapNhatHoaDon() {
        string maHD;
        cout << "Nhap ma hoa don can cap nhat: ";
        cin >> maHD;
        HoaDon* hd = timHoaDon(maHD);
        if (!hd) { cout << "Khong tim thay hoa don voi ma: " << maHD << endl; return; }

        cout << "0. Chua thanh toan\n" << "1. Da dat coc\n" << "2. Da thanh toan\n" << "3. Da huy\n";
        int chonTrangThai = nhapSo<int>("Lua chon trang thai: ");
        if (chonTrangThai >= 0 && chonTrangThai <= 3) {
            hd->setTrangThai(static_cast<TrangThaiThanhToan>(chonTrangThai));
            cout << "Cap nhat trang thai thanh cong!" << endl;
        } else {
            cout << "Lua chon khong hop le!" << endl;
        }
    }

    void InChiTietHoaDon() {
        string maHD;
        cout << "Nhap ma hoa don can in: ";
        cin >> maHD;
        HoaDon* hd = timHoaDon(maHD);
        if (hd) hd->inChiTiet();
        else cout << "Khong tim thay hoa don voi ma: " << maHD << endl;
    }

    // 1. Thong ke tong doanh thu (chi tinh hoa don DA_THANH_TOAN)
    void ThongKeTongDoanhThu() const {
        double tongDoanhThu = 0;
        int dem = 0;
        for (const auto& hd : dsHoaDon) {
            if (hd.getTrangThai() == DA_THANH_TOAN) {
                tongDoanhThu += hd.getTongTien();
                dem++;
            }
        }
        cout << "\n================ THONG KE DOANH THU ================" << endl;
        cout << "So luong hoa don da thanh toan hop le : " << dem << endl;
        cout << "Tong doanh thu thu duoc               : " << fixed << setprecision(0) << tongDoanhThu << " VND" << endl;
        cout << "=====================================================\n" << endl;
    }

    // 2. Thong ke theo thoi gian. Khoa sap xep theo YYYYMMDD de dung thu tu.
    void ThongKeTheoThoiGian() const {
        if (dsHoaDon.empty()) {
            cout << "Danh sach hoa don trong!" << endl;
            return;
        }

        cout << "\n--- CHON LOAI THONG KE THEO THOI GIAN ---\n";
        cout << "1. Theo ngay (DD/MM/YYYY)\n";
        cout << "2. Theo thang (MM/YYYY)\n";
        cout << "3. Theo quy (Qx/YYYY)\n";
        cout << "4. Theo nam (YYYY)\n";
        int chon = nhapSo<int>("Lua chon: ");
        if (chon < 1 || chon > 4) {
            cout << "Lua chon khong hop le!\n";
            return;
        }

        // key la khoa thoi gian co thu tu; first la nhan hien thi, second la tong doanh thu
        map<string, pair<string, double> > thongKe;
        for (const auto& hd : dsHoaDon) {
            if (hd.getTrangThai() != DA_THANH_TOAN) continue;

            int d, m, y;
            if (!tachNgayHopLe(hd.getNgayLap(), d, m, y)) continue;

            string key, nhan;
            if (chon == 1) {
                key = to_string(y) + (m < 10 ? "0" : "") + to_string(m)
                    + (d < 10 ? "0" : "") + to_string(d);
                nhan = hd.getNgayLap();
            } else if (chon == 2) {
                key = to_string(y) + (m < 10 ? "0" : "") + to_string(m);
                nhan = (m < 10 ? "0" : "") + to_string(m) + "/" + to_string(y);
            } else if (chon == 3) {
                int q = (m - 1) / 3 + 1;
                key = to_string(y) + "Q" + to_string(q);
                nhan = "Q" + to_string(q) + "/" + to_string(y);
            } else {
                key = to_string(y);
                nhan = to_string(y);
            }

            thongKe[key].first = nhan;
            thongKe[key].second += hd.getTongTien();
        }

        if (thongKe.empty()) {
            cout << "Khong co hoa don da thanh toan hop le trong du lieu!\n";
            return;
        }

        cout << "\n" << left << setw(18) << "Thoi gian"
             << setw(20) << "Doanh thu (VND)" << "Bieu do (1 vach ~ 2 trieu)\n";
        cout << string(75, '-') << endl;
        for (const auto& item : thongKe) {
            double doanhThu = item.second.second;
            int soVach = static_cast<int>(doanhThu / 2000000.0);
            if (soVach > 40) soVach = 40;
            cout << left << setw(18) << item.second.first
                 << setw(20) << fixed << setprecision(0) << doanhThu
                 << string(soVach, '=') << ">\n";
        }
        cout << string(75, '=') << endl;
    }

    // 3. Thong ke theo tour (doanh thu cao nhat / thap nhat)
    void ThongKeTheoTour() const {
        if (dsHoaDon.empty()) {
            cout << "Danh sach hoa don trong!" << endl;
            return;
        }

        map<string, double> doanhThuTour;
        map<string, int> soLuongTour;
        for (const auto& hd : dsHoaDon) {
            if (hd.getTrangThai() == DA_THANH_TOAN) {
                doanhThuTour[hd.getMaT()] += hd.getTongTien();
                soLuongTour[hd.getMaT()] += (hd.getSoLuongNguoiLon() + hd.getSoLuongTreEm());
            }
        }

        if (doanhThuTour.empty()) {
            cout << "Chua co du lieu hoa don da thanh toan de thong ke tour!" << endl;
            return;
        }

        cout << "\n================== THONG KE DOANH THU THEO TOUR ==================" << endl;
        cout << left << setw(12) << "Ma Tour" << setw(18) << "So Khach" << setw(20) << "Tong Doanh Thu (VND)" << endl;
        cout << "------------------------------------------------------------------" << endl;

        string tourMax = "", tourMin = "";
        double maxDT = -1, minDT = -1;

        for (const auto& pair : doanhThuTour) {
            string maT = pair.first;
            double dt = pair.second;
            cout << left << setw(12) << maT
                 << setw(18) << soLuongTour[maT]
                 << setw(20) << fixed << setprecision(0) << dt << endl;

            if (maxDT == -1 || dt > maxDT) {
                maxDT = dt;
                tourMax = maT;
            }
            if (minDT == -1 || dt < minDT) {
                minDT = dt;
                tourMin = maT;
            }
        }
        cout << "------------------------------------------------------------------" << endl;
        cout << "=> Tour doanh thu CAO NHAT  : " << tourMax << " (" << fixed << setprecision(0) << maxDT << " VND)" << endl;
        cout << "=> Tour doanh thu THAP NHAT : " << tourMin << " (" << fixed << setprecision(0) << minDT << " VND)" << endl;
        cout << "==================================================================\n" << endl;
    }

    // 4. Thong ke so luong khach hang dang ky tour
    void ThongKeSoLuongKhach() const {
        if (dsHoaDon.empty()) {
            cout << "Danh sach hoa don trong!" << endl;
            return;
        }

        int tongNL = 0, tongTE = 0;
        map<string, int> khachTheoTour;

        for (const auto& hd : dsHoaDon) {
            // Tinh khach cua cac hoa don hop le (da thanh toan hoac da dat coc)
            if (hd.getTrangThai() == DA_THANH_TOAN || hd.getTrangThai() == DA_DAT_COC) {
                tongNL += hd.getSoLuongNguoiLon();
                tongTE += hd.getSoLuongTreEm();
                khachTheoTour[hd.getMaT()] += (hd.getSoLuongNguoiLon() + hd.getSoLuongTreEm());
            }
        }

        cout << "\n=============== THONG KE SO LUONG KHACH HANG ===============" << endl;
        cout << "Tong so nguoi lon dang ky : " << tongNL << endl;
        cout << "Tong so tre em dang ky    : " << tongTE << endl;
        cout << "Tong cong so luong khach  : " << (tongNL + tongTE) << " khach" << endl;
        cout << "(Tinh hoa don da dat coc va da thanh toan; khong tinh hoa don huy/chua thanh toan)" << endl;
        cout << "-----------------------------------------------------------" << endl;
        cout << left << setw(15) << "Ma Tour" << "Tong So Khach" << endl;
        cout << "-----------------------------------------------------------" << endl;
        for (const auto& pair : khachTheoTour) {
            cout << left << setw(15) << pair.first << pair.second << " khach" << endl;
        }
        cout << "===========================================================\n" << endl;
    }

    // 5. Thong ke hoa don theo trang thai
    void ThongKeHoaDon() const {
        int demChuaTT = 0, demDatCoc = 0, demDaTT = 0, demDaHuy = 0;
        int tong = static_cast<int>(dsHoaDon.size());

        for (const auto& hd : dsHoaDon) {
            switch (hd.getTrangThai()) {
                case CHUA_THANH_TOAN: demChuaTT++; break;
                case DA_DAT_COC:      demDatCoc++; break;
                case DA_THANH_TOAN:   demDaTT++;   break;
                case DA_HUY:          demDaHuy++;  break;
            }
        }

        cout << "\n=================== THONG KE TRANG THAI HOA DON ===================" << endl;
        cout << left << setw(20) << "Trang Thai" << setw(15) << "So Luong" << "Ty Le (%)" << endl;
        cout << "-------------------------------------------------------------------" << endl;

        auto inDongBang = [](string ten, int sl, int tongSo) {
            double phanTram = (tongSo > 0) ? (static_cast<double>(sl) / tongSo) * 100.0 : 0.0;
            cout << left << setw(20) << ten << setw(15) << sl
                 << fixed << setprecision(1) << phanTram << "%" << endl;
        };

        inDongBang("Da thanh toan", demDaTT, tong);
        inDongBang("Da dat coc", demDatCoc, tong);
        inDongBang("Chua thanh toan", demChuaTT, tong);
        inDongBang("Da huy", demDaHuy, tong);
        cout << "-------------------------------------------------------------------" << endl;
        cout << "Tong so hoa don trong he thong : " << tong << endl;

        // Bieu do cot ASCII phan bo trang thai
        cout << "\n[Bieu do cot phan bo hoa don]" << endl;
        auto inBieuDoCot = [](string nhan, int sl) {
            cout << left << setw(16) << nhan << " | " << string(sl * 3, '*') << " (" << sl << ")" << endl;
        };
        inBieuDoCot("Da thanh toan", demDaTT);
        inBieuDoCot("Da dat coc", demDatCoc);
        inBieuDoCot("Chua thanh toan", demChuaTT);
        inBieuDoCot("Da huy", demDaHuy);
        cout << "===================================================================\n" << endl;
        cout << "Luu y: Day la bieu do cot ASCII, chua phai bieu do tron. Ty le phan tram da duoc the hien trong bang.\n";
    }

    // Ghi danh sach hoa don ra file
    bool luuFile(const string& tenFile) const {
        ofstream f(tenFile);
        if (!f) return false;
        for (const HoaDon& hd : dsHoaDon) f << hd.ghiRaDong() << "\n";
        return static_cast<bool>(f);
    }

    // Doc danh sach hoa don tu file. Tra ve so ban ghi, hoac -1 neu file khong ton tai
    int docFile(const string& tenFile) {
        ifstream f(tenFile);
        if (!f) return -1;
        dsHoaDon.clear();
        string dong;
        while (getline(f, dong)) {
            if (dong.empty()) continue;
            HoaDon hd;
            if (hd.docTuDong(dong) && !timHoaDon(hd.getMaHD()))
                dsHoaDon.push_back(hd);
        }
        return (int)dsHoaDon.size();
    }
};

// ============================================================
// PHAN 7: MENU KHACH HANG, TOUR, HOA DON
// ============================================================
void chayMenuKhachHang(vector<KhachHang>& ds) {
    int luaChon;
    do {
        cout << "\n========== QUAN LY KHACH HANG ==========\n"
             << "1. Nhap danh sach khach hang\n"
             << "2. Hien thi danh sach khach hang\n"
             << "3. Tim khach hang theo ma\n"
             << "4. Tim khach hang theo ten\n"
             << "5. Tim khach hang theo so dien thoai\n"
             << "6. Tim khach hang theo loai\n"
             << "0. Quay lai\n"
             << "========================================\n";
        luaChon = docSoNguyen(cin, "Nhap lua chon: ", 0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (luaChon) {
        case 1: {
            int n = docSoNguyen(cin, "Nhap so luong khach hang: ", 0);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int i = 0; i < n; i++) {
                cout << "\nNhap khach hang thu " << i + 1 << ":\n"
                     << "Ma|HoTen|SDT|NgaySinh|DiaChi|Loai\n";
                string dong;
                getline(cin, dong);
                KhachHang kh;
                if (!kh.docTuDong(dong)) {
                    cout << "Du lieu khong hop le!\n";
                    continue;
                }
                if (timKhachTheoMa(ds, kh.getMa()) != -1) {
                    cout << "Ma khach hang da ton tai!\n";
                    continue;
                }
                ds.push_back(kh);
                cout << "Them khach hang thanh cong!\n";
            }
            break;
        }
        case 2:
            cout << "\nDANH SACH KHACH HANG\n";
            if (ds.empty()) cout << "Danh sach rong!\n";
            for (const auto& kh : ds) kh.hienThi();
            break;
        case 3: {
            cout << "Nhap ma khach hang can tim: ";
            string ma; getline(cin, ma);
            int vt = timKhachTheoMa(ds, ma);
            if (vt != -1) { cout << "Tim thay khach hang:\n"; ds[vt].hienThi(); }
            else cout << "Khong tim thay khach hang!\n";
            break;
        }
        case 4: {
            cout << "Nhap ten khach hang can tim: ";
            string ten; getline(cin, ten);
            vector<KhachHang> kq = timKhachTheoTen(ds, ten);
            if (kq.empty()) cout << "Khong tim thay khach hang!\n";
            else for (const auto& kh : kq) kh.hienThi();
            break;
        }
        case 5: {
            cout << "Nhap so dien thoai can tim: ";
            string sdt; getline(cin, sdt);
            int vt = timKhachTheoSDT(ds, sdt);
            if (vt != -1) ds[vt].hienThi();
            else cout << "Khong tim thay khach hang!\n";
            break;
        }
        case 6: {
            cout << "Nhap loai khach hang (VIP/Thuong): ";
            string loai; getline(cin, loai);
            string k = chuThuong(loai);
            if (k == "vip") loai = "VIP";
            else if (k == "thuong") loai = "Thuong";
            vector<KhachHang> kq = timKhachTheoLoai(ds, loai);
            if (kq.empty()) cout << "Khong tim thay khach hang!\n";
            else for (const auto& kh : kq) kh.hienThi();
            break;
        }
        case 0:
            cout << "Quay lai menu chinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
}

void chayMenuTour(QuanLyTour& ql) {
    int chon;
    do {
        cout << "\n===== QUAN LY TOUR DU LICH =====\n"
             << "1. Them tour\n2. Hien thi danh sach tour\n3. Tim kiem tour\n"
             << "4. Sua thong tin tour\n5. Xoa tour\n0. Quay lai\n";
        chon = docSoNguyen(cin, "Nhap lua chon: ", 0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (chon) {
        case 1: ql.themTour(); break;
        case 2: ql.hienThi();  break;
        case 3: ql.timKiem();  break;
        case 4: ql.suaTour();  break;
        case 5: ql.xoaTour();  break;
        case 0: cout << "Quay lai menu chinh.\n"; break;
        default: cout << "Lua chon khong hop le!\n";
        }
    } while (chon != 0);
}

void chayMenuHoaDon(Quanlyhoadon& qlhd) {
    int luachon;
    do {
        cout << "\n===== QUAN LY HOA DON & THONG KE DU LICH =====\n"
             << "1. Lap hoa don moi.\n"
             << "2. Hien thi danh sach hoa don.\n"
             << "3. Cap nhat trang thai hoa don.\n"
             << "4. In chi tiet hoa don.\n"
             << "5. Thong ke tong doanh thu.\n"
             << "6. Thong ke doanh thu theo thoi gian (Ngay/Thang/Quy/Nam).\n"
             << "7. Thong ke doanh thu theo tour (Cao nhat / Thap nhat).\n"
             << "8. Thong ke so luong khach hang dang ky tour.\n"
             << "9. Thong ke tinh trang hoa don (Bang & Bieu do cot).\n"
             << "0. Quay lai.\n";
        luachon = nhapSo<int>("Nhap lua chon cua ban: ");
        switch (luachon) {
            case 1: qlhd.LapHoaDon(); break;
            case 2: qlhd.HienThiHoaDon(); break;
            case 3: qlhd.CapNhatHoaDon(); break;
            case 4: qlhd.InChiTietHoaDon(); break;
            case 5: qlhd.ThongKeTongDoanhThu(); break;
            case 6: qlhd.ThongKeTheoThoiGian(); break;
            case 7: qlhd.ThongKeTheoTour(); break;
            case 8: qlhd.ThongKeSoLuongKhach(); break;
            case 9: qlhd.ThongKeHoaDon(); break;
            case 0: cout << "Quay lai menu chinh.\n"; break;
            default: cout << "Lua chon khong hop le, vui long nhap lai!\n";
        }
    } while (luachon != 0);
}

// ============================================================
// PHAN 8: DOC VA GHI TOAN BO DU LIEU
// ============================================================
void thongBaoDoc(const string& ten, int n) {
    if (n < 0) cout << "  [THONG BAO] Chua co file " << ten << ", se tao moi khi luu.\n";
    else       cout << "  Da doc " << n << " ban ghi tu " << ten << ".\n";
}

void docTatCa(vector<KhachHang>& dsKH, DanhSachNhanVien& dsNV,
              QuanLyTour& dsTour, Quanlyhoadon& dsHD) {
    thongBaoDoc(FILE_KHACHHANG, docKhachHang(dsKH, FILE_KHACHHANG));
    thongBaoDoc(FILE_NHANVIEN,  dsNV.docFile(FILE_NHANVIEN));
    thongBaoDoc(FILE_TOUR,      dsTour.docFile(FILE_TOUR));
    thongBaoDoc(FILE_HOADON,    dsHD.docFile(FILE_HOADON));
}

void luuTatCa(const vector<KhachHang>& dsKH, const DanhSachNhanVien& dsNV,
              const QuanLyTour& dsTour, const Quanlyhoadon& dsHD) {
    if (!luuKhachHang(dsKH, FILE_KHACHHANG)) cout << "[LOI] Khong ghi duoc " << FILE_KHACHHANG << " (kiem tra thu muc data/ co ton tai)\n";
    if (!dsNV.luuFile(FILE_NHANVIEN))        cout << "[LOI] Khong ghi duoc " << FILE_NHANVIEN << "\n";
    if (!dsTour.luuFile(FILE_TOUR))          cout << "[LOI] Khong ghi duoc " << FILE_TOUR << "\n";
    if (!dsHD.luuFile(FILE_HOADON))          cout << "[LOI] Khong ghi duoc " << FILE_HOADON << "\n";
}

// ============================================================
// PHAN 9: HAM MAIN
// ============================================================
int main() {
    vector<KhachHang> dsKH;
    DanhSachNhanVien dsNV;
    QuanLyTour dsTour;
    Quanlyhoadon dsHD;

    docTatCa(dsKH, dsNV, dsTour, dsHD);

    int chon;
    do {
        cout << "\n===== HE THONG QUAN LY DU LICH =====\n"
             << "1. Quan ly khach hang\n"
             << "2. Quan ly nhan vien\n"
             << "3. Quan ly tour\n"
             << "4. Quan ly hoa don & thong ke\n"
             << "0. Thoat\n";
        chon = docSoNguyen(cin, "Nhap lua chon: ", 0);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (chon) {
            case 1: chayMenuKhachHang(dsKH); break;
            case 2: chayMenuNhanVien(dsNV);  break;
            case 3: chayMenuTour(dsTour);    break;
            case 4: chayMenuHoaDon(dsHD);    break;
            case 0: cout << "Ket thuc chuong trinh!\n"; break;
            default: cout << "Lua chon khong hop le!\n";
        }
        luuTatCa(dsKH, dsNV, dsTour, dsHD);
    } while (chon != 0);

    return 0;
}

#endif // KHACHHANG2_H

