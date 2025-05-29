#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
using namespace std;

struct dropOfLaundry
{
    int id;
    char nama[50];
    char noTelp[15];
    char tanggal[20];
    char layanan[10];
    float berat;
    int harga;
    dropOfLaundry *next;
};

dropOfLaundry *head = NULL; // Pointer head untuk linked list Drop-Off

int hitungIDTerbesar()
{
    int lastID = 0;
    dropOfLaundry temp;

    FILE *file = fopen("dropOfDataLaundry.dat", "rb");
    if (file)
    {
        while (fread(&temp, sizeof(dropOfLaundry), 1, file)) // Baca semua data dan ambil ID terakhir (paling besar dalam file)
            lastID = temp.id;
        fclose(file);
    }

    FILE *fileRiw = fopen("dropOfDataLaundryRiwayat.dat", "rb");
    if (fileRiw)
    {
        while (fread(&temp, sizeof(dropOfLaundry), 1, fileRiw)) // Baca semua data riwayat dan cari ID terbesar
            if (temp.id > lastID)
                lastID = temp.id;
        fclose(fileRiw);
    }

    return lastID;
}

int hitungHarga(const char *layanan, float berat)
{
    if (strcmp(layanan, "reguler") == 0) // Jika layanan reguler, harga Rp7.000 per kg
        return (int)(7000 * berat);
    else if (strcmp(layanan, "express") == 0) // Jika layanan express, harga Rp10.000 per kg
        return (int)(10000 * berat);
    return 0;
}

void loadDataFromFile() // Fungsi untuk memuat data dari file ke dalam linked list
{
    FILE *file = fopen("dropOfDataLaundry.dat", "rb"); // Buka file biner berisi data laundry Drop-Off
    if (!file)                                         // Jika file tidak ditemukan, set head ke NULL dan keluar
    {
        head = NULL;
        return;
    }

    dropOfLaundry temp;         // Menyimpan data sementara dari file
    dropOfLaundry *last = NULL; // Penunjuk ke node terakhir dalam list
    head = NULL;                // Awalnya list kosong

    while (fread(&temp, sizeof(dropOfLaundry), 1, file)) // Baca file dan masukkan setiap data ke dalam linked list
    {
        dropOfLaundry *baru = new dropOfLaundry; // Alokasikan memori untuk node baru dan salin datanya
        *baru = temp;
        baru->next = NULL;

        if (head == NULL) // Jika list masih kosong, inisialisasi head dan last
        {
            head = baru;
            last = baru;
        }
        else
        {
            last->next = baru; // Tambahkan node baru di akhir list
            last = baru;
        }
    }

    fclose(file); // Tutup file setelah selesai dibaca
}

void dropOfTambahLaundryMasuk() // Fungsi untuk menambahkan data laundry Drop-Off baru ke linked list dan menyimpannya ke file
{
    loadDataFromFile(); // Memuat data lama dari file ke linked list (jika ada)

    dropOfLaundry *baru = new dropOfLaundry; // Alokasi memori untuk node baru
    baru->next = NULL;

    int lastID = hitungIDTerbesar(); // Menentukan ID baru berdasarkan ID terbesar yang pernah ada
    baru->id = lastID + 1;

    cin.ignore();
    cout << "Masukkan Nama: "; // Input data pelanggan dari pengguna
    cin.getline(baru->nama, 50);

    cout << "Masukkan No Telp: ";
    cin.getline(baru->noTelp, 15);

    cout << "Masukkan Tanggal (dd-mm-yyyy): ";
    cin.getline(baru->tanggal, 20);

    cout << "Masukkan Layanan (reguler/express): ";
    cin.getline(baru->layanan, 10);

    cout << "Masukkan Berat (kg): ";
    cin >> baru->berat;

    baru->harga = hitungHarga(baru->layanan, baru->berat); // Hitung harga berdasarkan layanan dan berat

    if (head == NULL)
    {
        head = baru;
    }
    else
    {
        dropOfLaundry *temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = baru;
    }

    cout << "\nLaundry berhasil ditambahkan.\n";

    FILE *file = fopen("dropOfDataLaundry.dat", "wb"); // Simpan seluruh linked list ke file (overwrite)
    if (file == NULL)
    {
        cout << "Gagal membuka file untuk menulis.\n";
        return;
    }

    dropOfLaundry *curr = head;
    while (curr != NULL)
    {
        fwrite(curr, sizeof(dropOfLaundry), 1, file);
        curr = curr->next;
    }

    fclose(file);

    cout << "Data laundry berhasil disimpan ke file.\n";
}

void dropOfTampilLaundryMasuk()
{
    FILE *file = fopen("dropOfDataLaundry.dat", "rb");
    if (file == NULL)
    {
        cout << "File dropOfDataLaundry.dat tidak ditemukan atau kosong.\n";
        return;
    }

    dropOfLaundry temp;

    cout << "+-----+--------------------+---------------+------------+----------+--------+----------+\n";
    cout << "| ID  | Nama               | No Telp       | Tanggal    | Layanan  | Berat  | Harga    |\n";
    cout << "+-----+--------------------+---------------+------------+----------+--------+----------+\n";

    while (fread(&temp, sizeof(dropOfLaundry), 1, file)) // Baca dan tampilkan semua data laundry dari file
    {
        cout << "| " << setw(3) << right << temp.id << " "
             << "| " << setw(19) << left << temp.nama
             << "| " << setw(14) << left << temp.noTelp
             << "| " << setw(11) << left << temp.tanggal
             << "| " << setw(9) << left << temp.layanan
             << "| " << setw(6) << right << fixed << setprecision(2) << temp.berat
             << " | " << setw(8) << right << temp.harga << " |\n";
    }

    cout << "+-----+--------------------+---------------+------------+----------+--------+----------+\n";
    fclose(file);
}

void dropOfAmbilLaundry()
{
    loadDataFromFile(); // Muat data laundry dari file ke linked list
    int idCari;
    cout << "Masukkan ID laundry yang ingin diambil: ";
    cin >> idCari;

    dropOfLaundry *curr = head, *prev = NULL;
    bool found = false;

    while (curr != NULL) // Cari laundry berdasarkan ID yang dimasukkan
    {
        if (curr->id == idCari)
        {
            found = true;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found)
    {
        cout << "Data dengan ID " << idCari << " tidak ditemukan.\n";
        return;
    }

    if (prev == NULL) // Hapus node laundry dari linked list
        head = curr->next;
    else
        prev->next = curr->next;

    FILE *file = fopen("dropOfDataLaundry.dat", "wb"); // Simpan linked list yang sudah dihapus node tadi kembali ke file (overwrite)
    dropOfLaundry *temp = head;
    while (temp != NULL)
    {
        fwrite(temp, sizeof(dropOfLaundry), 1, file);
        temp = temp->next;
    }
    fclose(file);

    FILE *riwayat = fopen("dropOfDataLaundryRiwayat.dat", "ab"); // Tambahkan data laundry yang diambil ke file riwayat (append)
    fwrite(curr, sizeof(dropOfLaundry), 1, riwayat);
    fclose(riwayat);

    cout << "Laundry ID " << idCari << " telah diambil dan dipindahkan ke riwayat.\n";
    delete curr; // Hapus node dari memori
}

void dropOfTampilRiwayat(bool urutHarga)
{
    FILE *file = fopen("dropOfDataLaundryRiwayat.dat", "rb");
    if (!file)
    {
        cout << "Riwayat masih kosong.\n";
        return;
    }

    dropOfLaundry *riwayat = NULL;

    dropOfLaundry temp;
    while (fread(&temp, sizeof(dropOfLaundry), 1, file)) // Muat semua data riwayat ke linked list sementara
    {
        dropOfLaundry *baru = new dropOfLaundry;
        *baru = temp;
        baru->next = NULL;

        if (riwayat == NULL)
        {
            riwayat = baru;
        }
        else
        {
            dropOfLaundry *last = riwayat;
            while (last->next != NULL)
                last = last->next;
            last->next = baru;
        }
    }
    fclose(file);

    if (urutHarga) // Jika diminta, urutkan linked list berdasarkan harga secara ascending (bubble sort)
    {
        for (dropOfLaundry *i = riwayat; i != NULL; i = i->next)
        {
            for (dropOfLaundry *j = i->next; j != NULL; j = j->next)
            {
                if (i->harga > j->harga)
                {
                    swap(i->id, j->id);
                    swap(i->nama, j->nama);
                    swap(i->noTelp, j->noTelp);
                    swap(i->tanggal, j->tanggal);
                    swap(i->layanan, j->layanan);
                    swap(i->berat, j->berat);
                    swap(i->harga, j->harga);
                }
            }
        }
    }

    int totalPendapatan = 0;
    cout << "+-----+--------------------+---------------+------------+----------+--------+----------+\n";
    cout << "| ID  | Nama               | No Telp       | Tanggal    | Layanan  | Berat  | Harga    |\n";
    cout << "+-----+--------------------+---------------+------------+----------+--------+----------+\n";

    dropOfLaundry *curr = riwayat;
    while (curr)
    {
        cout << "| " << setw(3) << right << curr->id << " "
             << "| " << setw(19) << left << curr->nama
             << "| " << setw(14) << left << curr->noTelp
             << "| " << setw(11) << left << curr->tanggal
             << "| " << setw(9) << left << curr->layanan
             << "| " << setw(6) << right << fixed << setprecision(2) << curr->berat
             << " | " << setw(8) << right << curr->harga << " |\n";
        totalPendapatan += curr->harga;
        curr = curr->next;
    }

    cout << "+-----+--------------------+---------------+------------+----------+--------+----------+\n";
    cout << "Total Pendapatan: Rp" << totalPendapatan << endl;

    while (riwayat) // Bebaskan memori linked list sementara
    {
        dropOfLaundry *hapus = riwayat;
        riwayat = riwayat->next;
        delete hapus;
    }
}

void dropOfCariNotaLaundryByID()
{
    int idCari;
    cout << "Masukkan ID laundry yang ingin dicari: ";
    cin >> idCari;

    FILE *file = fopen("dropOfDataLaundry.dat", "rb");
    if (!file)
    {
        cout << "File dropOfDataLaundry.dat tidak ditemukan atau kosong.\n";
        return;
    }

    dropOfLaundry temp;
    bool found = false;

    while (fread(&temp, sizeof(dropOfLaundry), 1, file)) // Baca file record per record dan cari yang ID-nya sama dengan input
    {
        if (temp.id == idCari)
        {
            found = true;

            cout << "============================================\n";
            cout << "            Nota Laundry ID: " << temp.id << endl;
            cout << "--------------------------------------------\n";
            cout << "Nama    : " << temp.nama << endl;
            cout << "No Telp : " << temp.noTelp << endl;
            cout << "Tanggal : " << temp.tanggal << endl;
            cout << "Layanan : " << temp.layanan << endl;
            cout << "Berat   : " << fixed << setprecision(2) << temp.berat << " kg\n";
            cout << "Harga   : Rp" << temp.harga << endl;
            cout << "============================================\n";

            break;
        }
    }

    if (!found)
    {
        cout << "Data dengan ID " << idCari << " tidak ditemukan.\n";
    }

    fclose(file);
}

void dropOfHapusRiwayatByID()
{
    int idHapus;
    cout << "Masukkan ID laundry riwayat yang ingin dihapus: ";
    cin >> idHapus;

    FILE *file = fopen("dropOfDataLaundryRiwayat.dat", "rb");
    if (!file)
    {
        cout << "File riwayat kosong atau tidak ditemukan.\n";
        return;
    }

    dropOfLaundry *headRiw = NULL, *last = NULL;
    dropOfLaundry temp;

    while (fread(&temp, sizeof(dropOfLaundry), 1, file)) // Muat semua data riwayat ke linked list
    {
        dropOfLaundry *baru = new dropOfLaundry;
        *baru = temp;
        baru->next = NULL;

        if (headRiw == NULL)
        {
            headRiw = baru;
            last = baru;
        }
        else
        {
            last->next = baru;
            last = baru;
        }
    }
    fclose(file);

    dropOfLaundry *curr = headRiw; // Cari node dengan ID yang akan dihapus
    dropOfLaundry *prev = NULL;
    bool found = false;

    while (curr != NULL)
    {
        if (curr->id == idHapus)
        {
            found = true;
            if (prev == NULL) // Hapus node dari linked list
                headRiw = curr->next;
            else
                prev->next = curr->next;

            delete curr;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found)
    {
        cout << "Data dengan ID " << idHapus << " tidak ditemukan di riwayat.\n";

        while (headRiw != NULL) // Bersihkan memori linked list
        {
            dropOfLaundry *hapus = headRiw;
            headRiw = headRiw->next;
            delete hapus;
        }
        return;
    }

    file = fopen("dropOfDataLaundryRiwayat.dat", "wb"); // Tulis ulang file riwayat dengan data linked list yang sudah dihapus node tertentu
    if (!file)
    {
        cout << "Gagal membuka file riwayat untuk menulis ulang.\n";

        while (headRiw != NULL) // Bersihkan memori linked list
        {
            dropOfLaundry *hapus = headRiw;
            headRiw = headRiw->next;
            delete hapus;
        }
        return;
    }

    curr = headRiw;
    while (curr != NULL)
    {
        fwrite(curr, sizeof(dropOfLaundry), 1, file);
        curr = curr->next;
    }
    fclose(file);

    while (headRiw != NULL)
    {
        dropOfLaundry *hapus = headRiw;
        headRiw = headRiw->next;
        delete hapus;
    }

    cout << "Data riwayat dengan ID " << idHapus << " berhasil dihapus.\n";
}

//========================================================================================================//

struct selfServiceLaundry
{
    int id;
    char nama[50];
    char noTelp[15];
    char tanggal[20];
    float berat;
    int jumlahMesin;
    int harga;
    selfServiceLaundry *next;
};

selfServiceLaundry *selfHead = NULL;

// Hitung jumlah mesin yang dibutuhkan berdasarkan berat laundry,
// setiap mesin maksimal 7 kg, jika berat > 7 maka mesin bertambah.
int hitungJumlahMesin(float berat)
{
    return (int)(berat / 7.0 + 0.999);
}

// Hitung harga Self-Service berdasarkan jumlah mesin yang digunakan.
// Tarif Rp 20.000 per mesin.
int hitungHargaSelf(int jumlahMesin)
{
    return jumlahMesin * 20000;
}

// Cari ID terbesar dari file riwayat Self-Service Laundry,
// digunakan untuk generate ID baru secara incremental.
int hitungIDTerbesarSelfService()
{
    int lastID = 0;
    selfServiceLaundry temp;

    FILE *fileRiw = fopen("selfServiceLaundryRiwayat.dat", "rb");
    if (fileRiw)
    {
        while (fread(&temp, sizeof(selfServiceLaundry), 1, fileRiw))
            if (temp.id > lastID)
                lastID = temp.id;
        fclose(fileRiw);
    }

    return lastID;
}

void selfServiceTambahLaundry()
{
    selfServiceLaundry *baru = new selfServiceLaundry; // buat node baru
    baru->next = NULL;

    static int idSelf = hitungIDTerbesarSelfService() + 1; // inisialisasi ID unik
    baru->id = idSelf++;

    cin.ignore();
    cout << "Masukkan Nama: ";
    cin.getline(baru->nama, 50);
    cout << "Masukkan No Telp: ";
    cin.getline(baru->noTelp, 15);
    cout << "Masukkan Tanggal (dd-mm-yyyy): ";
    cin.getline(baru->tanggal, 20);
    cout << "Masukkan Berat Cucian (kg): ";
    cin >> baru->berat;

    baru->jumlahMesin = hitungJumlahMesin(baru->berat);
    baru->harga = hitungHargaSelf(baru->jumlahMesin);

    if (selfHead == NULL) // tambahkan node ke akhir linked list
        selfHead = baru;
    else
    {
        selfServiceLaundry *temp = selfHead;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = baru;
    }

    cout << "\nData self-service laundry berhasil ditambahkan.\n";
}

void selfServiceCetakNota()
{
    if (selfHead == NULL)
    {
        cout << "Tidak ada data laundry untuk dicetak.\n";
        return;
    }

    selfServiceLaundry *hapus = selfHead; // ambil node depan
    selfHead = selfHead->next;            // hapus dari linked list

    cout << "\n===================================================\n";
    cout << "            Nota Self-Service Laundry\n";
    cout << "---------------------------------------------------\n";
    cout << "ID        : " << hapus->id << endl;
    cout << "Nama      : " << hapus->nama << endl;
    cout << "No Telp   : " << hapus->noTelp << endl;
    cout << "Tanggal   : " << hapus->tanggal << endl;
    cout << "Berat     : " << fixed << setprecision(2) << hapus->berat << " kg\n";
    cout << "Mesin     : " << hapus->jumlahMesin << " mesin\n";
    cout << "Harga     : Rp" << hapus->harga << endl;
    cout << "===================================================\n";

    FILE *file = fopen("selfServiceLaundryRiwayat.dat", "ab"); // simpan data ke file riwayat (append binary)
    if (file)
    {
        fwrite(hapus, sizeof(selfServiceLaundry), 1, file);
        fclose(file);
    }

    delete hapus; // hapus node dari memory
}

void selfServiceTampilRiwayat(bool urutHarga)
{
    FILE *file = fopen("selfServiceLaundryRiwayat.dat", "rb"); // buka file riwayat baca
    if (!file)
    {
        cout << "Riwayat self-service masih kosong.\n";
        return;
    }

    selfServiceLaundry *riwayat = NULL; // linked list untuk simpan data riwayat sementara

    selfServiceLaundry temp;
    while (fread(&temp, sizeof(selfServiceLaundry), 1, file)) // baca data file per record
    {
        selfServiceLaundry *baru = new selfServiceLaundry; // buat node baru
        *baru = temp;                                      // salin data
        baru->next = NULL;

        if (riwayat == NULL) // tambahkan node ke akhir linked list
            riwayat = baru;
        else
        {
            selfServiceLaundry *last = riwayat;
            while (last->next != NULL)
                last = last->next;
            last->next = baru;
        }
    }
    fclose(file);

    if (urutHarga) // jika pilih urut berdasarkan harga
    {
        for (selfServiceLaundry *i = riwayat; i != NULL; i = i->next)
        {
            for (selfServiceLaundry *j = i->next; j != NULL; j = j->next)
            {
                if (i->harga > j->harga)
                {
                    swap(i->id, j->id);
                    swap(i->nama, j->nama);
                    swap(i->noTelp, j->noTelp);
                    swap(i->tanggal, j->tanggal);
                    swap(i->berat, j->berat);
                    swap(i->jumlahMesin, j->jumlahMesin);
                    swap(i->harga, j->harga);
                }
            }
        }
    }

    int totalPendapatan = 0;
    cout << "+-----+--------------------+---------------+------------+--------+--------------+----------+\n";
    cout << "| ID  | Nama               | No Telp       | Tanggal    | Berat  | Jumlah Mesin | Harga    |\n";
    cout << "+-----+--------------------+---------------+------------+--------+--------------+----------+\n";

    selfServiceLaundry *curr = riwayat;
    while (curr)
    {
        cout << "| " << setw(3) << right << curr->id << " "
             << "| " << setw(19) << left << curr->nama
             << "| " << setw(14) << left << curr->noTelp
             << "| " << setw(11) << left << curr->tanggal
             << "| " << setw(6) << right << fixed << setprecision(2) << curr->berat
             << " | " << setw(12) << right << curr->jumlahMesin
             << " | " << setw(8) << right << curr->harga << " |\n";

        totalPendapatan += curr->harga; // hitung total pendapatan
        curr = curr->next;
    }

    cout << "+-----+--------------------+---------------+------------+--------+--------------+----------+\n";
    cout << "Total Pendapatan: Rp" << totalPendapatan << endl;

    while (riwayat) // hapus linked list agar tidak memory leak
    {
        selfServiceLaundry *hapus = riwayat;
        riwayat = riwayat->next;
        delete hapus;
    }
}

void selfServiceHapusRiwayatByID()
{
    int idHapus;
    cout << "Masukkan ID riwayat yang ingin dihapus: ";
    cin >> idHapus;

    FILE *file = fopen("selfServiceLaundryRiwayat.dat", "rb");
    if (!file)
    {
        cout << "Riwayat self-service masih kosong atau file tidak ditemukan.\n";
        return;
    }

    selfServiceLaundry *head = NULL, *tail = NULL; // baca semua data dari file ke linked list
    selfServiceLaundry temp;
    while (fread(&temp, sizeof(selfServiceLaundry), 1, file))
    {
        selfServiceLaundry *baru = new selfServiceLaundry;
        *baru = temp;
        baru->next = NULL;

        if (head == NULL)
            head = tail = baru;
        else
        {
            tail->next = baru;
            tail = baru;
        }
    }
    fclose(file);

    selfServiceLaundry *curr = head; // cari node dengan ID yang ingin dihapus
    selfServiceLaundry *prev = NULL;
    bool found = false;

    while (curr != NULL)
    {
        if (curr->id == idHapus)
        {
            found = true;
            if (prev == NULL) // hapus head
                head = curr->next;
            else // hapus node selain head
                prev->next = curr->next;

            delete curr; // free memory node yang dihapus
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if (!found)
    {
        cout << "Data dengan ID " << idHapus << " tidak ditemukan di riwayat.\n";

        while (head) // bersihkan linked list sebelum return
        {
            selfServiceLaundry *hapus = head;
            head = head->next;
            delete hapus;
        }
        return;
    }

    file = fopen("selfServiceLaundryRiwayat.dat", "wb"); // tulis ulang file dengan data yang sudah dihapus node tertentu
    if (!file)
    {
        cout << "Gagal membuka file untuk penulisan ulang.\n";

        while (head) // bersihkan linked list sebelum return
        {
            selfServiceLaundry *hapus = head;
            head = head->next;
            delete hapus;
        }
        return;
    }

    curr = head;
    while (curr)
    {
        fwrite(curr, sizeof(selfServiceLaundry), 1, file);
        curr = curr->next;
    }
    fclose(file);

    while (head) // hapus linked list dari memory
    {
        selfServiceLaundry *hapus = head;
        head = head->next;
        delete hapus;
    }

    cout << "Data riwayat dengan ID " << idHapus << " berhasil dihapus.\n";
}

int main()
{
    int pilih, subPilih;
    do
    {
        cout << "==== Menu Laundry Sistem ====\n\n";
        cout << "1. Drop-Off Laundry\n";
        cout << "2. Self-Service Laundry\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;

        system("cls");
        switch (pilih)
        {
        case 1:
            do
            {
                system("cls");
                cout << "\n==== Drop-Off Laundry ====\n";
                cout << "1. Tambah Laundry Masuk\n";
                cout << "2. Tampilkan Laundry [Masuk]\n";
                cout << "3. Ambil Laundry (Selesai)\n";
                cout << "4. Tampilkan Laundry [Riwayat]\n";
                cout << "5. Nota Laundry [Masuk] Berdasarkan ID\n";
                cout << "6. Hapus Laundry [Riwayat] Berdasarkan ID\n";
                cout << "0. Kembali\n";
                cout << "Pilih: ";
                cin >> subPilih;

                switch (subPilih)
                {
                case 1:
                    system("cls");
                    dropOfTambahLaundryMasuk();
                    system("pause");
                    break;
                case 2:
                    system("cls");
                    dropOfTampilLaundryMasuk();
                    system("pause");
                    break;
                case 3:
                    system("cls");
                    dropOfAmbilLaundry();
                    system("pause");
                    break;
                case 4:
                {
                    int subSubPilih;
                    do
                    {
                        system("cls");
                        cout << "==== Drop-Off Laundry - Tampilkan Laundry [Riwayat] ====\n";
                        cout << "1. Tampilkan Riwayat Laundry Natural\n";
                        cout << "2. Tampilkan Riwayat Laundry Terurut Berdasarkan Harga\n";
                        cout << "0. Kembali\n";
                        cout << "Pilih: ";
                        cin >> subSubPilih;

                        switch (subSubPilih)
                        {
                        case 1:
                            system("cls");
                            cout << "Menampilkan data tanpa urut...\n";
                            dropOfTampilRiwayat(false);
                            system("pause");
                            break;
                        case 2:
                            system("cls");
                            cout << "Menampilkan data terurut...\n";
                            dropOfTampilRiwayat(true);
                            system("pause");
                            break;
                        case 0:
                            cout << "Kembali ke menu sebelumnya...\n";
                            system("pause");
                            break;
                        default:
                            cout << "Pilihan tidak valid!\n";
                            system("pause");
                            break;
                        }

                    } while (subSubPilih != 0);
                }
                break;
                case 5:
                    system("cls");
                    dropOfCariNotaLaundryByID();
                    system("pause");
                    break;
                case 6:
                    system("cls");
                    dropOfHapusRiwayatByID();
                    system("pause");
                case 0:
                    cout << "Kembali ke menu utama...\n";
                    system("pause");
                    system("cls");
                    break;
                default:
                    cout << "Pilihan tidak valid!\n";
                    system("pause");
                    break;
                }
            } while (subPilih != 0);
            break;

        case 2:
            do
            {
                system("cls");
                cout << "\n==== Self-Service Laundry ====\n";
                cout << "1. Tambah Laundry [Masuk]\n";
                cout << "2. Cetak Nota Laundry\n";
                cout << "3. Tampilkan Laundry [Riwayat]\n";
                cout << "4. Hapus Laundry [Riwayat] Berdasarkan ID\n";
                cout << "0. Kembali\n";
                cout << "Pilih: ";
                cin >> subPilih;

                switch (subPilih)
                {
                case 1:
                    system("cls");
                    selfServiceTambahLaundry();
                    system("pause");
                    break;
                case 2:
                    system("cls");
                    selfServiceCetakNota();
                    system("pause");
                    break;
                case 3:
                {
                    int subSubPilih;
                    do
                    {
                        system("cls");
                        cout << "==== Self-Service Laundry - Tampilkan Laundry [Riwayat] ====\n";
                        cout << "1. Tampilkan Riwayat Laundry Natural\n";
                        cout << "2. Tampilkan Riwayat Laundry Terurut Berdasarkan Harga\n";
                        cout << "0. Kembali\n";
                        cout << "Pilih: ";
                        cin >> subSubPilih;

                        switch (subSubPilih)
                        {
                        case 1:
                            system("cls");
                            cout << "Menampilkan data tanpa urut...\n";
                            selfServiceTampilRiwayat(false);
                            system("pause");
                            break;
                        case 2:
                            system("cls");
                            cout << "Menampilkan data terurut...\n";
                            selfServiceTampilRiwayat(true);
                            system("pause");
                            break;
                        case 0:
                            cout << "Kembali ke menu sebelumnya...\n";
                            system("pause");
                            break;
                        default:
                            cout << "Pilihan tidak valid!\n";
                            system("pause");
                            break;
                        }

                    } while (subSubPilih != 0);
                }
                break;
                case 4:
                    system("cls");
                    selfServiceHapusRiwayatByID();
                    system("pause");
                    break;
                case 0:
                    cout << "Kembali ke menu utama...\n";
                    system("cls");
                    break;
                default:
                    cout << "Pilihan tidak valid!\n";
                    system("pause");
                    break;
                }

            } while (subPilih != 0);
            break;

        case 0:
            cout << "Terima kasih telah menggunakan sistem laundry. Semoga sehat selalu :)\n";
            break;

        default:
            cout << "Pilihan tidak valid!\n";
            system("pause");
            break;
        }

    } while (pilih != 0);

    return 0;
}
