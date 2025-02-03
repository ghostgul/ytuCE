#include <stdio.h>
#define N 5000
int main(void)
{
 FILE *fp;
 char text[N];
 /********* de�i�ken tan�mlar� ************/
 int count = 0;
 int i;
 char c = '.';
 int isWord = 0;
 /********* de�i�ken tan�mlar� sonu ************/
 
 // Dosyay� a�ma
 if ((fp = fopen ("soru.txt", "r")) == NULL) {
 printf("Dosya a�ma hatas�!");
 return 1;
 }
 // Dosyadan okuma
 fgets(text, N-1, fp);
 printf("%s\n", text);
 printf("\n\n");
 /* 
 * text karakter dizisi de�i�keninde dosyadan okunan metin yer almaktad�r.
 * toplam kelime say�s�n�n hesab� ve bo�luk temizleme i�lemleri -TERC�HEN-
 * bu dizi �zerinde yap�lmal�d�r. Harici dizi kullan�m� �nerilmemektedir. 
 */
 /********* kod blo�u ************/
 printf("cikti: ");
 for( i=0 ; c!='\0' ; i++ )
 {
	c = text[i];
	if(c!=' ' && c!='\n' && c!='\0'){
		if (isWord==0)
		{
			count++;
		}
		isWord = 1;
		printf("%c", c);
	}
	else{
		if(isWord==1){
			printf(" ");
		}	
		isWord = 0;
 	}
 }
 printf("\n\nKelime sayisi:%d", count);
 /********* kod blo�u sonu ************/
 // Dosyay� kapama
 fclose(fp);
 return 0;
}
