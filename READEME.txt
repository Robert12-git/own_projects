// CIUREA ROBERT-MIHAI 313CB
// PC - TEMA 1

		    REZOLVARE DE TASKURI

Gates.c

	Task 1 a fost efectuat utilizand operatii pe biti, folosindu-ne de masti pentru a efectua urmatoarele operatii in 
aceasta ordine implementate: aflarea valorii unui bit,schimbarea valorii unui bit(0 -> 1, 1 -> 0), activarea unui bit si 
dezactivarea unui bit.
	Task 2 a fost efectuat creand poarta de nandutilizand formula aferenta, pentru ca restul portilorsa fie implementate
utilizand numai porti nand.
	Task 3 a fost efectuat utilizand o functia ajutatoare full_adder ce ne face suma a 2 biti, salvand un al 3 lea bit
ce reprezinta carryul, astfel incat functia ripple_carry_adder sa returneze suma a 2 variabile, tinand cont de faptul ca 
daca se face overflow, adica daca suma noastra depaseste limitele spatiului predefinit, sa se returneze valoarea 0.

Communication.c

	Observatii:
	-taskurile au fost efectuate cu ajutorul functiilor send_squanch ce trimite mesajul codat si recv_squanch ce primeste
mesajul codat
	-mesajul despre care vorbim pentru functiile ajutatoare reprezinta numere de la 1 la 26 ce constituie litere de la 
A la Z
	-decodarea unui mesaje se face adunand mesajul cu 64 pentru a aduce fiecare litera la forma ei in codul ASCII, pentru
ca in final sa facem o conversie in forma de caracter(ex: (char)(mesaj + 64)).
	-codarea unui mesaj reprezinta procesul invers decodarii, asa ca nu il vom mai descrie
	-atunci cand se va pune problema sa primim un sir de caractere, inainte de toate vom primi lungimea codata a sirului
de caractere. Faptul ca lungimea este codata inseamna ca biti lungimii au fost shiftati cu 2 biti la stanga, astfel pentru
a avea acces la valoarea lungimii este necesara efectuarea unei shiftari la stanga cu 2 biti pentru a deseta ultimii 2 biti
in cazul in care unul dintre acestia ar fi fost setat, pentru ca apoi sa efectuam o shiftare la dreapta cu 4 biti. Acest
proces reprezinta decodarea lungimii.
	-asemeni primirii unui sir de caractere, trimiterea va presupune inainte de toate codarea lungimii sirului de
caractere. Acest lucru se face efectuand o shiftare la stanga cu 2 biti.
	-mai multe detalii in legatura cu rezolvarea acestor taskuri apar in codul sursa

Hunt.c
	
	Task 1 a fost efectuat implementand functiile find_spell si find_key al caror nume anticipeaza rolul acestora.
In ultima functie, spell si key considerandu-se cunoscute, se face xor intre acestea 2 pentru a se face decriptarea 
spellului.
	Task 2 a fost efectuat rezolvandu-se Monster Equation si Human Equation, pentru ca in final pe ultimii biti ai sabiei 
formate sa setam tiparul necesar activarii acesteia, respectiv 1001 pentru pentru monstri si 0110 pentru oameni.
	Task 3 se concentreaza exclusiv pe explicatiile din enunt, care de asemenea se afla in codul sursa.

----------------explicatii mai amanuntite ale taskurilor si ale functiilor utilizate se gasesc in codul sursa----------------
	
	