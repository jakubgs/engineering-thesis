Polsko-Japońska Wyszka Szkoła Technik Komputerowych
Inzynierska Praca Dyplomowa
Udostepnianie znakowych urządzeń fizycznych przez sieć w systemach GNU/Linux

Jakub Sokołowski

# Wstęp (5%)

Celem tego projektu jest stworzenie oprogramowania pozwalającego na współdzielenie znakowych urządzeń fizycznych za pośrednictwem sieci w dwóch zdanych maszynach z systemem GNU/Linux. Platforma GNU/Linux udostępnia wiele możliwości współdzielenia urządzeń blokowych takich jak dyski za pomocą oprogramowania działającego z przestrzeni użytkownika i korzystającego zazwyczaj z włąsnego protokołu komunikacji. Najbardziej znanymi przykładami są pakiety Samba oraz NFS pozwalające na zdalny dostęp do systemów plików.

Istnieją również metody udostępniania fizycznych urządzeń blokowych na poziomie jądra Linux przy u życiu pakietu oprogramowania [NBD(Network Block Device)][b00]. Pozwala on na stworzenie sztucznego urządzenia blokowego w zdalnym systemie które będzie się zachowywać tak jak by było to fizycznym urządzeniem podłączonym bezpośrednio do danej maszyny.

Tego typu metoda udostepniania urządzeń znakowych nie istnieje w systemie GNU/Linux.  W celu rozwiązania tego problemu poniższy projekt implemetnuje moduł jądra Linux oraz program serwerowy które w połączeniu umożliwiają współdzielenie urządzeń znakowych na stacjach połączonych przez sieć.

Projekt ten ilustruje wiele kluczowych modeli oraz standardów zdefiniowanych przez standard POSIX i wykorzystuje je w celu zaimplementowania rozwiązania które będzie przezroczyste dla jakiegokolwiek procesu który chciał by skorzystać ze zdalnego zasobu urządzenia znakowego.

## 

## Funkcjonalność

Sterownik jądra ma na celu stworzenie urządzenia-atrapy które będzie udawać w systemie rzeczywiste urządzenia fizyczne. Program serwerowy ma na celu odbieranie jakichkolwiek operacji wykonywanych na danym urządzeniu-atrapie i przekazanie ich do kolejnej instancji programu na zdalnej maszynie na której znajduje się rzeczywiste urządzenie które może odpowiedzieć na zapytania in wykonane operacje na urządzeniu-atrapie.

# Układ Pracy (10%)

* Podział pracy na podrozdziały

# Teoria

* definicja rozważanego zagadnienia
* aktualny stan wiedzy
* modele, wzory
* rzeczywisty stan

## Model sterowników urządzeń w jądrze Linux

## Model VFS

## 

# Wybrane rowziązania (10%)

* Wymienienie problemów
* możliwych rozwiązań
* wybrane rowziązania

## Wybór metody komunikacji z przestrzenią użytkownika

## Wybór metody komunikacji ze zdalnym urządzeniem

## Rozwiązanie problemu serializacji

## Prohlem wywłaszczania/współdzielenia

## Problem przechowywania danych

## Sposób implementacji programu serwera

# Implementacja (70%)

* techniczna strona rozwiązania
* istotne wyjątki z kodu
* kluczowe elementy kodu

# Wyniki (5%)

## Porównanie opóźnień
* Testy

## Porównanie predkości transmisji danych
* Analiza wydajności

## Identyfikacja wąskich gardeł

## Proponowane możliwe zmiany w architekturze/poprawki

## Bibliografia

[b00]: http://nbd.sourceforge.net/ "Nework Block Device"
[b01]: "Linux Kernel - Przewodnik Programisty", Robert Love
[b02]: "Linux Device Drivers", 3rd Edition, Jonathan Corbet, Alessandro Rubini i Greg Kroah-Hartman
[b03]: "Understanding Linux Kernel", 3rd Edtion, Daniel P. Bovet i Marco Cesati
[b04]: "Programowanie zastosowań sieciowych w systemie UNIX", Tom 1, Richard Stevens
[b05]: "Overview of Linux-Kernel Reference Counting", Paul E. McKenney, Linux Technology Center, IBM Beaverton
[b06]: "The Netlink protocol: Mysteries Uncovered", Jan Engelhardt
[b07]: "Communicating between the kernel and user-space in Linux using Netlink Sockets", Pablo Neira Ayuso, Rafael M. Gasca i Laurent Lefevre

## Zawartość CD
## Spis tabel
## Spis rysunków
