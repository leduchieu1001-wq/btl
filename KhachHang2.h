#ifndef KHACHHANG2_H
#define KHACHHANG2_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
using namespace std;

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
        if (n.size() != 10 || n[2] != '/' || n[5] != '/') return false;
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

#endif