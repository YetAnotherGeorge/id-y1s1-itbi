Instrumente si tehnici de baza in informatica (ITBI), ID anul 1:
===============================================================

Continut disciplina:
 - generalitati despre sistemele de operare;
 - operare si programare in C sub Linux;
   principalele capitole de programare:
     utilizatori si grupuri, fisiere si directoare, semnale,
     procese, tuburi, scripting bash.

Materiale didactice:
 - arhiva generala cu lectii 'ddd_lectii_ma10feb2015.zip',
   folderul 'so_s18feb2012';
 - folderul 'adaos/USO_SO_SOL' (aici se gaseste inclusiv suportul
    pentru cursul de USO/ITBI, versiunea curenta este 'uso_j13ian2022.pdf');
 - materiale suplimentare transmise pe parcursul semestrului.
Atentie ca pe parcursul semestrului materialele pot fi actualizate
 si se vor transmite versiunile actualizate.

Notare:
 - se da o proba scrisa, unde se obtine un punctaj S si se rezolva
  doua probleme de laborator (programe), pentru care se vor obtine
  respectiv punctajele P1, P2; se mai da un punctaj din oficiu Of;
  aceste sarcini nu sunt toate obligatorii, prin lipsa punctajele
  aferente au o valoare implicita, a se vedea mai jos;
 - proba scrisa este cu materialele la vedere, dar numai in forma
    scrisa, si se dau numai problme; pentru rezolvarea problemelor
    va fi suficienta doar informatia din materialele scrise de mine;
 - problemele de laborator se aleg din suportul de curs, capitolul
    'Teme de laborator', aflat la sfarsit;
   o problema trebuie sa fie din sectiunile 'Utilizatori si grupuri'
    sau 'Fisiere, directoare' (punctajul P1);
   o problema trebuie sa fie din sectiunile 'Procese, semnale, tuburi'
    sau 'Scripting bash' (punctajul P2);
 - nota finala se calculeaza astfel:
  Notam:
  OF = punctele din oficiu, in numar de 2;
  S  = punctele la proba scrisa, in intervalul [0, 4], prin lipsa este 0;
  P1 = punctele la prima problema, in intervalul [0, 2], prin lipsa este 0;
  P2 = punctele la a doua problema, in intervalul [0, 2], prin lipsa este 0;
  F = nota finala, care se trece in catalog.
  Conditia de promovare este: OF + S + P1 + P2 >= 5.
  Regula de calcul a notei finale este:
     F = rotunjire(OF + S + P1 + P2)
   unde rotunjirea in sus se face de la pragul 0.5.
  Daca nota finala rezultata este < 4, in catalog se trece 4.
  Daca lipsesc toate componentele S, P1, P2, in catalog se trece absenta.


# Probleme Rezolvate

### Problema 1/2 (De la Fisiere / Directoare)
Problema 8: Implementarea matricilor dreptunghiulare de numere reale ca fisiere binare:  O matrice va fi stocata intr-un fisier binar continand elementele sale pe linii (in format intern), plus o informatie suplimentara din care sa se poata deduce numarul de linii si de coloane. 
Scrieti functii pentru urmatoarele operatii:
- void new(f,m,n) : creaza in fisierul f o matrice m x n initializata cu 0;
* float get(f,i,j) : returneaza elementul de pe pozitia i,j din matricea stocata in fisierul f;
* void set(f,i,j,x) : scrie elementul real x pe pozitia i,j in matricea stocata in fisierul f.

Se vor folosi functii de nivel superior (tipul ’FILE’).

Scrieti programe care folosesc aceste functii pentru a calcula suma si produsul a doua matrici. Programele se vor apela sub forma:
* sum f1 f2 f
* pro f1 f2 f
unde f1, f2 sunt specificatorii fisierelor continand matricile sursa, iar f specificatorul fisierului care va contine matricea destinatie. Fisierele f1 si f2 vor fi generate in prealabil cu alte programe ajutatatoare.
