#ifndef KA_HEADER

#include "Btrans.h"

//pembuatan struct beserta data datanya
#define MODAL_DASAR 3500000

typedef struct SKA KA;

struct SKA {
  int kode_kereta;
  char namaKeretaApi[255];
  char destinasi[255];
  char jam_keberangkatan[50];
  char jenis_kelas[100];
  int daya_tampung;
  int sisa_tiket;
  int kelas_eksekutif;
  int kelas_bisnis;
  int kelas_biasa;
  int harga_eksekutif;
  int harga_bisnis;
  int harga_biasa;
  int id_eksekutif;
  int id_bisnis;
  int id_biasa;
  int keuntungan_kereta;

  KA *nextNode;
};

//fungsi dan prosedur yang dibutuhkan untuk menjalankan sistem nantinya

void cetakInformasi(KA *data);

void daftarKa(KA *data);

void pilihanKa(KA *data, int pilih, int pesan_jumlah);

boolean operasitiket(KA *data, int pilih_kelas, int pesan_jumlah, int pilih_ka);

void hitungKeuntungan(KA *data, int pilih_ka, int pilih_kelas,
                      int pesan_jumlah_tiket);

void cetakKeuntungan(KA *data);

void hargaNaik(KA *data, int sz_eksekutif1, int sz_bisnis1, int sz_biasa1,
               int sz_eksekutif2, int sz_bisnis2, int sz_biasa2,
               int sz_eksekutif3, int sz_bisnis3, int sz_biasa3,
               int harga_eksekutif1, int harga_bisnis1, int harga_biasa1,
               int harga_eksekutif2, int harga_bisnis2, int harga_biasa2,
               int harga_eksekutif3, int harga_bisnis3, int harga_biasa3,
               int pilih_ka);

#endif
