## Problema 6: 
Scrieti un program care numara aparitiile unui sir de caractere ca subcuvant in alt sir de caractere cu procese paralele (cele doua siruri sunt date ca argumente in linia de comanda). De fiecare data cand se verifica daca primul sir apare ca subcuvant incepand de la o pozitie, verificarea se va face de catre un proces copil iar procesul parinte nu va asteapta ca acesta sa se termine pentru a initia o cautare incepand de la o alta pozitie - astfel, verificarile au loc in paralel. 

Fiecare proces copil furnizeaza parintelui raspunsul 'gasit' sub forma unui semnal SIGUSR1; handlerul asociat in parinte va incrementa un contor. Dupa initierea tuturor cautarilor, parintele asteapta primirea semnalelor si terminarea tuturor copiilor, apoi afisaza contorul. 

Se va asigura protectia la pierderea de semnale.