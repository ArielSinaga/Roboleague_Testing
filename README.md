# Teknofest_Roboleauge 

# Workflow & Workflow
### Berikut Struktur dari Repo ini:
```
.
├── include
│   ├── task_sub_deneyap
│   ├── task_main_deneyap
│   │   ├── ARM.hpp
│   │   ├── CTRL.hpp
│   │   ├── I2C.hpp
│   │   ├── pins.hpp
│   │   ├── RFID.hpp
│   │   ├── SemaQHandlers.hpp
│   │   ├── SharedData.hpp
│   │   ├── TaskHandlers.hpp
│   │   └── Zipper.hpp
├── lib
│   └── README
├── platformio.ini
├── README.md
├── src
│   ├── ctype_fix.c
│   ├── main_deneyap.cpp
│   ├── sub_deneyap.cpp
│   ├── task_sub_deneyap
│   ├── task_main_deneyap
│   │   ├── ARM.cpp
│   │   ├── CTRL.cpp
│   │   ├── I2C.cpp
│   │   ├── RFID.cpp
│   │   └── Zipper.cpp
└── test
    └── README

12 directories, 23 files
```

> [!IMPORTANT]
> DILARANG UNTUK MENGUBAH STRUKTUR DARI REPO INI
> INFO LENGKAPNYA ADA PADA ATURAN REPOSITORI DIBAWAH INI

# 📌 ATURAN REPOSITORI SELAMA KALIAN MAGANG

---

## 1. GIT and Flow

Dibebaskan tetapi sebisa mungkin menggunakan **Git Best Practice** dengan spesifikasi berikut:
### Brancing:
* `main`
    * Branch utama yang berisi kode produksi stabil dan siap pakai.
    * *Proteksi:* Dilarang keras melakukan `push` langsung ke branch ini.
* `dev/<nama-sub-proyek>`
    * Branch untuk lingkungan pengembangan spesifik atau modul besar.
    * pakai dev-main sebagai branch dev utama atau pakai main aja jika kalian mau
    * *Contoh Penggunaan:* `dev/ugv-navi`, `dev/mops-pipeline`, `dev/hardware-esp32`.
* `bugfix/<deskripsi-error>` atau `hotfix/<deskripsi-error>`
    * Branch untuk perbaikan bug, kesalahan logika, atau error sistem.
    * *Contoh:* `bugfix/fix-i2c-collision`, `hotfix/vlan-arp-cache`
### Format Commit:
```bash

feat: Penambahan fitur baru atau komponen kode baru.

fix: Perbaikan bug, error, atau penanganan pengecualian (exception).

docs: Perubahan atau penambahan dokumentasi (seperti file README atau komentar program).

refactor: Restrukturisasi kode internal tanpa mengubah fungsionalitas eksternal.

style: Perubahan terkait formatting kode (spasi, titik koma, indentasi) tanpa mengubah
```
### 
---

## 2. Workflow

* *Header* **WAJIB** di taruh di folder <code>include/</code> dan implementasi dari *Header* nya wajib di taruh di folder <code>src/</code>
* Untuk Mulitiple Microcontroller semisal memiliki 2 atau lebih microcontroller dengan fungsi yang beda beda maka **WAJIB** menambah ***env*** baru pada pio, Berikut Contohnya:

 Semisal Kalian Memiliki 3 Microcontroller(Contoh Microcontroller: ESP32,STM32,Arduino dll) yang dimana 1 Microcontroller berfungsi sebagai yang utamanya , 1 untuk penggerak dan 1 untuk sensor (asumsi bahwa microcontroller utama menggunakan STM32 dan sisanya ESP) maka kalian **WAJIB** modify platformio.ini dengan format:
 ```pio
[env:main];<-- nama env untuk (microcontroller/micon utama)
platform = ;<sesuain sama micon nya contoh make STM: ststm32> 
board = ;<sesuain sama series boardnya misal stm bluepill f103c8 maka :bluepill_f103c8>
framework = arduino
monitor_speed = 115200 ;<-- ga perlu di ubah
upload_speed = 921600 ;<-- ga perlu di ubah
build_src_filter = 
    +<main.cpp>;<-- tambahin file .cpp mana aja yang dipake sama micon utamanya kek contoh main.cpp
    +<tasks/*.cpp>;<-- tambahin file .cpp mana aja yang dipake sama micon utamanya kek contoh tasks/*.cpp
    +<ctype_fix.c> <-- tambahin juga file 
lib_deps =
    ;<TAMBAH TAMBAHIN LIBRARY YANG PERLU>
    ;<lib1>
    ;<lib2>
     ;dst..
build_flags =
    ; tambahin jika perlu

[env:sub1]<-- nama env untuk (microcontroller/micon kedua)
platform = ;sesuain sama micon nya contoh make ESP: espressif32>
board = ;sesuain sama series boardnya misal stm bluepill f103c8 maka :bluepill_f103c8 kalau esp esp32dev
framework = arduino
monitor_speed = 115200 ;<-- ga perlu di ubah
upload_speed = 921600 ;<-- ga perlu di ubah
build_src_filter = 
    +<sub1.cpp>
    +<ctype_fix.c>
    +<tasks/*.cpp>
lib_deps =
    ;<TAMBAH TAMBAHIN LIBRARY YANG PERLU>
    ;<lib1>
    ;<lib2>
     ;dst..
build_flags =
    ; tambahin jika perlu

[env:<nama env untuk esp(microcontroller/micon 3)> ]
platform = ;sesuain sama micon nya contoh make ESP: espressif32>
board = ;sesuain sama series boardnya misal stm bluepill f103c8 maka :bluepill_f103c8 kalau esp esp32dev
framework = arduino
monitor_speed = 115200 ;<-- ga perlu di ubah
upload_speed = 921600 ;<-- ga perlu di ubah
build_src_filter = 
    +<sub1.cpp>
    +<ctype_fix.c>
    +<tasks/*.cpp>
lib_deps =
    ;<TAMBAH TAMBAHIN LIBRARY YANG PERLU>
    ;<lib1>
    ;<lib2>
     ;dst..
build_flags =
    ; tambahin jika perlu
;jika pake sub-module pio kayak yang sekarang dipake formatnya jadi:

; Template dasar untuk semua sub-module berbasis ESP32
[env_sub_esp_base]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 921600 

; Implementasi pada sub-module 3
[env:sub3]
extends = env_sub_esp_base ; Mewarisi konfigurasi dari template di atas
build_src_filter = 
    +<sub3.cpp>
    +<ctype_fix.c>
    +<tasks_sub3/*.cpp>
lib_deps =
    ; Masukkan library spesifik sub3 di sini jika ada

```


