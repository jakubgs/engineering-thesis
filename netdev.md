Polsko-Japońska Wyszka Szkoła Technik Komputerowych
Inzynierska Praca Dyplomowa
Udostepnianie znakowych urządzeń fizycznych przez sieć w systemach GNU/Linux

Jakub Sokołowski

# Wstęp (2%)

Celem tego projektu jest stworzenie oprogramowania pozwalającego na współdzielenie znakowych urządzeń fizycznych za pośrednictwem sieci w dwóch zdanych maszynach z systemem GNU/Linux. Platforma GNU/Linux udostępnia wiele możliwości współdzielenia urządzeń blokowych takich jak dyski za pomocą oprogramowania działającego z przestrzeni użytkownika i korzystającego zazwyczaj z włąsnego protokołu komunikacji. Najbardziej znanymi przykładami są pakiety Samba oraz NFS pozwalające na zdalny dostęp do systemów plików.

Istnieją również metody udostępniania fizycznych urządzeń blokowych na poziomie jądra Linux przy u życiu pakietu oprogramowania [NBD(Network Block Device)][b00]. Pozwala on na stworzenie sztucznego urządzenia blokowego w zdalnym systemie które będzie się zachowywać tak jak by było to fizycznym urządzeniem podłączonym bezpośrednio do danej maszyny.

Tego typu metoda udostepniania urządzeń znakowych nie istnieje w systemie GNU/Linux.  W celu rozwiązania tego problemu poniższy projekt implemetnuje moduł jądra Linux oraz program serwerowy które w połączeniu umożliwiają współdzielenie urządzeń znakowych na stacjach połączonych przez sieć.

Projekt ten ilustruje wiele kluczowych modeli oraz standardów zdefiniowanych przez standard POSIX i wykorzystuje je w celu zaimplementowania rozwiązania które będzie przezroczyste dla jakiegokolwiek procesu który chciał by skorzystać ze zdalnego zasobu urządzenia znakowego.

##

## Funkcjonalność

Sterownik jądra ma na celu stworzenie urządzenia-atrapy które będzie udawać w systemie rzeczywiste urządzenia fizyczne. Program serwerowy ma na celu odbieranie jakichkolwiek operacji wykonywanych na danym urządzeniu-atrapie i przekazanie ich do kolejnej instancji programu na zdalnej maszynie na której znajduje się rzeczywiste urządzenie które może odpowiedzieć na zapytania in wykonane operacje na urządzeniu-atrapie.

# Układ Pracy/Założenia projektu (3%)

* Podział pracy na podrozdziały

## Przenośność
### Język programistyczny

Z uwagi na to iż większość kodu znajduje się w module jądra Linux głównym i jedynym językiem implementacji projektu jest język C obsługiwany przez kompilator GCC. Standard GCC wprowadził dużą liczbę rozszerzeń i ustprawnień do podstawowego standardu ANSI C również znanego jako C89. Pisanie kodu jądra systemu wymaga dużej kontroli nad wyjątkowo specyficznymi szczegółami implementacji. Z uwagi na to Linus Torvalds podjął świadomą decyzje aby przywiązać implementacji Linuxa do jednego kompilatora - GCC - użyskując w ten sposób liczbę [dodtkowych fukncjonalności][b10] oraz wyższy poziom kontroli na kodem.

Kod jadra nie jest jednak napisany w czystym C. Wiele kluczowych elementów jądra, zwłaszcza te które są zależne od platformy czyli zazwwyczaj używanego procesora, jest napisanych w asemblerze. Asembler jest interetowany przez GCC i zapisywany przy pomocy wywołania [asm][b11].

### Biblioteki

Wszystkie biblioteki urzyte w projekcie są częścią standardu POSIX. Implementacja komunikacji sieciowej jak i komunikacji pomiędzy przestrzenią jądra a przestrzenią użytkownika korzysta ze standardowych bibliotek systemu GNU/Linux. TODO

## Interfejs
### Argumenty
### Plik konfiguracyjny

## Podział pracy

# Teoria (5%)

* definicja rozważanego zagadnienia
* aktualny stan wiedzy
* modele, wzory
* rzeczywisty stan

## Model sterowników urządzeń w jądrze Linux

## Rezerwacja pamięci w jądrze

## Zabezpieczenia przed paniką jądra

## Model VFS

## Urządzenia znakowe

# Wybrane rowziązania (10%)

* Wymienienie problemów
* możliwych rozwiązań
* wybrane rowziązania

## Wybór metod komunikacji

### Z przestrzenią użytkownika

Ponieważ tworzenie tworzenie nowych urządzeń-atrap oraz wybieranie zdalnej maszyny do której należy się podłączyć jak i samo zadanie przesyłania operacji po sieci musi być wykonane w przestrzeni użytkownika moduł jądra musi posiadać jakąś metodę komunikacji pomiędzy przestrzenią jądra a przestrzenią użytkownika. Jądro Linux udostępnia kilka możliwości prowadzenia takiej komunikacji. Mają one swoje wady i zalety kluczowym jest więc wybranie tej która zapewni wystarczające przenośność, przepustowość, rozszerzalność oraz szybkość w informowaniu o nowych zdarzeniach w systemie.

    * Przenośność oznacza spójną strukturę danych na przestrzeni wielu różnych platform. Podsatwowym problemem w komunikacji pomiędzy przestrzenią użytkownika a jądra może być na przykłąd wielkość słowa maszynowego które może się zmieniać w zależności od architektury na której się pracuje. Architektura x86_64 pozwala na używane 32 bitowych słów w przestrzeni użytkownika w czasie gdy słowo w przestrzeni jądra może mieć 64 bity. Tego typu różnice są kluczowe podczas wymiany danych takich jak wskaźniki czy wartości long.

    * Przepustowość decyduje o tym jak szybko i jak duże ilości danych mogą być przekazywane pomiędzy przestrzeniami użytkownika i jądra. W przypadku pojedyńczych opcji nie jest to problemem jednak istnieje wiele podsystemów jądra które wymagają transferów znacznych ilości informacji w celu ich konfiguracji. Dobrym przykłądem jest system zapory ogniowej w jądrze Linux, zaawansowany routing oraz narzędznia do zarządzania kluczami w protokole IPsec.

    * Rozszerzalność zapewnia iż zmiany w formacie przekazywanych danych nie spowodują problemów ze wsteczną kompatybilnością ze starszymi wersjami tego samego oprogramowania. Używanie statycznych struktur lub formatów podobnych do SCV w których poszczególne dane są po prostu rozdzielone przecinkami lub innymi znakami są trudne w modyfikowaniu pez powodowania poważnych problemów w w poprzednich wersjach. Z uwagi na to dodawanie nowych funkcjonalności może powodować wiele problemów.

    * Szybkość w informowaniu o zdażeniach zazwyczaj oznacza iż program w przestrzeni użytkownika nie jest zmuszony nieprzerwanie odpytywać jakiś element systemu w celu uzyskania informacji o zmianie stanu. Budzony jest on sam przez jądro w celu poinformowania o zmianie stanu w urządzeniu lub module o którym informacje próbuje uzyskać dana aplikacja przestrzeni użytkownika.

Interfejsy udostępnione przez jądro można podzielić na trzy podstawowe kategorie. Z czego pierwsza jest tak kluczowa iż jest ona podstawą dla wszystkich pozostałych. Najbardziej pierwotnym i podstawowym sposobem komunikacji z jądrem są wywołania systemowe. System Linux implemetnuje ich w obecnej chwili 244 plus 16 zależnych od architektury i liczba ta nie zmienia się często z uwagi na to iż większość wywołań systemowych pyło zaprojektowanych zmyślą o wypełnianiu wielu ról jednocześnie. Większość programistów jądra stroni od dodawania nowych wywołań z uwagi na to iż dotychczas istniejące so prawie w każdym przypadku wystarczające do każdego możliwego zastosowania oraz ponieważ raz dodane wywołanie otrzymuje swój numer identyfiklacyjny który musi być umieszonych w dwóch kluczowych plikach: arch/sh/kernel/syscalls_32.S oraz include/uapi/asm-generic/unistd.h, i raz tam dodany nie może być usunięty. Numer ten na zawsze odpowiada temu wywołaniu i gdyby przestał być implementowany musi pozostać wśrod definicji i zwracać odpowiedni kod błędu w przypadku jego użycia. Prawdą jest że żadna inna forma komunikacji z jądrem nie jest tak szybka, co sprawia iż jest to kusząca możliwość, jednak z uwagi na wymienione zastrzeżenia tworzenie własnych wywołań systmowych jest prawie zawsze złym pomysłem i powinno być uznawane jako ostateczność.

Pozostałe dwie kategorie to wirtualne systemy plikół takie jak /proc i /sys oraz standardowe gniazda BSD. Wszystkie korzystają z już istniejących wywołań systemowych i implementują istniejące modele używane do komunikacji z plikami na dyskach oraz z gniazdami sieciowymi. W przypadku systemu plików /proc dane pobierane są z plików w postaci czytelnej dla człowieka ale mało praktycznej dla programu który musi przeprowadzić analizę składniową danych w cely otrzymania jakichkolwiek informacji. To samo tyczy się wysyłania ich do plików znajdujących się w systemie /proc. Dodatkowo transfer danych jest ograniczony do jednej strony pamięci oraz nie udostępnia żadnego mechanizmu do informowania programu o zdażeniach w jądrze. Oryginalnie miał on być używany jedynie w cely przekazywania informacji na temat istniejących procesów, niestety w obecnej chwili jest on naużywany do wielu innych zdań i przez cały czas trwają pracę nad przeniesieniem całej funkcjonalności do systemu plików /sys który udostępnia kilka usprawnienie w stosunku do jego poprzednika.

Wirtualny system plików /sys znany również jako sysfs ma znacznie bardziej logiczną strukturę która pozwala ne eksportowanie obiektów z przestrzeni jądra jako katalogów, ich atrybutów jako plików oraz relacji z innymi obiektami jako dowiązania symboliczne. Posiada on również sygnalizację o wydażeniach zaimplementowaną przy pomocy gniazd Netlink o których będzie mowa w nastepnym paragrafie. Niestety system sysfs nadal ogrannicza transfer danych do jednej strony pamięci oraz format danych potrafi zmieniać się pomiędzy wersjami jądra więc nie jest to bezpieczny sposób przekazywania informacji do przestrzeni użytkownika.

TODO

### Ze zdalnym urządzeniem

## Rozwiązanie problemu serializacji

## Prohlem wywłaszczania/współdzielenia

## Problem przechowywania danych

## Sposób implementacji programu serwera

# Implementacja (70%)

* techniczna strona rozwiązania
* istotne wyjątki z kodu
* kluczowe elementy kodu

## Budowanie i uruchamianie kodu
### Makefile
### insmod i rmmod

## Podział kodu
### Pliki
### Wspólne nagłówki

## Kluczowe struktury danych
### netdev_data
### fileop_request
### struktury operacji

## Wielowątkowość/Wywłaszczanie
### Semafory rw
### atomic_t

## Netlink
### Tworzenie gniazda
### odbieranie wiadomości
### wysyłanie wiadomości

## Architektura serwera
### Podział na procesy
### Uzycie select()

##

# Wyniki (5%)

* Testy
* Analiza wydajności

## Porównanie opóźnień

## Porównanie predkości transmisji danych

## Identyfikacja wąskich gardeł

## Proponowane możliwe zmiany w architekturze/poprawki

# Podsumowanie

# Bibliografia

[b00]: http://nbd.sourceforge.net/ "Nework Block Device"
[b01]: "Linux Kernel - Przewodnik Programisty", Robert Love
[b02]: "Linux Device Drivers", 3rd Edition, Jonathan Corbet, Alessandro Rubini i Greg Kroah-Hartman
[b03]: "Understanding Linux Kernel", 3rd Edtion, Daniel P. Bovet i Marco Cesati
[b04]: "Programowanie zastosowań sieciowych w systemie UNIX", Tom 1, Richard Stevens
[b05]: "Overview of Linux-Kernel Reference Counting", Paul E. McKenney, Linux Technology Center, IBM Beaverton
[b06]: "The Netlink protocol: Mysteries Uncovered", Jan Engelhardt
[b07]: "Communicating between the kernel and user-space in Linux using Netlink Sockets", Pablo Neira Ayuso, Rafael M. Gasca i Laurent Lefevre
[b10]: http://gcc.gnu.org/onlinedocs/gcc/C-Extensions.html#C-Extensions "Extensions to the C Language Family"
[b11]: http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html#s4 "Inline Assembly HOWTO"

# Dodatki
## Zawartość CD
## Spis tabel
## Spis rysunków
