# **Quy luật chung đọc trước khi làm và xem cả ví dụ**

* phân công

| thanh vien | job |

|Le Duc Hieu | khung chung, test, ghep code, khach hang |

|Nguyen Duc Dong | tim kiem |

|Nguyen Quoc Viet | hoa don, thong ke|

|Ngo Van Thinh | nhan vien |

|Do Hoang Minh Duc | tour |

### \*\* Quy ước code

Số: Viết liền, không có dấu chấm, dấu phẩy ngăn hàng nghìn (3500000)

Ngày: Luôn dd/mm/yyyy, đủ 2 chữ số (05/11/1988)

Mã: Tiền tố cố định: KH, NV, T, HD + số

Giá trị loại: Cố định viết hoa: HDV, SALES, TN, QT, Thuong, VIP

Ký tự ngăn cách	|, không có dấu cách hai bên (KH001|Nguyen Van An, không phải KH001 | Nguyen Van An)

Trình tự code nhập dữ liệu khách hàng:( mã | họ tên | SĐT | ngày sinh |địa chỉ |loại)

Trình tự code nhập dữ liệu nhan vien: (loại |mã |họ tên |SĐT |ngày sinh |lương cơ bản |thông số riêng| HDV| số tour đã dẫn| SALES| hoa hồng %)

Trình tự code nhập dữ liệu tour: (loại |mã |tên |điểm đến |ngày đi |số ngày |giá cơ bản |số chỗ |số chỗ đã đặt| tour quốc tế có thêm trường cuối là phụ phí visa)

Trình tự code nhập dữ liệu hoá đơn: (mã HD |mã KH |mã tour |mã NV |số người |ngày lập |tổng tiền)

Ten lop viet hoa chu dau (KhachHang)

Ten file khong dau mỗi file mới thêm số để khác không để trùng file



#### **Làm ở trong phần có tên của mình ko làm sang phần có tên của người khác và không được làm vào phần main**



#### **# Quy tac nghiep vu**

###### \- Khach VIP giam 10% tren tong tien hoa don.

###### \- Gia tour quoc te = gia co ban + phu phi visa.

###### \- Luong HDV = luong co ban + 200000 x so tour da dan.

###### \- Luong sales = luong co ban + hoa hong % x tong doanh so hoa don da lap. 

###### \# quy tắc git

###### \- Xong mot phan thi tao Pull Request, nhom truong gop.

###### \- Truoc khi lam: Fetch/Pull origin. Ghi chu commit ro rang.

### **Nhớ lưu file k có dấu ko dấu cách ko chưa kí tự đặc biệt**
Không commit file rác (thư mục Debug, x64, .vs, file .exe): nhìn tab Changes trước khi commit, thấy lạ thì hỏi.
Không hiểu cái gì thì hỏi t trước khi tự làm nha 



