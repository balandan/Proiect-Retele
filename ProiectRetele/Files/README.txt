Am realizat tema 2.
1.Pachete de date folosite:g++ compiler

2.Cum se instaleaza proiectul: se instaleaza prin comanda make in consola avand pathul unde se afla proiectul.

3.Explicatii:Se executa comanda make in consola pentru a compila fisierele sursa pentru client si server.Se deschide serverul prin comanda ./server iar apoi prin comanda ./client in consola, putem conecta unul sau mai multi clienti.Dupa conectare este necesara completarea de catre client a doua campuri:username si password.Odata conectat clientul poate alege una dintre urmatoarele comenzi :download (nume_fisier),delete (nume_fisier),copy (nume_fisier),ls ce listeaza tot continutul de fisiere pe care il detine serverul si exit comanda specifica pentru a iesi.La download dupa ce da comanda si numele fisierului, clientul trebuie sa ofere un path destinatie pentru fisierul downloadat.Prin comanda delete si un nume de fisier clientul poate alege ce fisier sa stearga iar prin comanda copy poate alege sa copieze un fisier.


4.Bibliografie:

-Server Tcp:
 https://profs.info.uaic.ro/~computernetworks/files/NetEx/S5/servTcpIt.c
-Client Tcp:
 https://profs.info.uaic.ro/~computernetworks/files/NetEx/S5/cliTcpIt.c
-Afisarea tuturor fisierelor dintr-un director:
 https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
-Detalii despre utilizarea unor functii ca read(),write(),open(),etc:
 http://gd.tuwien.ac.at/languages/c/programming-bbrown/c_075.htm
-Informatii suplimentare despre socketi si alte primitive in C:
 https://www.geeksforgeeks.org/socket-programming-cc/
