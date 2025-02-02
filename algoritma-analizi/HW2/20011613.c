/*
@brief k-way divide and conquer algoritmas� implementasyonu.
Bu algoritma, verilen bir diziyi k par�aya b�lerek s�ralama i�lemi yapar.
B�l�nen par�alar kendi i�lerinde s�ralan�r (mergsort gibi) ve sonra k par�a birle�tirilir.
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/*
@brief Verilen N boyutlar�na g�re elemanlar� unique olmak �zere bir dizi olu�turulur.
@param arr olu�turulacak dizi
@param N   dizinin boyutu
@return void
*/
void generateUniqueArray(int *arr, int N) {
    int i;
    // 1'den N'e kadar olan say�lar� yerle�tir diziye.
    for (i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

	// Knuth Shuffle algoritmas� ile basit�e diziyi kar��t�r.
    for (i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

/*
@brief K adeti kadar par�alad���m�z par�alar�n s�ralanm�� halini birle�tirme.
@param arr    ana dizi
@param temp   ge�ici dizi
@param start  ilk indeks
@param end    son indeks
@param k      par�a say�s�
@return void
*/
void mergePartsK(int *arr, int *temp, int start, int end, int k) {
    if (k <= 0 || start >= end) return;

    // par�a boyutu ve kalan hesaplanmas�
    int total_elements = end - start + 1;
    int base_part_size = total_elements / k;
    int remainder = total_elements % k;

    // indeks ve limit dizileri i�in bellek alan� verilmesi
    int *indices = (int *)malloc(k * sizeof(int));
    int *limits = (int *)malloc(k * sizeof(int));
    
    // malloc problemi ��karsa diye var
    if (!indices || !limits) {
        printf("Memory allocation failed\n");
        exit(1);
    }
	
    // her par�an�n ilk ve son indeksi belirleme
    int i;
	int curr_pos = start;
    for  (i = 0; i < k; i++) {
        indices[i] = curr_pos;
        int part_size = base_part_size + (i < remainder ? 1 : 0);
        curr_pos += part_size;
        limits[i] = curr_pos;
    }

    /* 
	@brief her ad�mda k par�adan en k���k eleman� se� ve temp dizisine ekle.
	bu �ekilde olu�an temp dizi, ilk verilen dizinin bizden istenildi�i gibi sortlanm�� haline b�r�n�yor.
	k tane par�ay� ayn� anda birle�tirmek i�in kulland�m.
	*/
	int temp_pos = start;
    while (temp_pos <= end) {
        int min_val = INT_MAX;
        int min_idx = -1;

        for (i = 0; i < k; i++) {
        	// e�er �u anki par�ada, eleman kald�ysa 
			// VE kalan eleman �u ana kadar bulunan min'den k���kse
			// Yeni min de�eri ve onun bulundu�u par�alar� indeksini g�ncelle
            if (indices[i] < limits[i] && (min_idx == -1 || arr[indices[i]] < min_val)) {
                min_val = arr[indices[i]];
                min_idx = i;
            }
        }
		
		// sortlanacak par�a kalmad�ysa �IK
        if (min_idx == -1) break;
        
		//  min de�eri temp dizisine ekle ve bir sonraki aramada bu par�adaki s�radaki elemana bak�lmas� i�in indeks konumunu ileri ta��
        temp[temp_pos++] = min_val;
        indices[min_idx]++;
    }

    // sonu�lar� ana diziye kopyalama
    for (i = start; i <= end; i++) {
        arr[i] = temp[i];
    }

    free(indices);
    free(limits);
}

/*
@brief 
@param arr	  ana dizi (bunu s�ralicaz)
@param temp	  ge�ici dizi	
@param start  ilk indeks
@param end    son indeks
@param k      b�l�nd��� par�a say�s�
@return void
*/
void kWayMergeSort(int *arr, int *temp, int start, int end, int k) {
    if (start >= end) return;
    
    // toplam eleman say�s�n� VE kalan elemanlar� hesapla
    int total_elements = end - start + 1;
    // par�an�n toplam eleman say�s� < k ise,, k = toplam eleman say�s� yapar, 
    // bunun sayesinde k par�aya b�lme i�lem eleman say�s�na uygun olur.
    if (total_elements <= k) k = total_elements / 2;
    int base_part_size = total_elements / k;
    int remainder = total_elements % k;
	int i, j;
	
    // b�l�nm�� par�alar� birer birer s�rala
    int curr_start = start;
    for (i = 0; i < k; i++) {
        int part_size = base_part_size + (i < remainder ? 1 : 0);
        int curr_end = curr_start + part_size - 1;
        
        // par�alar�n eleman say�s� k'den d���k ise insertion sort kullan.
        // k���k dizilerde daha h�zl�d�r ve daha iyi performans sa�lar.
        if (curr_start < curr_end) {			// d�zg�n  par�alanm�� m�
            if (curr_end - curr_start < k) {
                //insertion sort
				for (j = curr_start + 1; j <= curr_end; j++) {
                    int key = arr[j];
                    int l = j - 1;
                    while (l >= curr_start && arr[l] > key) {
                        arr[l + 1] = arr[l];
                        l--;
                    }
                    arr[l + 1] = key;
                }
            } else { 
				// e�er par�alar�n eleman say�s� 'k' veya daha b�y�kse buraya d��
				// k-way mergeSort'u rek�rsif olarak �a��rarak bu alt par�ay� daha k���k par�alara b�l ve s�rala. 
                kWayMergeSort(arr, temp, curr_start, curr_end, k);
            }
        }
        curr_start = curr_end + 1;
    }

    // s�ralanm�� par�alar� birle�tirme algoritmas�n� �a��r
    mergePartsK(arr, temp, start, end, k);
}

int main() {
	// rastgele say� �retimi i�in ayar
    srand((unsigned int)time(0));

    // �zel test dizi ile kontrol� fonksiyonu ile ilk testi yap
    // testExample();

    printf("\nPerformans testleri basliyor...\n");

    // bize verilen N de�erlerini tan�mlama
    int N_values[] = { 100, 1000, 10000, 100000, 1000000, 10000000};
    int num_N = sizeof(N_values) / sizeof(N_values[0]);
    int i, k, j;
    
	// her bir N de�eri i�in s�ralama algoritmas� test edilecek bu loopin i�inde
    for (i = 0; i < num_N; i++) {
        int N = N_values[i];
        int *arr = (int *)malloc(N * sizeof(int));
        // Kontrol ama�l�, continue bo�una d�nmesin diye kullan�ld�.
		if (!arr) {
            printf("Memory allocation failed for N = %d\n", N);
            continue;
        }
		
		// rastgele unique elemanlardan olu�an bir dizi olu�turma fonksiyonunu �a��r
        generateUniqueArray(arr, N);

        printf("\n==============================\n");
        printf("Tests for N = %d\n", N);
        printf("==============================\n");
		
		// bellek do�ru ayr�ld� m� kontrol� -- problem debuglamak i�in yazd�m.
        int *temp = (int *)malloc(N * sizeof(int));
        // Kontrol ama�l�, continue bo�una d�nmesin diye kullan�ld�.
		if (!temp) {
            printf("Memory allocation failed for temp array\n");
            free(arr);
            continue;
        }

        // bize verilen her k-way de�eri i�in yazd���m�z divide and conquer algoritmas�n� �al��t�r ve s�reyi �l�ece�iz
        for (k = 2; k <= 10; k++) {
            int *test_arr = (int *)malloc(N * sizeof(int));
            // Kontrol ama�l�, continue bo�una d�nmesin diye kullan�ld�.
			if (!test_arr) {
                printf("Memory allocation failed for test array\n");
                continue;
            }

            // diziyi kopyala
            for (j = 0; j < N; j++) {
                test_arr[j] = arr[j];
            }
			
			// zamanlay�c� ba�lat, s�ralama s�resini hesaplayaca��z daha sonra
            LARGE_INTEGER frequency, start, end;
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&start);
			
			// k-way merge sort'u �al��t�r
            kWayMergeSort(test_arr, temp, 0, N-1, k);
			
			// zamanlay�c�y� durdur ve s�reyi hesapla
            QueryPerformanceCounter(&end);
            double time_taken = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

            // s�ralaman�n do�ru olup olmad���n� kontrol et kopyas� ile
            int sorted = 1;
            for (j = 1; j < N; j++) {
                if (test_arr[j] < test_arr[j-1]) {
                    sorted = 0;
                    // burdaki break, bo�u bo�una d�nmesin.
                    break;
                }
            }

            printf("k=%d: %.6f seconds %s\n", k, time_taken, 
                   sorted ? "" : "- WARNING: not properly sorted!");

            free(test_arr);
        }

        free(temp);
        free(arr);
    }

    return 0;
}
