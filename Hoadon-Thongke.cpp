#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

// Enum định nghĩa các trạng thái thanh toán
enum TrangThaiThanhToan {
    CHUA_THANH_TOAN,
    DA_DAT_COC,
    DA_THANH_TOAN,
    DA_HUY
};

// Hàm hỗ trợ chuyển enum thành chuỗi để hiển thị
string trangThaiToString(TrangThaiThanhToan trangThai) {
    switch (trangThai) {
        case CHUA_THANH_TOAN: return "Chua thanh toan";
        case DA_DAT_COC:      return "Da dat coc";
        case DA_THANH_TOAN:   return "Da thanh toan";
        case DA_HUY:          return "Da huy";
        default:              return "Khong xac dinh";
    }
}

// Hàm nhập số nguyên/số thực an toàn
template <class T>
T nhapSo(const string& loiNhac) {
    T v;
    while (true) {
        cout << loiNhac;
        if (cin >> v) return v;
        cout << "  Gia tri khong hop le, vui long nhap lai!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Kiem tra ngay theo dung dinh dang DD/MM/YYYY
bool tachNgayHopLe(const string& s, int& ngay, int& thang, int& nam) {
    if (s.size() != 10 || s[2] != '/' || s[5] != '/') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (s[i] < '0' || s[i] > '9') return false;
    }

    ngay = stoi(s.substr(0, 2));
    thang = stoi(s.substr(3, 2));
    nam = stoi(s.substr(6, 4));
    if (nam < 1 || thang < 1 || thang > 12) return false;

    int soNgay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool namNhuan = (nam % 400 == 0 || (nam % 4 == 0 && nam % 100 != 0));
    if (namNhuan) soNgay[1] = 29;
    return ngay >= 1 && ngay <= soNgay[thang - 1];
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

    HoaDon(string maKH, string maT, string ngayLap, int slNL, int slTE, double giaNL, double giaTE, string ptThanhToan)
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

    // Getters bổ sung phục vụ Thống kê
    string getMaHD() const { return MaHD; }
    string getMaKH() const { return MaKH; }
    string getMaT() const { return MaT; }
    string getNgayLap() const { return NgayLap; }
    int getSoLuongNguoiLon() const { return SoLuongNguoiLon; }
    int getSoLuongTreEm() const { return SoLuongTreEm; }
    double getTongTien() const { return TongTien; }
    TrangThaiThanhToan getTrangThai() const { return TrangThai; }
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

    // ==========================================
    // CÁC CHỨC NĂNG THỐNG KÊ VÀ BÁO CÁO
    // ==========================================

    // 1. Thống kê tổng doanh thu (chỉ tính hóa đơn DA_THANH_TOAN)
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

        // key la khoa thoi gian co thu tu; value.first la nhan hien thi.
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

    // 3. Thống kê theo Tour (Doanh thu cao nhất / thấp nhất)
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

    // 4. Thống kê số lượng khách hàng đăng ký tour
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

    // 5. Thống kê hóa đơn
    void ThongKeHoaDon() const {
        int demChuaTT = 0, demDatCoc = 0, demDaTT = 0, demDaHuy = 0;
        int tong = static_cast<int>(dsHoaDon.size());

        for (const auto& hd : dsHoaDon) {
            switch (hd.getTrangThai()) {
                case CHUA_THANH_TOAN: demChuaTT++; break;
                case DA_DAT_COC:      demDatCoc++; break;
                case DA_THANH_TOAN:   demDaTT++; break;
                case DA_HUY:          demDaHuy++; break;
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

        // Bieu do cot phan bo trang thai (ASCII bar chart)
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
};

int main() {
    Quanlyhoadon qlhd;
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
             << "0. Thoat.\n";
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
            case 0: cout << "Thoat chuong trinh\n"; break;
            default: cout << "Lua chon khong hop le, vui long nhap lai!\n";
        }
    } while (luachon != 0);
    return 0;
}