#ifndef KHACHHANG1_H
#define KHACHHANG1_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

class KhachHang {
private:
    string maKH;
    string hoTen;
    string sdt;
    string ngaySinh;
    string diaChi;
    string loaiKH;

public:
    KhachHang();
    KhachHang(string ma, string ten, string soDT, string ns, string dc, string loai);

    string getMaKH() const;
    void setMaKH(const string& ma);

    string getHoTen() const;
    void setHoTen(const string& ten);

    string getSdt() const;
    void setSdt(const string& soDT);

    string getNgaySinh() const;
    void setNgaySinh(const string& ns);

    string getDiaChi() const;
    void setDiaChi(const string& dc);

    string getLoaiKH() const;
    void setLoaiKH(const string& loai);

    double tinhPhanTramGiamGia() const;

    void nhapThongTin();
    void xuatThongTin() const;

    string toChuoiFile() const;
    bool docTuChuoiFile(const string& line);
};

bool docFileKhachHang(const string& tenFile, vector<KhachHang>& dsKH);
bool ghiFileKhachHang(const string& tenFile, const vector<KhachHang>& dsKH);
void hienThiDanhSachKH(const vector<KhachHang>& dsKH);
void themKhachHang(vector<KhachHang>& dsKH);

KhachHang* timKiemTheoMaKH(vector<KhachHang>& dsKH, const string& maCanTim);
vector<KhachHang> timKiemTheoTenKH(const vector<KhachHang>& dsKH, const string& tuKhoaTen);
vector<KhachHang> timKiemTheoSdtKH(const vector<KhachHang>& dsKH, const string& sdtCanTim);
vector<KhachHang> locKhachHangTheoLoai(const vector<KhachHang>& dsKH, const string& loaiCanLoc);

#endif