#include <iostream>
using namespace std;

struct KaryawanNode {
    int nip;
    string nama;
    string divisi;
    string status;
    KaryawanNode* left;
    KaryawanNode* right;
};

struct StackNode {
    int nip;
    string aksi;
    StackNode* next;
};

struct QueueNode {
    int nip;
    QueueNode* next;
};

KaryawanNode* root = nullptr;
StackNode* top = nullptr;
QueueNode* frontQ = nullptr;
QueueNode* rearQ = nullptr;

KaryawanNode* buatKaryawan(int nip, string nama, string divisi) {
    KaryawanNode* baru = new KaryawanNode;
    baru->nip = nip;
    baru->nama = nama;
    baru->divisi = divisi;
    baru->status = "Tidak Cuti";
    baru->left = nullptr;
    baru->right = nullptr;
    return baru;
}

KaryawanNode* insertKaryawan(KaryawanNode* node, int nip, string nama, string divisi) {
    if (node == nullptr) {
        cout << "Data karyawan berhasil ditambahkan\n";
        return buatKaryawan(nip, nama, divisi);
    }
    if (nip < node->nip) {
        node->left = insertKaryawan(node->left, nip, nama, divisi);
    } else if (nip > node->nip) {
        node->right = insertKaryawan(node->right, nip, nama, divisi);
    } else {
        cout << "Error: Karyawan dengan NIP tersebut sudah ada.\n";
    }
    return node;
}

KaryawanNode* cariKaryawan(KaryawanNode* node, int nip) {
    if (node == nullptr || node->nip == nip) {
        return node;
    }
    if (nip < node->nip) {
        return cariKaryawan(node->left, nip);
    } else {
        return cariKaryawan(node->right, nip);
    }
}

void tampilKaryawan(KaryawanNode* node) {
    if (node != nullptr) {
        tampilKaryawan(node->left);
        cout << "NIP: " << node->nip
             << ", Nama: " << node->nama
             << ", Divisi: " << node->divisi
             << ", Status: " << node->status << endl;
        tampilKaryawan(node->right);
    }
}

KaryawanNode* cariMin(KaryawanNode* node) {
    KaryawanNode* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}

KaryawanNode* deleteKaryawan(KaryawanNode* root, int nip) {
    if (root == nullptr) {
        cout << "Karyawan tidak ditemukan!\n";
        return root;
    }
    if (nip < root->nip) {
        root->left = deleteKaryawan(root->left, nip);
    } else if (nip > root->nip) {
        root->right = deleteKaryawan(root->right, nip);
    } else {
        if (root->left == nullptr) {
            KaryawanNode* temp = root->right;
            delete root;
            cout << "Data karyawan berhasil dihapus\n";
            return temp;
        } else if (root->right == nullptr) {
            KaryawanNode* temp = root->left;
            delete root;
            cout << "Data karyawan berhasil dihapus\n";
            return temp;
        }
        KaryawanNode* temp = cariMin(root->right);
        root->nip = temp->nip;
        root->nama = temp->nama;
        root->divisi = temp->divisi;
        root->status = temp->status;
        root->right = deleteKaryawan(root->right, temp->nip);
    }
    return root;
}

void push(int nip, string aksi) {
    StackNode* baru = new StackNode;
    baru->nip = nip;
    baru->aksi = aksi;

    baru->next = top;
    top = baru;
}

void undoAksi() {
    if (top == nullptr) {
        cout << "Tidak ada aksi untuk di Undo!\n";
        return;
    }
    StackNode* hapus = top;
    KaryawanNode* k = cariKaryawan(root, top->nip);
    if (k != nullptr) {
        if (top->aksi == "pengajuan") {
            k->status = "Tidak Cuti";
            cout << "Aksi pengajuan cuti berhasil di Undo\n";
        } else if (top->aksi == "selesai") {
            k->status = "Cuti";
            cout << "Aksi selesai cuti berhasil di Undo\n";
        }
    }
    top = top->next;
    delete hapus;
}

void enqueue(int nip) {
    QueueNode* baru = new QueueNode;
    baru->nip = nip;
    baru->next = nullptr;

    if (rearQ == nullptr) {
        frontQ = rearQ = baru;
    } else {
        rearQ->next = baru;
        rearQ = baru;
    }
}

int dequeue() {

    if (frontQ == nullptr) {
        cout << "Antrian kosong!\n";
        return -1; 
    }
    QueueNode* hapus = frontQ;
    int nip = hapus->nip;
    frontQ = frontQ->next;
    if (frontQ == nullptr) {
        rearQ = nullptr;
    }
    delete hapus;
    return nip;
}

void removeFromQueue(int nip) {
    if (frontQ == nullptr) return;

    QueueNode* curr = frontQ;
    QueueNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->nip == nip) {
            QueueNode* toDelete = curr;

            if (prev == nullptr) {
                frontQ = frontQ->next;
            } else {
                prev->next = curr->next;
            }

            if (curr == rearQ) {
                rearQ = prev;
            }

            delete toDelete;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}


void tampilAntrian() {
    if (frontQ == nullptr) {
        cout << "Antrian kosong!\n";
        return;
    }
    QueueNode* bantu = frontQ;
    while (bantu != nullptr) {
        KaryawanNode* k = cariKaryawan(root, bantu->nip);
        if (k != nullptr) {
            cout << k->nama << " menunggu giliran cuti\n";
        } 
        bantu = bantu->next;
    }
}

void pengajuanCuti(int nip) {
    KaryawanNode* krywn = cariKaryawan(root, nip);
    if (krywn == nullptr) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }

    push(nip, "pengajuan");

    if (krywn->status == "Tidak Cuti") {
        krywn->status = "Cuti";
        cout << "Pengajuan cuti berhasil\n";
    } else {
        enqueue(nip);
        cout << "Karyawan sedang cuti, pengajuan masuk ke dalam antrian\n";
    }
}

void selesaikanCuti(int nip) {
    KaryawanNode* krywn = cariKaryawan(root, nip);
    if (krywn == nullptr) {
        cout << "Karyawan tidak ditemukan!\n";
        return;
    }

    push(nip, "selesai");

    cout << "Cuti berhasil diselesaikan\n";

    if (frontQ != nullptr) {
        int nextNIP = dequeue();
        KaryawanNode* nextK = cariKaryawan(root, nextNIP);
        if (nextK != nullptr) {
            nextK->status = "Cuti";
            cout << "Pengajuan cuti berikutnya langsung diproses dari antrian\n";
        }
    }
}

int main(){
    int pilihan;
    do {
        cout << "\n--- Sistem Manajemen Cuti Karyawan ---\n";
        cout << "1. Tambah Karyawan\n"; 
        cout << "2. Tampil Karyawan\n";
        cout << "3. Pengajuan Cuti\n";
        cout << "4. Selesai Cuti\n";
        cout << "5. Hapus Karyawan\n";
        cout << "6. Undo Aksi Terakhir\n";
        cout << "7. Tampilkan Antrian Pengajuan Cuti\n";
        cout << "8. Keluar\n";
        cout << "Pilih opsi: ";
        cin >> pilihan;

        if (pilihan == 1) {
            int nip;
            string nama, divisi;

            cout << "NIP    : ";
            cin >> nip;

            cin.ignore();

            cout << "Nama   : ";
            getline(cin, nama);

            cout << "Divisi : ";
            getline(cin, divisi);

            root = insertKaryawan(root, nip, nama, divisi);
        } else if (pilihan == 2) {
            cout << "\n--- Data Karyawan ---\n";
            tampilKaryawan(root);
        } else if (pilihan == 3) {
            int nip;

            cout << "Masukkan NIP karyawan yang mengajukan cuti: ";
            cin >> nip;

            pengajuanCuti(nip);
        } else if (pilihan == 4) {
            int nip;

            cout << "Masukkan NIP karyawan yang selesai cuti: ";
            cin >> nip;

            selesaikanCuti(nip);
        } else if (pilihan == 5) {
            int nip;

            cout << "Masukkan NIP karyawan yang akan dihapus: ";
            cin >> nip;

            root = deleteKaryawan(root, nip);
        } else if (pilihan == 6) {
            undoAksi();
        } else if (pilihan == 7) {
            tampilAntrian();
        } else if (pilihan == 8) {
            cout << "Program selesai\n";
        } else {
            cout << "Pilihan tidak valid! Silakan coba lagi.\n";
        }
    } while (pilihan != 8);
    return 0;
}