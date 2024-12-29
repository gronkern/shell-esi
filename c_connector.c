#include<stdio.h>
#include<stdlib.h>
#include "coba.h"
#include "boolean.h"
#include<string.h>
#include<conio.h>
#define coba 13
#define tes 9
#define del 8

struct user{
	char fullName[50];
	char noKTP[16];
	char email[50];
	char password[50];
	char username[50];
	char phone[50];
	char keberangkatan[50];
	char Tujuan[50];
	char kode[50];
	char bisnis[50];
	char premium[50];
};

void Input(char ch[50]){
	fgets(ch,50,stdin);
	ch[strlen(ch) - 1] = 0; 
}

char cekUser(char email[50],char username[50]){
    int i;
	for(i=0;i<strlen(email);i++){
		if(email[i] == '@') break;
		else username[i] = email[i];
	}
}

void Password(char pwd[50]){
	int i;
	char ch;
	while(1){
		ch = getch();
		if(ch == coba || ch == tes){
			pwd[i] = '\0';
			break;
		}else if(ch == del){
			if(i>0){
				i--;
				printf("\b \b");
			}
		}else{
			pwd[i++] = ch;
			printf("* \b");
		} 
	}
}

void cetakInformasi(KA *data) {
  system("cls"); //linux = clear, windows = cls
  char *choice;
  char *key;

  choice = (char *)malloc(1 * sizeof(char));
  key = (char *)malloc(1 * sizeof(char));
  key = "y";

  while (data != NULL) {
    printf("Nama Kereta Api\t\t: %s\n", data->namaKeretaApi);
    printf("Destinasi\t\t: %s\n", data->destinasi);
    printf("Jam Keberangkatan\t: %s\n", data->jam_keberangkatan);
    printf("Daya Tampung\t\t: %d Orang\n", data->daya_tampung);
    printf("Tiket Tersedia\t\t: %d Tiket\n", data->sisa_tiket);
    printf("Kelas Tersedia\t\t: %s\n", data->jenis_kelas);
    printf("\n=======================================================\n\n");
    data = data->nextNode;
  }

  printf("Apakah anda ingin keluar? (enter y to exit/any key to continue) : ");
  scanf("%s", choice);
  puts("");

  if (strcmp(choice, key) == 0) {
    exit(1);
  }

  system("cls"); //linux = clear, windows = cls
}


int main(){
	FILE *fp;
	int opt,usrFound = 0;
	int pilihan,pil,option,pilih;
	struct user user;
    char username[50],pword[50],email[50];
	char password2[50];
	char keberangkatan[50];
	char Tujuan[50];
	char kode[50];
	char bisnis[50];
	char premium[50];
	int pilihan_menuAwal, pilih_ka, pilih_kelas, pesan_jumlah_tiket, sistemTiket;

  	KA *kereta_api1 = (KA *)malloc(sizeof(KA) * 1);
  	KA *kereta_api2 = (KA *)malloc(sizeof(KA) * 1);
  	KA *kereta_api3 = (KA *)malloc(sizeof(KA) * 1);
    struct user usr;
	
	menu: 
	printf("\n\t\t---------- Selamat datang di sistem kami ----------");
	printf("\nTentukan Pilihan anda");
	printf("\n1.Register");
	printf("\n2.Login");
	printf("\n3.Exit");

	printf("\n\nPilihan Anda :\t");
	scanf("%d",&opt);
	fgetc(stdin);
	
	switch(opt){
		case 1:
			system("cls");
			printf("No KTP :\t");
			Input(user.noKTP);
			printf("\nNama lengkap :\t");
			Input(user.fullName);
			printf("Nomor Handphone :\t");
			Input(user.phone);
			printf("Email:\t");
			Input(user.email);
			printf("Masukkan password:\t");
			Password(user.password);
			printf("\nMasukkan Konfirmasi password:\t");
			Password(password2);
			
			if(!strcmp(user.password,password2)){
			    cekUser(user.email,user.username);
				fp = fopen("Users.txt","a+");
				fwrite(&user,sizeof(struct user),1,fp);
				if(fwrite != 0) printf("\n\nUser berhasil terdaftar");
				else printf("\n\nMaaf , Sepertinya ada kesalahan dalam pengisian :(");
				fclose(fp);
			}
			else{
				printf("\n\nPassword tidak sama");
			}
            goto menu;
            break;
		case 2:			
			printf("\nEmail :\t");
			Input(email);
			printf("Password :\t");
			Password(pword);
			
			fp = fopen("Users.txt","r");
			while(fread(&usr,sizeof(struct user),1,fp)){
				if(!strcmp(usr.email,email)){
					if(!strcmp(usr.password,pword)){
						system("cls");
						if(!strcmp(usr.fullName,"admin")){
							menu_utama:
							printf("1. Kelola Akun\n");
							printf("2. Kelola Kota\n");
							printf("3. Kelola Rute\n");
							printf("4. Kelola Rute\n");
							printf("5. Lihat Jadwal Kereta Api\n");
							printf("0. Logout\n");
							printf("Masukkan pilihan anda : ");
							scanf("%d",&option);
							if(option==1){
								
							}else if(option==3){
								printf("1. Tambah Data Rute\n");
								printf("2. Lihat Data Rute\n");
								printf("3. Edit Data Kota\n");
								printf("4. Delete Data Kota\n");
								printf("0. Logout\n");
								printf("Masukkan pilihan anda : ");
								scanf("%d",&pilih);
								if(pilih==1){
									printf("Tambah Data Rute :\t");
									Input(user.keberangkatan);
									Input(user.Tujuan);
									Input(user.kode);
									Input(user.bisnis);
									Input(user.premium);
									if(!user.keberangkatan){
										fp = fopen("Keberangkatan.txt","a+");
										fwrite(&user,sizeof(struct user),1,fp);
										if(fwrite != 0) printf("\n\nRute berhasil Ditambahkan");
										else printf("\n\nMaaf , Sepertinya ada kesalahan dalam pengisian :(");
										fclose(fp);
									}
									else{
										printf("\n\nPassword tidak sama");
									}
								}
							}
						}else{ 
							printf("1. Booking Tiket\n");
							printf("2. Kelola Profile\n");
							printf("3. History Pembelian\n");
							printf("0. Logout\n");
							printf("Masukkan pilihan anda : ");
							scanf("%d",&pilihan);
							if(pilihan==1){
								printf("1. Informasi Kereta Api\n");
								printf("2. Beli Tiket\n");
								scanf("%d",&pil);
								if(pil==1){
								}
							}
						}
					}
					else {
						printf("\n\nPassword salah!");
					}
					usrFound = 1;
				}
			}
			if(!usrFound){
				printf("\n\nUser tidak terdaftar!");
			}
			fclose(fp);
			break; 
			
		case 3:
			printf("\t\t Terimakasih telah berkunjung)");
			return 0;
			
	}
	return 0;
}
