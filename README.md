# CAU TRUC DU LIEU MO RONG - HOC KY 231

# Nhom gom 2 thanh vien:

## Nguyen Dang Anh Khoa - 2010339

## Nguyen Xuan Trieu

# Cau truc thu muc

- File README.md: Huong dan chay chuong trinh.
- Folder `data`: Chua du lieu ve ten dia danh o Viet Nam.
- Folder `src`: Chua ma nguon chinh cua chuong trinh (cac file .h, .cpp, file object .o)

* Cac file `build_addr_map_*.o` la cac file object chua bien luu tru thong tin tu dien tra cuu ten dia danh da duoc bien dich.
* `extract_inp_new.cpp`, `extract_inp_new.h`: Chua cac ham xu ly cua chuong trinh.
* `main_test.cpp`: File chinh cua chuong trinh.

# Cac buoc chay chuong trinh

- B1: Nhap cac chuoi input. Trong file main_test.cpp,

* Mang `std::vector<std::string> str_lst` la danh sach cac chuoi dia chi dau vao can xu ly. Khai bao chuoi muon xu ly vao mang nay.
* Dat ten file CSV chua ket qua dau ra tai dong `std::ofstream outputFile("results_final.csv")`.

- B2: Mo Powershell tai folder `src` (la folder chua file `main_test.cpp`) va chay cau lenh sau de bien dich chuong trinh:
  `g++ build_addr_map_1_10.o build_addr_map_11_20.o build_addr_map_21_30.o build_addr_map_31_40.o build_addr_map_41_50.o build_addr_map_51_60.o build_addr_map_61_63.o extract_inp_new.cpp main_test.cpp -o res_test`
- B3: Chay chuong trinh bang cau lenh:
  `./res_test`
- B4: Khi chay xong, kiem tra ket qua tai file `results_final.csv` o cung thu muc.

## Hien da co file ket qua cho cac public test tai folder `src` (xem file `result_main.csv`)

## Nhom chuc thay nhieu suc khoe!
