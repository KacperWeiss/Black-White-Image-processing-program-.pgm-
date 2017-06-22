/* Kacper Weiss
Przetwarzanie Obrazów 1
30.11.2016 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawidłowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien być P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return *wymx**wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */


/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/* ZAPISYWANIE OBRAZU */

int zapisz(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int i=0;
  int j=0;
  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

/* NEGATYW OBRAZU */

int negatyw(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int i=0;
  int j=0;
  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      Obraz[i][j]=(Obraz[i][j]*-1)+odcieni;
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

/* PROGOWANIE OBRAZU */

int progowanie(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int prog=odcieni/2;
  int i=0;
  int j=0;
  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      if (Obraz[i][j]<=prog) 
      {
	Obraz[i][j]=0;
      }
      else
      {
	Obraz[i][j]=odcieni;
      }
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

/* ZAPISYWANIE POLPROGOWANIA BIELI */

int polprogowaniebieli(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int prog=odcieni/2;
  int i=0;
  int j=0;
  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      if (Obraz[i][j]>prog)
      {
	Obraz[i][j]=odcieni;
      }
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

/* ZAPISYWANIE POLPROGOWANIA CZERNI */

int polprogowanieczerni(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int prog=odcieni/2;
  int i=0;
  int j=0;
  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      if (Obraz[i][j]<=prog) 
      {
	Obraz[i][j]=0;
      }
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

/* ZAPISYWANIE KONTUROWANIA */

int konturowanie(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int i=0;
  int j=0;

  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      int Obraz1[MAX][MAX];
      int Obraz2[MAX][MAX];
      Obraz1[i][j]=(Obraz[i+1][j]-Obraz[i][j]);
      Obraz2[i][j]=(Obraz[i][j+1]-Obraz[i][j]);

      if (Obraz1[i][j]<0) {Obraz1[i][j]=Obraz1[i][j]*-1;}
      if (Obraz2[i][j]<0) {Obraz2[i][j]=Obraz2[i][j]*-1;}

      Obraz[i][j]=Obraz1[i][j]+Obraz2[i][j];
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

/* ZAPISYWANIE ROZCIAGANIA HISTOGRAMU */

int rozciaganie(FILE *plik_wy, int Obraz[][MAX], int wymx, int wymy, int odcieni)
{
  fprintf(plik_wy, "P2\n");
  fprintf(plik_wy, "%d %d\n", wymx, wymy);
  fprintf(plik_wy, "%d\n", odcieni);

  int lmax=0;
  int lmin=odcieni;
  int i=0;
  int j=0;

  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      if (Obraz[i][j]>lmax) {lmax=Obraz[i][j];}
      if (Obraz[i][j]<lmin) {lmin=Obraz[i][j];}
    }
  }
  for (i=0; i<wymy; i++)
  {
    for (j=0; j<wymx; j++)
    {
      Obraz[i][j]=((Obraz[i][j]-lmin)*(odcieni/(lmax-lmin)));
      fprintf(plik_wy, "%d ", Obraz[i][j]);
    }
  fprintf(plik_wy, "\n");
  }
}

int main()
{
  int zakoncz = 0;
  char opcja=0;
  char opcja1[]="1 - Zapisz";
  char opcja2[]="2 - Negatyw";
  char opcja3[]="3 - Progowanie";
  char opcja4[]="4 - Polprogowanie bieli";
  char opcja5[]="5 - Polprogowanie czerni";
  char opcja6[]="6 - Konturowanie";
  char opcja7[]="7 - Rozciaganie histogramu";
  char opcjazakoncz[]="8 - Zakoncz dzialanie bez wykonania jakiejkolwiek akcji";

  int obraz[MAX][MAX] ;
  int wymx,wymy,odcieni;

  int odczytano = 0;
  FILE *plik;
  char nazwa[100];
  char nazwazapisu[100];

  printf("Podaj nazwe pliku:\n");
  scanf("%s",nazwa);
  plik=fopen(nazwa,"r");

  if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plik,obraz,&wymx,&wymy,&odcieni);
    fclose(plik);
  }

  /* Wyswietlenie poprawnie wczytanego obrazu zewnetrznym programem */
  if (odczytano != 0)
    wyswietl(nazwa);

  while (zakoncz!=1)
  {
    printf("Opcje przetworzenia obrazu:\n");
    printf("  %s\n", opcja1);
    printf("  %s\n", opcja2);
    printf("  %s\n", opcja3);
    printf("  %s\n", opcja4);
    printf("  %s\n", opcja5);
    printf("  %s\n", opcja6);
    printf("  %s\n", opcja7);
    printf("  %s\n", opcjazakoncz);
    printf("Twoj wybor: ");
    scanf("%c", &opcja);
  
    switch(opcja)
    {
      case '1':
	/* Zapisywanie obrazu programem */
        printf("Wybrano zapisywanie - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	zapisz(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
        break;
      case '2':
	/* Zapisywanie negatywu obrazu programem */
        printf("Wybrano zapisanie negatywu - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	negatyw(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
        break;
      case '3':
	/* Zapisywanie progowania obrazu programem */
	printf("Wybrano zapisanie progowania - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	progowanie(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
	break;
      case '4':
	/* Zapisywanie polprogowania bieli programem */
	printf("Wybrano zapisanie polprogowania bieli - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	polprogowaniebieli(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
	break;
      case '5':
	/* Zapisywanie polprogowania czerni programem */
	printf("Wybrano zapisanie polprogowania czerni - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	polprogowanieczerni(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
	break;
      case '6':
	/* Zapisywanie konturowania programem */
	printf("Wybrano zapisanie konturowania - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	konturowanie(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
	break;
      case '7':
	/* Zapisywanie konturowania programem */
	printf("Wybrano zapisanie rozciagniecia histogramu - podaj nazwe zapisywanego pliku:\n");
	scanf("%s",nazwazapisu);
	plik=fopen(nazwazapisu, "w");
	rozciaganie(plik, obraz, wymx, wymy, odcieni);
	fclose(plik);
	return odczytano;
	break;
      case '8':
        printf("Koniec dzialania programu\n\n");
        zakoncz++;
        break;
      default:
        printf("Brak opcji!\n");
        break;
    }
  }
  return odczytano;
}

/*=================================*/
/*||  	    SPRAWOZDANIE  	 ||*/
/*=================================*/


/*
OPIS DZIALANIA PROGRAMU:  	
Program wczytuje obraz z pliku wejsciowego w formacie .pgm, wyświetla go, potem wyswietla menu w ktorym wybieramy interesujaca nas operacje przetwarzania obrazu, a nastepnie program przetwarza go na plik wyjscia obrazu rowniez w formacie .pgm po odpowiedniej wybranej wczesniej modyfikacji i nazwie wybranej przez uzytkownika. Po wykonaniu dzialania program konczy prace.

Prawidlowy sposob wprowadzania danych wejsciowych:
Dane wejsciowe wprowadzamy w formie obrazu w formacie .pgm, obraz zawiera w kolejnosci:
-Odpowiedni "nr magiczny" --> P2
-Szerokosc obrazu (W)
-Wysokosc obrazu (H)
-Maksymalna wartosc szarosci (odcieni)
-Ciag W*H wartosci od 0 do maks szarosci
-Znak bialy pomiedzy kazdym z powyzej podanych elementow (co najmniej jeden)

1. Testy na wlasnym komputerze:
1.A. Sprawdzenie dzialania wczytania i wyswietlenia obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 8, skutkujace zakonczeniem programu. (Gdyz wybrany test ma na celu sprawdzenie tylko poprawnego wczytania i wyswietlenia obrazu).

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 8, skutkujace zakonczeniem programu. (Gdyz wybrany test ma na celu sprawdzenie tylko poprawnego wczytania i wyswietlenia obrazu).

1.B. Sprawdzenie dzialania zapisu obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 1, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisywanie - podaj nazwe zapisywanego pliku:", wpisuje kubuszapis.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubuszapis.pgm", wyswietla mi sie niezmieniony obraz o nazwie kubuszapis.pgm identyczny do kubus.pgm

Dane wyjsciowe: kubuszapis.pgm o tym samym wygladzie co oryginal

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 1, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisywanie - podaj nazwe zapisywanego pliku:", wpisuje lenazapis.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenazapis.pgm", wyswietla mi sie niezmieniony obraz o nazwie lenazapis.pgm identyczny do lena.pgm

Dane wyjsciowe: lenazapis.pgm o tym samym wygladzie co oryginal

1.C. Sprawdzenie dzialania zapisu negatywu obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 2, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie negatywu - podaj nazwe zapisywanego pliku:", wpisuje kubusnegatyw.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubusnegatyw.pgm", wyswietla mi sie negatyw obrazu o nazwie kubusnegatyw.pgm stworzony z obrazu kubus.pgm

Dane wyjsciowe: kubusnegatyw.pgm o kolorach odwroconych, tzn. tam gdzie najjasniejsze w oryginale, tam najciemniejsze w obrazie wyjsciowym i vice versa


b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 2, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie negatywu - podaj nazwe zapisywanego pliku:", wpisuje lenanegatyw.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenanegatyw.pgm", wyswietla mi sie negatyw obrazu o nazwie lenanegatyw.pgm stworzony z obrazu lena.pgm

Dane wyjsciowe: lenanegatyw.pgm o kolorach odwroconych, tzn. tam gdzie najjasniejsze w oryginale, tam najciemniejsze w obrazie wyjsciowym i vice versa

1.D. Sprawdzenie dzialania zapisu progowania obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 3, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie progowania - podaj nazwe zapisywanego pliku:", wpisuje kubusprogowanie.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubusprogowanie.pgm", wyswietla mi sie sprogowany obraz o nazwie kubusprogowanie.pgm stworzony z obrazu kubus.pgm

Dane wyjsciowe: kubusprogowanie.pgm o kolorach czarnym (w miejscach gdzie odcien byl ciemniejszy) i bialym (w miejscach gdzie odcien byl jasniejszy)

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 3, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie progowania - podaj nazwe zapisywanego pliku:", wpisuje lenaprogowanie.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenaprogowanie.pgm", wyswietla mi sie sprogowany obraz o nazwie lenaprogowanie.pgm stworzony z obrazu lena.pgm

Dane wyjsciowe: lenaprogowanie.pgm o kolorach czarnym (w miejscach gdzie odcien byl ciemniejszy) i bialym (w miejscach gdzie odcien byl jasniejszy)

1.E. Sprawdzenie dzialania zapisu polprogowania bieli obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 4, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie polprogowania bieli - podaj nazwe zapisywanego pliku:", wpisuje kubuspolprogowanieb.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubuspolprogowanieb.pgm", wyswietla mi sie przetworzony obraz o nazwie kubuspolprogowanieb.pgm stworzony z obrazu kubus.pgm

Dane wyjsciowe: kubuspolprogowanieb.pgm o kolorach niezmienionym (w miejscach gdzie odcien byl ciemniejszy) i bialym (w miejscach gdzie odcien byl jasniejszy)

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 4, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie polprogowania bieli - podaj nazwe zapisywanego pliku:", wpisuje lenapolprogowanieb.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenapolprogowanieb.pgm", wyswietla mi sie przetworzony obraz o nazwie lenapolprogowanieb.pgm stworzony z obrazu lena.pgm

Dane wyjsciowe: lenapolprogowanieb.pgm o kolorach niezmienionym (w miejscach gdzie odcien byl ciemniejszy) i bialym (w miejscach gdzie odcien byl jasniejszy)

1.F. Sprawdzenie dzialania zapisu polprogowania czerni obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 5, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie polprogowania czerni - podaj nazwe zapisywanego pliku:", wpisuje kubuspolprogowaniec.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubuspolprogowaniec.pgm", wyswietla mi sie przetworzony obraz o nazwie kubuspolprogowaniec.pgm stworzony z obrazu kubus.pgm

Dane wyjsciowe: kubuspolprogowaniec.pgm o kolorach czarnym (w miejscach gdzie odcien byl ciemniejszy) i niezmienionym (w miejscach gdzie odcien byl jasniejszy)

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 5, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie polprogowania czerni - podaj nazwe zapisywanego pliku:", wpisuje lenapolprogowaniec.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenapolprogowaniec.pgm", wyswietla mi sie przetworzony obraz o nazwie lenapolprogowaniec.pgm stworzony z obrazu lena.pgm

Dane wyjsciowe: lenapolprogowaniec.pgm o kolorach czarnym (w miejscach gdzie odcien byl ciemniejszy) i niezmienionym (w miejscach gdzie odcien byl jasniejszy)

1.G. Sprawdzenie dzialania zapisu konturowania obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 6, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie konturowania - podaj nazwe zapisywanego pliku:", wpisuje kubuskonturowanie.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubuskonturowanie.pgm", wyswietla mi sie skonturowany obraz o nazwie kubuskonturowanie.pgm stworzony z obrazu kubus.pgm

Dane wyjsciowe: kubuskonturowanie.pgm o kolorze czarnym z bialym konturem w miejscach konturu oryginalnego obrazu

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 6, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie konturowania - podaj nazwe zapisywanego pliku:", wpisuje lenakonturowanie.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenakonturowanie.pgm", wyswietla mi sie skonturowany obraz o nazwie lenakonturowanie.pgm stworzony z obrazu lena.pgm

Dane wyjsciowe: lenakonturowanie.pgm o kolorze czarnym z bialym konturem w miejscach konturu oryginalnego obrazu

1.H. Sprawdzenie dzialania zapisu rozciagania histogramu obrazu:

a) Testy na pliku kubus.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje kubus.pgm, a program wyswietla komunikat "display kubus.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 7, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie rozciagniecia histogramu - podaj nazwe zapisywanego pliku:", wpisuje kubushistogram.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display kubushistogram.pgm", wyswietla mi sie przetworzony obraz o nazwie kubushistogram.pgm stworzony z obrazu kubus.pgm

Dane wyjsciowe: kubushistogram.pgm o wygladzie prawie nie zmienionym (przy blizszym porownaniu widac niewielka roznice)

b) Testy na pliku lena.pgm
Uruchamiam program poleceniem ./a.out, nastepnie program wyświetla komunikat "Podaj nazwe pliku:" wpisuje lena.pgm, a program wyswietla komunikat "display lena.pgm &" jednoczesnie wyswietlajac wybrany przeze mnie obraz. Pojawia sie menu, gdzie wybieram polecenie nr 7, program prosi mnie o wpisanie nazwy nowo utworzonego obrazu: "Wybrano zapisanie rozciagniecia histogramu - podaj nazwe zapisywanego pliku:", wpisuje lenahistogram.pgm, a nastepnie program konczy dzialanie. Pozniej sprawdzam rezultat wykonanej operacji wpisujac komende "display lenahistogram.pgm", wyswietla mi sie przetworzony obraz o nazwie lenahistogram.pgm stworzony z obrazu lena.pgm

Dane wyjsciowe: lenahistogram.pgm o wygladzie prawie nie zmienionym (przy blizszym porownaniu widac niewielka roznice, ale wieksza niz w wypadku kubus.pgm)


*/

/***********************************************************/
/*                 WYNIKI TESTOW NA PC                     */
/***********************************************************/

/*
1.A.
a) Wynik pozytywny
b) Wynik pozytywny
1.B.
a) Wynik pozytywny
b) Wynik pozytywny
1.C.
a) Wynik pozytywny
b) Wynik pozytywny
1.D.
a) Wynik pozytywny
b) Wynik pozytywny
1.E.
a) Wynik pozytywny
b) Wynik pozytywny
1.F.
a) Wynik pozytywny
b) Wynik pozytywny
1.G.
a) Wynik pozytywny
b) Wynik pozytywny
1.H.
a) Wynik pozytywny
b) Wynik pozytywny

Wnioski:
Program dziala zgodnie z zalozeniami na PC
*/
