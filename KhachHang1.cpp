#include "KhachHang1.h"

static bool kiemTraNgayHopLe(const string& ngay) {
    if (ngay.length() != 10) return false;
    if (ngay[2] != '/' || ngay[5] != '/') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(ngay[i])) return false;
    }
    return true;
}

static string toLowerStr(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

KhachHang::KhachHang() {
    maKH = "";
    hoTen = "";
    sdt = "";
    ngaySinh = "01/01/2000";
    diaChi = "";
    loaiKH = "Thuong";
}

KhachHang::KhachHang(string ma, string ten, string soDT, string ns, string dc, string loai) {
    maKH = ma;
    hoTen = ten;
    sdt = soDT;
    ngaySinh = ns;
    diaChi = dc;
    loaiKH = loai;
}

string KhachHang::getMaKH() const { return maKH; }
void KhachHang::setMaKH(const string& ma) { maKH = ma; }

string KhachHang::getHoTen() const { return hoTen; }
void KhachHang::setHoTen(const string& ten) { hoTen = ten; }

string KhachHang::getSdt() const { return sdt; }
void KhachHang::setSdt(const string& soDT) { sdt = soDT; }

string KhachHang::getNgaySinh() const { return ngaySinh; }
void KhachHang::setNgaySinh(const string& ns) { ngaySinh = ns; }

string KhachHang::getDiaChi() const { return diaChi; }
void KhachHang::setDiaChi(const string& dc) { diaChi = dc; }

string KhachHang::getLoaiKH() const { return loaiKH; }
void KhachHang::setLoaiKH(const string& loai) { loaiKH = loai; }

double KhachHang::tinhPhanTramGiamGia() const {
    if (loaiKH == "VIP") {
        return 0.10;
    }
    return 0.0;
}

void KhachHang::nhapThongTin() {
    do {
        cout << "Nhap ma KH (bat dau bang KH, VD: KH001): ";
        getline(cin, maKH);
        if (maKH.rfind("KH", 0) != 0 || maKH.length() <= 2) {
            cout << "[Loi] Ma KH phai bat dau bang 'KH' kem theo so!\n";
        } else {
            break;
        }
    } while (true);

    cout << "Nhap ho ten: ";
    getline(cin, hoTen);

    cout << "Nhap SDT (viet lien): ";
    getline(cin, sdt);

    do {
        cout << "Nhap ngay sinh (dd/mm/yyyy, VD: 05/11/1988): ";
        getline(cin, ngaySinh);
        if (!kiemTraNgayHopLe(ngaySinh)) {
            cout << "[Loi] Ngay sinh phai dung dinh dang dd/mm/yyyy (du 2 chu so)!\n";
        } else {
            break;
        }
    } while (true);

    cout << "Nhap dia chi: ";
    getline(cin, diaChi);

    do {
        cout << "Nhap loai khach hang (Thuong / VIP): ";
        getline(cin, loaiKH);
        if (loaiKH != "Thuong" && loaiKH != "VIP") {
            cout << "[Loi] Gia tri loai phai viet dung 'Thuong' hoac 'VIP'!\n";
        } else {
            break;
        }
    } while (true);
}

void KhachHang::xuatThongTin() const {
    cout << left << setw(10) << maKH
         << setw(25) << hoTen
         << setw(15) << sdt
         << setw(15) << ngaySinh
         << setw(25) << diaChi
         << setw(10) << loaiKH << endl;
}

string KhachHang::toChuoiFile() const {
    return maKH + "|" + hoTen + "|" + sdt + "|" + ngaySinh + "|" + diaChi + "|" + loaiKH;
}

bool KhachHang::docTuChuoiFile(const string& line) {
    if (line.empty()) return false;
    stringstream ss(line);
    string token;
    vector<string> fields;

    while (getline(ss, token, '|')) {
        fields.push_back(token);
    }

    if (fields.size() < 6) return false;

    maKH = fields[0];
    hoTen = fields[1];
    sdt = fields[2];
    ngaySinh = fields[3];
    diaChi = fields[4];
    loaiKH = fields[5];

    if (!loaiKH.empty() && loaiKH.back() == '\r') {
        loaiKH.pop_back();
    }
    return true;
}

bool docFileKhachHang(const string& tenFile, vector<KhachHang>& dsKH) {
    ifstream fileIn(tenFile);
    if (!fileIn.is_open()) {
        return false;
    }
    dsKH.clear();
    string line;
    while (getline(fileIn, line)) {
        if (line.empty()) continue;
        KhachHang kh;
        if (kh.docTuChuoiFile(line)) {
            dsKH.push_back(kh);
        }
    }
    fileIn.close();
    return true;
}

bool ghiFileKhachHang(const string& tenFile, const vector<KhachHang>& dsKH) {
    ofstream fileOut(tenFile);
    if (!fileOut.is_open()) {
        return false;
    }
    for (const auto& kh : dsKH) {
        fileOut << kh.toChuoiFile() << "\n";
    }
    fileOut.close();
    return true;
}

void hienThiDanhSachKH(const vector<KhachHang>& dsKH) {
    if (dsKH.empty()) {
        cout << "Danh sach khach hang trong!\n";
        return;
    }
    cout << string(100, '-') << endl;
    cout << left << setw(10) << "Ma KH"
         << setw(25) << "Ho Ten"
         << setw(15) << "SDT"
         << setw(15) << "Ngay Sinh"
         << setw(25) << "Dia Chi"
         << setw(10) << "Loai" << endl;
    cout << string(100, '-') << endl;
    for (const auto& kh : dsKH) {
        kh.xuatThongTin();
    }
    cout << string(100, '-') << endl;
}

void themKhachHang(vector<KhachHang>& dsKH) {
    KhachHang khMoi;
    cout << "\n--- NHAP KHACH HANG MOI ---\n";
    khMoi.nhapThongTin();

    if (timKiemTheoMaKH(dsKH, khMoi.getMaKH()) != nullptr) {
        cout << "[Loi] Ma khach hang " << khMoi.getMaKH() << " da ton tai!\n";
        return;
    }
    dsKH.push_back(khMoi);
    cout << "-> Da them khach hang thanh cong!\n";
}

KhachHang* timKiemTheoMaKH(vector<KhachHang>& dsKH, const string& maCanTim) {
    for (auto& kh : dsKH) {
        if (kh.getMaKH() == maCanTim) {
            return &kh;
        }
    }
    return nullptr;
}

vector<KhachHang> timKiemTheoTenKH(const vector<KhachHang>& dsKH, const string& tuKhoaTen) {
    vector<KhachHang> ketQua;
    string key = toLowerStr(tuKhoaTen);
    for (const auto& kh : dsKH) {
        if (toLowerStr(kh.getHoTen()).find(key) != string::npos) {
            ketQua.push_back(kh);
        }
    }
    return ketQua;
}

vector<KhachHang> timKiemTheoSdtKH(const vector<KhachHang>& dsKH, const string& sdtCanTim) {
    vector<KhachHang> ketQua;
    for (const auto& kh : dsKH) {
        if (kh.getSdt().find(sdtCanTim) != string::npos) {
            ketQua.push_back(kh);
        }
    }
    return ketQua;
}

vector<KhachHang> locKhachHangTheoLoai(const vector<KhachHang>& dsKH, const string& loaiCanLoc) {
    vector<KhachHang> ketQua;
    for (const auto& kh : dsKH) {
        if (kh.getLoaiKH() == loaiCanLoc) {
            ketQua.push_back(kh);
        }
    }
    return ketQua;
}