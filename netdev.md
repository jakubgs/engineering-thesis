Polsko-Japońska Wyższa Szkoła Technik Komputerowych
Inżynierska Praca Dyplomowa
Udostępnianie znakowych urządzeń fizycznych przez sieć w systemach GNU/Linux

Jakub Sokołowski

# Wstęp

Celem tego projektu jest stworzenie oprogramowania pozwalającego na współdzielenie znakowych urządzeń fizycznych za pośrednictwem sieci w dwóch zdalnych maszynach z systemem GNU/Linux. Platforma GNU/Linux udostępnia wiele możliwości współdzielenia systemów plików znajdujących się na urządzeń blokowych takich jak dyski za pomocą oprogramowania działającego z przestrzeni użytkownika i korzystającego zazwyczaj z własnego protokołu komunikacji. Najbardziej znanymi przykładami są pakiety Samba oraz NFS pozwalające na zdalny dostęp do normalnych plików.

Istnieją również metody udostępniania fizycznych urządzeń blokowych na poziomie jądra Linux przy u życiu pakietu oprogramowania [NBD(Network Block Device)][b00]. Pozwala on na stworzenie sztucznego urządzenia blokowego w zdalnym systemie które będzie się zachowywać tak jak by było to fizycznym urządzeniem podłączonym bezpośrednio do danej maszyny.

Tego typu metoda udostępniania w systemie GNU/Linux nie istnieje dla urządzeń znakowych. W celu rozwiązania tego problemu poniższy projekt implementuje moduł jądra Linux oraz program serwerowy, które w połączeniu umożliwiają współdzielenie urządzeń znakowych na stacjach połączonych przez sieć.

Projekt ten ilustruje wiele kluczowych modeli oraz standardów zdefiniowanych przez standard POSIX i wykorzystuje je w celu zaimplementowania rozwiązania które będzie przezroczyste dla jakiegokolwiek procesu który chciał by skorzystać ze zdalnego zasobu urządzenia znakowego.

## Funkcjonalność

Sterownik jądra ma na celu stworzenie urządzenia-atrapy które będzie udawać w systemie rzeczywiste urządzenia fizyczne. Program serwerowy ma na celu odbieranie jakichkolwiek operacji wykonywanych na danym urządzeniu-atrapie i przekazanie ich do kolejnej instancji programu na zdalnej maszynie na której znajduje się rzeczywiste urządzenie które może odpowiedzieć na zapytania in wykonane operacje na urządzeniu-atrapie.

## Język programistyczny

Z uwagi na to, iż większość kodu znajduje się w module jądra Linux głównym i jedynym językiem implementacji projektu jest język C obsługiwany przez kompilator GCC. Standard GCC wprowadził dużą liczbę rozszerzeń i usprawnień do podstawowego standardu ANSI C również znanego jako C89. Pisanie kodu jądra systemu wymaga dużej kontroli nad wyjątkowo specyficznymi szczegółami implementacji. Z uwagi na to Linus Torvalds podjął świadomą decyzje aby przywiązać implementacje Linuxa do jednego kompilatora - GCC - uzyskując w ten sposób liczbę [dodatkowych funkcjonalności][b10] oraz wyższy poziom kontroli na kodem.

Kod jadra nie jest jednak napisany w czystym C. Wiele kluczowych elementów jądra, zwłaszcza te które są zależne od platformy, czyli zazwyczaj używanego procesora, jest napisanych w asemblerze. Asembler jest interpretowany przez GCC i zapisywany przy pomocy wywołania [asm][b11].

## Biblioteki

Wszystkie wywołania systemowe użyte w projekcie są częścią standardu POSIX. Implementacja komunikacji sieciowej jak i komunikacji pomiędzy przestrzenią jądra a przestrzenią użytkownika korzysta ze standardowych bibliotek C środowiska operacyjnego GNU czyli [GLIBC][b13]. Żadne dodatkowe biblioteki nie zostały wykorzystane przy rozwoju tego projektu. Wszystkie niezbędne struktury danych takie jak tablice haszujące czy kolejki FIFO(First In, First Out) mają swoje ogólne implementacje dostępne bezpośrednio w nagłówkach jądra Linux. Do implementacji programu serwera zostały wykorzystane podstawowe funkcje interfejsu gniazd BSD dostępne w systemie Linux czyli `sendmsg`, `recvmsg` oraz `select`. Serializacja danych danych oraz implementacja protokołu komunikacji zostały zaimplementowane własnoręcznie bez użycia dodatkowych bibliotek.

## Interfejs

Interfejs przez jaki potencjalny użytkownik będzie mógł korzystać z tego oprogramowania przyjmuje dwa podstawowe sposoby zarządzania oprogramowaniem w systemach z rodziny Unix czyli tekstowe pliki konfiguracyjne oraz argumenty przekazywane bezpośrednio do programu serwera.

### Argumenty
### Plik konfiguracyjny

# Organizacja pracy

* Podział pracy na podrozdziały

# Teoria

* definicja rozważanego zagadnienia

## Urządzenia jako pliki

Tradycyjnie systemu Unix oraz jego pochodne - takie jak GNU/Linux - traktował wszystkie zasoby, które dało się odczytać lub zapisać w jednakowy sposób, czyli jako pliki. Bez względu na to czy dany zasób jest dokumentem tekstowym, dyskiem, klawiaturą, kartą dźwiękową czy gniazdem sieciowym jest on reprezentowany w systemie jako plik. Chociaż poprawnie należało by powiedzieć, iż jest reprezentowany jako deskryptor pliku od momentu, w którym jakiś proces otworzy dany plik w celu skorzystania z jego zasobów.

Istnieje siedem podstawowych typów plików:

* Pliki normalne - Najbardziej pospolity rodzaj pliku, tekstowy lub binarny, reprezentuje logicznie spójny obiekt przechowujący dane w jakimś konkretnym medium, na dysku, płycie lub w pamięci systemu.
* Pliki katalogów - Katalogi również są traktowane jak pliki w systemach z rodziny Unix. Są dość specyficznymi plikami ponieważ przechowują w sobie informacje na temat innych plików jakie się w nich znajdują ale nadal są traktowane jak pliki przez system.
* Pliki dowiązań symbolicznych- Specjalny rodzaj pliku, który może wskazywać na dowolny inny plik istniejący w systemie.
* Pliki blokowe - Te pliki reprezentują urządzenia takie jak dyski lub czytniki optyczne. Można je znaleźć w systemie plików `/dev`.
* Pliki znakowe - Najbardziej znane pliki znakowe to klawiatury lub myszki. Jest ich jednak znacznie więcej. Wirtualne terminale, modemy i jeszcze do niedawna drukarki są urządzeniami znakowymi. Również do znalezienia w systemie plików `/dev`.
* Pliki potoków - Jeden z dostępnych metod komunikacji międzyprocesowej. Istnieją nazwane i nienazwane potoki. Nazwane potoki są widoczne w systemie plików jako pliki, nienazwane nie są, nadal jednak tworzony jest dla nich deskryptor pliku w systemie.
* Pliki gniazd - Gniazda BSD zazwyczaj tworzone w celu komunikacji sieciowej przy pomocy protokołów TCP lub UDP. Pozwalają również na komunikację między procesową.

Na wszystkich tych plikach można wykonywać ten sam zestaw operacji, które zostaną dokładnie opisane w rozdziale [Model VFS]. W celu stworzenia modułu jądra, który tworzy specyficzny rodzaj pliku, na przykład plik blokowy, i obsługuje wykonane na nim operacji musi on implementować te operacje.

Jako że ten projekt obsługiwać będzie urządzenia znakowe reszta pracy skupi się na plikach urządzeń znakowych.

## Model VFS

Wirtualny system plików znany przede wszystkim pod skrótem VFS(Virtual File System) to warstwa abstrakcji wprowadzona w jądrze Linux mająca na celu ujednolicenie interfejsów, przez które przestrzeń jądra oraz przestrzeń użytkownika mają dostęp do różnego rodzaju systemów plików. W dzisiejszych czasach istnieje ogromne bogactwo wszelkiego rodzaju systemów plików, od dość uniwersalnych, takich jak rodzina Ext(2,3 i 4) po stronie GNU/Linux czy FAT32 oraz NTFS po stronie produktów Microsoft, przez przystosowane do konkretnych zastosowań takich jak ZFS, BTRFS, UFS czy JFS, sieciowe systemy plików jak NFS, SMBFS i CIFS, po wirtualne systemy plików takie jak `procfs`, `devfs`, `sysfs` czy `tmpfs`. Absurdem było by implementowanie oddzielnych narzędzi do współpracy ze wszystkimi tymi systemami. W celu unifikacji sposobu implementowania dostępu oraz kontroli wszelkiego rodzaju systemów plików stworzony został model wirtualnego systemu plików VFS. W jądrze Linux jest to prawdopodobnie system, który najbardziej czerpie z filozofii programowania obiektowego, pomimo tego iż jest ono prawie w 100% napisane w czystym języku C które nie posiada mechanizmów obiektowości takich jak dziedziczenie czy operacje na obiektach znanych z takich języków jak C++ czy Java.

Dzięki wprowadzeniu tego modelu wszystkie sterowniki poszczególnych systemów plików muszą implementować konkretny zestaw struktur oraz funkcji, które w sposób zupełnie przezroczysty pozwalają wszystkim pozostałym elementom systemu na korzystanie z zasobów znajdujących się w tych systemach plików bez zastanawiania się na różnicami pomiędzy poszczególnymi implementacjami, formatem danych czy rodzajem nośnika na którym przechowywane są owe dane. Bez względu na to jak rzeczywiście działa dany system plików, musi on przedstawiać swoje dane przy użyciu czterech podstawowych obiektów zdefiniowanych przez model VFS:

* Blok główny(superblock) - Jest to obiekt który zbiera w sobie wszystkie informacje o plikach i strukturze katalogów znajdujących się w danym systemie plików, jak i również jego rodzaj, przypisane do niego urządzenie oraz listę operacji które można wykonać na danym systemie plików. Blok główny bardzo często jest przechowywany w specjalnie do tego przeznaczonym sektorze dysku lub innego nośnika i posiada wszystkie informacje potrzebne by zlokalizować dane składające się na wybrany plik na przestrzeni całego nośnika. Systemy plików nie przywiązane do żadnego urządzenia fizycznego nadal muszą stworzyć obiekt bloku głównego i przechowywać go w pamięci jądra jeżeli chcą w jakimkolwiek wymiarze udostępniać swoje usługi jądru oraz procesom przestrzeni użytkownika. Blog główny definiowany jest jako struktura `super_block`.

* i-węzeł(i-node) - Każdy otwarty plik oraz katalog znajdujący się w obrębie konkretnego systemu plików reprezentowany jest przez jeden egzemplarz tego obiektu. Zbiera on w sobie wszystkie informacje potrzebne do manipulowania plikami i istnieje on tak długo jak długo plik lub katalog jest używany przez jakikolwiek proces w systemie. Obiekt i-węzła opisuje dowolny plik, bez względu na to czy jest to zwykły plik na dysku, plik strumienia danych, urządzenie z systemu plików `/dev` czy plikowa reprezentacja atrybutów sterowników w systemie plików `/sys`. Zawiera on w sobie informacje o czasie stworzenia i modyfikacji, prawa dostępu, właścicielu, rozmiarze pliku i wiele innych informacji niezbędnych procesom do wykonywania na nich operacji. Warto od razu zwrócić uwagę, iż w systemach z rodziny Unix takich jak Linux katalogi są po prostu specyficznym rodzajem pliku, w związku z tym i-węzeł opisuje zarówno pliki jak i katalogi. Struktura która definiuje ten obiekt nazywa się `inode`.

* Wpis katalogowy(dentry) - Obiekt ten jest przeznaczony raczej do ułatwienia wyszukiwania danych w hierarchicznej strukturze katalogowej wielu systemów plików niż do opisywania rzeczywistych obiektów znajdujących się w pojedynczym systemie plików. Każdy program przestrzeni użytkownika musi korzystać ze ścieżki dostępu jeżeli chce się dostać do jakiegokolwiek zasobu w systemie plików. Przykładową ścieżką dostępu do pliku znajdującego się na płycie CD jest `/mnt/cdrom/README.txt`. W tej ścieżce dostępu znajdują się cztery wpisy katalogowe: `/`, `mnt`, `cdrom` oraz `README.txt`. Trzy pierwsze obiekty, `/` przedstawiający katalog-korzeń i `mnt` katalog znajdujący się w korzeniu oraz `cdrom` zawierający się w katalog `mnt`, znajdują się na systemie plików ext4. Jednak sam plik `README.txt` znajduje się w systemie plików płyty CDROM, ISO9660. Jądro utrzymuje bufor wpisów katalogowych, który zbiera w miarę jak procesy otwierają i używają plików dostępnych w systemie. Utrzymywanie tego typu bufora znacznie przyspiesza otwieranie wcześniej otwartych zasobów. Wpisy te reprezentowane się przez strukturę `dentry`.

* Plik(file) - Tak jak i-węzeł reprezentuje każdy przynajmniej raz otwarty plik w jądrze tak obiekt pliku reprezentuje plik otwarty przez konkretny proces. Obiekt ten istnieje tak długo jak dany proces korzysta z rzeczywistego pliku i zostaje skasowany gdy proces ten użyje wywołania close() na przypisanym do niego deskryptorze pliku(file descriptor). Posiada on odwołanie do odpowiedniego obiektu i-węzła oraz zbiór informacji przydatnych przy operowaniu na pliku takich jak obecna pozycja w pliku, tryb dostępu do pliku czy zestaw praw dostępu. Nazwa jego struktury to `file` i jest to reprezentacja deskryptora pliku, który otrzymuje proces wywołując funkcje `open()` w przestrzeni jądra.

Z punktu widzenia sterowników urządzeń najbardziej kluczowymi obiektami są i-węzły oraz pliki. Obiekt pliku posiada jedną z najważniejszych struktury w całym jądrze Linux o nazwie `file_operations`, zapisaną jako atrybut `f_op`, która posiada kolekcję wskaźników do funkcji które pozwalają wykonywać takie operacje jak `open`, `read`, `write` czy `close`. Wszystkie urządzenia znakowe oraz blokowe muszą implementować podstawowy zestaw operacji reprezentowany przez tą strukturę i prawie każda z nich bierze jako argument obiekt pliku. Niektóre biorą również obiekt i-węzła. Dzięki temu sterownik otrzymuje niezbędne informacje odnośnie tego, który plik urządzenia został otwarty.

Struktura `file_operations` jest kluczowym interfejsem który pozwala ukryć ogromną różnorodność urządzeń pod zestawem prostych operacji, z których każdy proces w przestrzeni użytkownika może skorzystać bez rozróżniania pomiędzy typami urządzeń. Struktura ta jest zdefiniowana razem z wcześniej wymienionymi strukturami w pliku `include/linux/fs.h` i wygląda następująco:

    struct file_operations {
	    struct module *owner;
	    loff_t (*llseek) (struct file *, loff_t, int);
	    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	    ssize_t (*aio_read) (struct kiocb *, const struct iovec *,
                    unsigned long, loff_t);
	    ssize_t (*aio_write) (struct kiocb *, const struct iovec *,
                    unsigned long, loff_t);
	    int (*readdir) (struct file *, void *, filldir_t);
	    unsigned int (*poll) (struct file *, struct poll_table_struct *);
	    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	    long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	    int (*mmap) (struct file *, struct vm_area_struct *);
	    int (*open) (struct inode *, struct file *);
	    int (*flush) (struct file *, fl_owner_t id);
	    int (*release) (struct inode *, struct file *);
	    int (*fsync) (struct file *, loff_t, loff_t, int datasync);
	    int (*aio_fsync) (struct kiocb *, int datasync);
	    int (*fasync) (int, struct file *, int);
	    int (*lock) (struct file *, int, struct file_lock *);
	    ssize_t (*sendpage) (struct file *, struct page *, int, size_t,
                            loff_t *, int);
	    unsigned long (*get_unmapped_area)(struct file *, unsigned long,
                            unsigned long, unsigned long, unsigned long);
	    int (*check_flags)(int);
	    int (*flock) (struct file *, int, struct file_lock *);
	    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *,
                            loff_t *, size_t, unsigned int);
	    ssize_t (*splice_read)(struct file *, loff_t *,
                            struct pipe_inode_info *, size_t, unsigned int);
	    int (*setlease)(struct file *, long, struct file_lock **);
	    long (*fallocate)(struct file *file, int mode, loff_t offset,
			      loff_t len);
	    int (*show_fdinfo)(struct seq_file *m, struct file *f);
    };

W celu udostępnienia urządzenia znakowego na urządzeniu połączonym przez sieć kluczowe jest stworzenie "fałszywego" pliku urządzenia na komputerze klienckim, udostępnienie zestawu niezbędnych operacji plikowych i przekazanie wszystkich ich wywołań do maszyny posiadającej rzeczywiste urządzenie, a następnie przekazanie wyniku wywołania tej operacji z powrotem do maszyny klienckiej. Dokładniej ten proces będzie implementować ten projekt. *Przekazywanie operacji plikowych z jednej maszyny do drugiej będzie główną funkcją oprogramowania stworzonego w ramach tego projektu.*

Urządzenie znakowe nie musi jednak implementować wszystkich operacji. Przykładem operacji bezużytecznych dla plików urządzeń jest funkcja `readdir()`, która ma sens tylko i wyłącznie dla plików które są katalogami, oraz `sendpage()`, która przeznaczona jest raczej dla urządzeń sieciowych takich jak gniazda Unix. Inne operacje takie jak asynchroniczne `aio_read()` i `aio_write()` są rzadko implementowane dla urządzeń i prawie zawsze wartość ich wskaźnika wynosi `NULL` co sprawia że kernel po prostu używa zwykłych funkcji `read()` oraz `write()`.

Wirtualny system plików jest kluczową abstrakcją w zrozumieniu jak jądro linux udostępnia swoje urządzenia do przestrzeni użytkownika. Prawie każde urządzenie w systemie posiada swoją reprezentacje w systemie plików `/dev` i implementuje większość lub wszystkie z tych operacji. Oczywiście istnieje szereg innych metod komunikacji z procesami specyficznych dla konkretnego rodzaju urządzeń. Na przykład karty graficzne posiadają interfejs DRI(Direct Rendering Infrastructure) która omija interfejs operacji na plikach w celu osiągnięcia jak najwyższej wydajności w obróbce grafik. Na szczęście na potrzeby zwykłych urządzeń znakowych implementacja podstawowych operacji na plikach w zupełności wystarczy.

## Model sterowników urządzeń w jądrze Linux

Wszystkie urządzenia w systemie linux jak i sterowniki które implementują ich zarządzanie istnieją w jądrze w postaci dokładnie zdefiniowanych struktur, które razem tworzą model sterowników urządzań. W modelu tym istnieją cztery podstawowe struktury, bez wykorzystania których żadne urządzenie nie ma prawa istnieć.

* Szyna(Bus) - Szyna to kanał pomiędzy procesorem a urządzeniem lub wieloma urządzeniami. Na potrzeby modelu sterowników urządzeń linux wszystkie urządzenia, bez względu na to czy fizyczne czy wirtualne, muszą przynależeć do odpowiadającej im szyny. Przykładami szyn mogą być szyny PCI, USB czy I2C. Model ten reprezentuje rzeczywiste połączenia pomiędzy szynami a urządzeniami które one kontrolują. Szyna jest reprezentowana przez strukturę `bus_type`. Zawiera ona informacje o nazwie szyny, informacje o podłączonych urządzeniach, zbiór operacji które można na nich wykonać i wiele innych detali specyficznych dla konkretnych typów szyn.

* Klasa(Class) - Jest to struktura wyższego poziomu abstrakcji która nie skupia się na niskopoziomowych kwestiach implementacji, na przykład jak dokładnie jest ono podłączone do komputera, lecz na ogólnym rodzaju urządzenia i jakie operacje da się na nim wykonać. Przykładowe klasy urządzeń to urządzenia audio, sieciowe lub dyski. Klasa reprezentowana jest w jądrze przy pomocy struktury `class`.

* Sterownik(Driver) - Struktura ta opisuje i wskazuje na konkretny sterownik, również nazywany modułem, załadowany w systemie i pozwala na wykonywanie podstawowych operacji na sterownikach takich jak ich zatrzymywanie, uruchamianie, odpytywanie czy wyłączanie i usuwanie sterownika. Implementacja tego obiektu to struktura `device_driver`, która przechowuje takie informacje jak nazwa sterownika, prywatne dane sterownika czy rodzaj szyny, z której dany sterownik obsługuje należące do niego urządzenia.

* Urządzenie(Device) - Na prawie najniższym poziomie istnieje struktura urządzenia, która pozwala zdefiniować konkretną instancje obsługiwanego fizycznego lub wirtualnego urządzenia oraz plik reprezentujący je w wirtualnym systemie plików `/dev` (devfs). Zdefiniowany jako struktura o nazwie `device` zawiera duży zbiór informacji na temat urządzenia do którego jest przypisany, w tym urządzenie macierzyste, prywatne dane modułu sterownika, szynę do której jest przyłączony, sterownik który obsługuje dane urządzenie czy numery `major`(będziemy go nazywać numerem "dur") oraz `minor`(który nazywany będzie "moll"), które identyfikują wszystkie sterowniki i urządzenia w systemie.

Wszystkie te kluczowe struktury są zdefiniowane w pliku nagłówkowym `include/linux/device.h` wraz z dużą ilością komentarzy opisujących zwierające się w nich atrybuty, struktury oraz wskaźniki do funkcji. Wraz z wieloma innymi mniej ważnymi strukturami, które są zazwyczaj atrybutami tych kluczowych struktur, tworzą one model sterowników urządzeń w jądrze Linux.

Właściwie żadnej z tych struktur programista nie tworzy lub nie wypełnia własnoręcznie. Istnieje szereg metod opakowujących, z których należy korzystać jeżeli programista chce stworzyć lub wykonać jakiekolwiek operacje na którejkolwiek z tych struktur. Ma to na celu ograniczenie błędów płynących z wywłaszczania wątków jądra jak i problemów z kompatybilności w razie przyszłych zmian w definicjach którejkolwiek z tych kluczowych struktur.

Jednym z najważniejszych elementów identyfikujących urządzenia w systemie Linux wspomniany już w opisie struktury urządzenia jest para numerów dur oraz moll, znanych w angielskim jako `major` oraz `minor`. Numery te są zapisywane kodzie źródłowym jako pojedynczy typ danych `dev_t` który od prawie zawsze był definiowanych jako typ `unsigned long`. Pierwsze 12 bitów przeznaczone jest na numer dur a pozostałe 20 bitów na numer mol. Numer dur definiuje rodzaj sterownika a numer moll definiuje numer urządzenia obsługiwany przez dany sterownik. Wszystkie urządzenia w systemie posiadające swoją wirtualną reprezentacje w systemie plików `/dev` mają przydzielone numery dur oraz moll. Dobrym przykładem jest bardzo proste urządzenie generujące losowe dane znajdujące się pod nazwą `/dev/urandom`. Podejrzenie takiego urządzenia przy pomocy polecenia `ls -l` daje nam taki oto wynik:

    $ ls -l /dev/urandom
    crw-rw-rw- 1 root root        1,   9 Aug  7 21:34 urandom

W miejscu, w którym w przypadku normalnych plików pojawia się wielkość widnieją dwa oddzielone przecinkiem numery. Numer 1 to dur i identyfikuje on sterownik obsługujący urządzenia urandom a numer 9 to moll i identyfikuje on konkretne urządzenie obsługiwane przez ten sterownik. Listę sterowników urządzeń znakowych oraz blokowych i przypisanych do nich numerów dur można znaleźć odczytując wirtualny plik `/proc/devices`:

    $ cat /proc/devices
    Character devices:
      1 mem
      4 /dev/vc/0
      4 tty
      ...

Widać tutaj że sterownik odpowiedzialny za wirtualne urządzenie `/dev/urandom` nazywa się `mem`. Jest to zrozumiałe biorąc pod uwagę, iż jest do urządzenie zupełnie wirtualne, które w razie potrzeby na żywo generuje swoje dane ze źródeł entropii w jądrze Linux. Przy okazji warto wspomnieć, iż praktycznie wszystkie źródła entropii(losowości) w systemie Linux to urządzenia fizyczne. Na przykład używane są czasy przybycia nowych paczek do interfejsu sieciowego, a dokładnie pojawienia się przerwania IRQ związanego z danym urządzeniem w celu zwiększania entropii czyli losowości generowanych danych. Dokładniejszy opis można znaleźć w książce [Roberta Love][b12].

Ogólny wgląd w model sterowników urządzeń można znaleźć w plikach dokumentacji rozprowadzanych razem z kodem źródłowym jądra w folderze `Documentation/driver-model`. Jest to jednak bardzo pobieżne spojrzenie na elementy tworzące ten system i do prawidłowego jego zrozumienia niezbędne jest czytanie samego kodu źródłowego i ciągłe konsultowanie z dokumentacją oraz bardzo często listami mailowymi związanymi z rozwijaniem jądra Linux jako że jest to model wciąż rozwijany. Nic w jądrze Linux nie jest statyczne, i pomimo tego, że dla przeciętnego użytkownika sposób korzystania z systemu nie zmienia się zbytnio na przełomie lat pod maską trwają szeroko zakrojone prace mające na celu ulepszenie jądra Linux. Szybkość z jaką prowadzone są prace nad jądrem Linux jest jedną z jego ogromnych zalet, sprawia ona jednak, iż książki opisujące techniki programowania sterowników czy innych elementów jądra bardzo szybko stają się przestarzałe i w dużym stopniu bezużyteczne jako przewodnik dla potencjalnych przyszłych programistów jądra.

## Urządzenia znakowe

Urządzenia znakowe to jedne z najprostszych urządzeń jakie można znaleźć w systemie Linux. Główna różnica pomiędzy urządzeniami znakowymi a urządzeniami blokowymi wynika z użycia buforów i pamięci podręcznej oraz możliwość wykonywania operacji losowego dostępu(random access). Urządzenia znakowe nie korzystają z buforów ani pamięci podręcznej i nie udostępniają możliwości odczytywania czy zapisywania danych w dowolnie wybranych miejscach(wyjątkiem są taśmy). Urządzenia blokowe natomiast pozwalają procesom wybrać punkt, z którego będą odczytywać i do którego będą zapisywać dane oraz korzystają z pamięci podręcznej w celu zwiększenia wydajności ,jako iż częstym zjawiskiem jest wielokrotne odczytywanie lub zapisywanie tej samej lokalizacji w medium przechowującym dane.

Podstawowym obiektem przedstawiającym urządzenia znakowe w jądrze Linux jest struktura `cdev` zdefiniowany w pliku `include/linux/cdev.h`. Wygląda ona następująco:

    struct cdev {
	    struct kobject kobj;
	    struct module *owner;
	    const struct file_operations *ops;
	    struct list_head list;
	    dev_t dev;
	    unsigned int count;
    };

Jak prawie każda ważna struktura w jądrze posiada ona k-obiekt(kobject), który jest strukturą używaną przez jądro Linux do zarządzania i organizowania obiektów używanych w trakcie działania systemu. Można ten obiekt nazwać obiektem pomocniczym, ułatwia on na przykład lokalizowanie struktur na podstawie elementów, które się w nich zawierają. K-obiekty to złożony mechanizm kontroli wielu kluczowych obiektów istniejących w kodzie jądra Linux i pojawia się on w prawie każdym podsystemie jaki w nim istnieje. Dokładne wytłumaczenie tej struktury i jej działania wykracza znacznie poza zakres tego projektu i samo w sobie było by dobrym tematem na dość rozbudowaną pracę inżynierską.

Kolejnym atrybutem jest  wskaźnik do struktury `module` który wskazuje na moduł sterownika obsługujący dane urządzenie znakowe. Posiada on oczywiście strukturę `file_operations` którą już zidentyfikowaliśmy jako niezbędny element każdego sterownika który chce udostępnić systemowi urządzenie, które obsługuje. Wskaźnik ten jest wypełniany przez sterownik podczas inicjalizacji urządzenia i jądro używa tych operacji do nadpisania domyślnych operacji na plikach które przypisane są plikowi na podstawie konfiguracji sterownika systemu plików w jakim dany plik się znajduję. Struktura `list_head` jest pierwszym wskaźnikiem w dwukierunkowej liście obiektów i-węzłów czyli plików w systemie `/dev` połączonych z tym konkretnym urządzeniem znakowym a count to ilość elementów w liście i-węzłów. Atrybut `dev` zapisuje przedstawione wcześniej numery dur oraz moll, które w unikalny sposób identyfikują to urządzenia oraz jego sterownik.

Struktura ta nie jest jednak tworzona i wypełniana przez programistę. Jądro udostępnia szereg metod opakowujących, które gwarantują prawidłową obsługę błędów i kompatybilność na przestrzeni wielu wersji jądra. Oto lista najważniejszych z nich:

* `struct cdev *cdev_alloc(void);` - Służy do alokowania przestrzeni pamięci niezbędnej dla struktury `cdev`. Zaleca się używania tej funkcji z uwagi na to, iż poza alokowaniem pamięci inicjalizuje ona również k-obiekt zawarty w `cdev`.

* `void cdev_init(struct cdev *, const struct file_operations *);` - Odpowiada za inicjalizację struktury `cdev` przy pomocy wcześniej przygotowanej struktury reprezentującej pełen zestaw operacji plikowych implementowanych przez dany sterownik. Wszystkie sterowniki urządzeń znakowych wywołują tę funkcje z celu przygotowania do użycia struktury `cdev`, która zazwyczaj istnieje jako atrybut jakiejś wewnętrznej struktury używanej przez sterownik do przechowywania danych o implementowanym urządzeniu. Również inicjalizuje k-obiekt.

* `int cdev_add(struct cdev *, dev_t, unsigned);` - Dodaje urządzenie reprezentowane przez strukturę `cdev` do systemu. Funkcja ta powinno zostać wywołana wtedy i tylko wtedy gdy sterownik wykonał pomyślnie wszystkie operacje przygotowujące urządzenie do obsługi operacji, które można na nim wykonać. Po poprawnym powrocie z tej funkcji urządzenie jest uznawana za gotowe do pracy. Dodatkowo zwiększa licznik referencji zawarty w k-obiekcie.

* `void cdev_del(struct cdev *);` - Usuwa urządzenie z systemu oraz zmniejsza licznik referencji zawarty w k-obiekcie. W efekcie może to spowodować zwolnienie pamięci przeznaczonej dla struktury gdy licznik referencji osiągnie 0.

Funkcje te są zdeklarowane w pliku `include/linux/cdev.h` oraz zdefiniowane w pliku `fs/char_dev.c`. Poza wywołaniem funkcji `cdev_alloc()`, `cdev_init()` oraz `cdev_add()` w odpowiedniej kolejności sterownik nie musi w żaden sposób operować na strukturze `cdev` aż do momentu, w którym gotowy jest usunąć urządzenie i wywołuje `cdev_del()`. Poza tym struktura `cdev` pozostaje dostępna raczej jako repozytorium informacji o urządzeniu raczej niż aktywnie modyfikowany obiekt w trakcie działania urządzenia.

W celu przechowywania dokładniejszych danych o naszym urządzeniu sterownik będzie musiał stworzyć własną wewnętrzną strukturę reprezentującą urządzenie. Zostanie to opisane w rozdziale "[Kluczowe struktury danych]".

## Model oprogramowania

W tej chwili powinno już być jasne że na projekt będzie musiał składać się moduł sterownika jądra Linux, który będzie odpowiedzialny za stworzenie urządzenia-atrapy i odbieranie operacji plikowych na nim wykonanych, następnie i przekazywanie ich do prawdziwego urządzenia na zdalnej maszynie, która je rzeczywiście posiada i oczekiwanie na odpowiedź. Jednak z uwagi na na wiele decyzji podjętych podczas tworzenia jądra Linux wielu programistów jądra od razu zwróciło by uwagę,  iż nie należy robić w przestrzeni jądra wielu rzeczy, które wydają się jak najbardziej naturalne w przestrzeni użytkownika. Dwa takie przykłady to otwieranie i czytanie plików oraz wykonywanie połączeń sieciowych w kodzie jądra.

Istnieje wiele powodów dla których nie należy robić tych rzeczy z przestrzeni jądra. Po pierwsze czytanie plików czy odbieranie danych z gniazda wymaga konwertowania tych danych do formatu używalnego przez kod jądra. W wielu przypadkach jest to wyzwanie, które jest bardzo podatne na błędy w postaci rezerwowania zbyt dużych lub małych przestrzeni pamięci w trakcie tego procesu lub używanie wartości, które nie zostały poprawnie sprawdzone, i które mogą prowadzić do wykraczania poza przydzieloną nam pamięć. W przypadku plików problemem jest również lokalizacja pliku a w przypadku połączeń sieciowych odnalezienie poprawnego adresu, co bardzo często kończy się na statycznym ustawieniu ścieżek lub adresów w kodzie jądra. Spowoduje to potrzebę przekompilowania jądra za każdym razem gdy ta lokalizacja lub adres się zmieni.

Dodatkowym problemem jest zagrożenie bezpieczeństwa jądra. Pobieranie danych prosto ze zdalnej lokalizacji do jądra jest wręcz jawnym zaproszeniem dla wszelkiego rodzaju złośliwych użytkowników sieci do próby wykorzystania naszego modułu w celu przejęcia kontroli nad naszym systemem. Jądro powinno być ostatnim bastionem bezpieczeństwa w systemie operacyjnym i wystawianie go dla publicznego dostępu przez sieć jest bardzo złą praktyką. Popełnienie takiego błędu jest wręcz gwarancją ze kod nie zostanie przyjęty do projektu jądra Linux i najprawdopodobniej programista taki zostanie pouczony na temat podstawowych zasad projektowania systemów operacyjnych.

Z uwagi na te obiekcje projekt musi zostać podzielony na dwa odrębne elementy. Przede wszystkim moduł jądra odpowiedzialny za stronę sprzętową oraz tworzenie urządzenia atrapy i odbieranie operacji plikowych na nim wykonanych. Drugim elementem będzie program przestrzeni użytkownika odpowiedzialny za wczytanie odpowiedniej konfiguracji, połączenie się z modułem jądra oraz zdalną maszyną i ustanowieniem połączenia pomiędzy oboma końcami transakcji. Dzięki takiemu modelowi wszystkie problemy związane z wczytywaniem konfiguracji, kontrolą modułu oraz bezpieczeństwem zostaną przeniesione do warstwy użytkownika co powinno znacznie uprościć kod modułu i przyspieszyć jego działanie oraz sprawić że końcowy produkt będzie bardziej elastyczny w użytkowaniu i konfiguracji. Poniżej przedstawiam prosty diagram opisujący przykładowe połączenie pomiędzy urządzeniem fizycznym udostępniającym swoje zasoby na maszynie serwerowej a urządzeniem-atrapą udającym rzeczywiste urządzenie na maszynie klienckiej:

                            Fizyczne urządzenie
                                    |
                                Moduł jądra
                                    |
                                    |- Połączenie Netlink
                                    |
                                  Serwer
                                    |
                                    |- Połączenie TCP/IP
                                    |
                                  Klient
                                    |
                                    |- Połączenie Netlink
                                    |
                                Moduł jądra
                                    |
                            Urządzenie-atrapa

Przyjmując taki model organizacji rozszerza projekt o dodatkową warstwę komunikacji pomiędzy przestrzenią jądra a przestrzenią użytkownika co w wyraźny sposób komplikuje proces komunikacji pomiędzy urządzeniem-atrapą a rzeczywistym fizycznym urządzeniem. Jest to jednak utrudnienie niezbędne biorąc pod uwagę polisy jakie rządzą rozwojem kodu jądra Linux. Jako że chcemy udostępniać urządzenia fizyczne za pośrednictwem tego oprogramowania kluczowa jest wysoka przepustowość w przesyłaniu dużych ilości danych i niezawodność, która zapewni że żadna operacja na pliku nie zostanie pominięta podczas przesyłania. Dokładny opis wyboru metody komunikacji znajduje się w rozdziale "[Komunikacja z przestrzenią użytkownika]".

Dodatkowo w celu zmniejszenia nakładu pracy kosztem małego zwiększenia złożoności kodu podjęta została decyzja aby zaimplementować jeden moduł jądra, który będzie odpowiedzialny za urządzenie atrapę po stronie klienta oraz rzeczywiste urządzenie po stronie serwera i jeden program przestrzeni użytkownika, który analogicznie będzie wypełniał rolę serwera oraz klienta jednocześnie. Powinno to znacznie zmniejszyć ilość linijek kodu niezbędnych do ukończenia tego projektu. Powinno to również uprości konfiguracje dla potencjalnych użytkowników z uwagi na prosty podział oprogramowania na moduł jądra oraz serwer.

# Decyzje projektowe

## Wybór metod komunikacji

Model jaki został przyjęty narzuca wybór dwóch konkretnych metod komunikacji. Jednej dla komunikacji przestrzeni użytkownika z przestrzenią jądra w celu połączenia procesu serwera z załadowanym modułem jądra oraz drugiej dla komunikacji sieciowej pomiędzy dwoma instancjami procesu serwera na dwóch zdalnych maszynach. Kolejne dwa rozdziały rozważają dostępne alternatywy i wybierają metoda która najlepiej pasuje do tego projektu.

### Komunikacja z przestrzenią użytkownika

Ponieważ tworzenie tworzenie nowych urządzeń-atrap oraz wybieranie zdalnej maszyny do której należy się podłączyć jak i samo zadanie przesyłania operacji po sieci musi być wykonane w przestrzeni użytkownika moduł jądra musi implementować jakąś metodę komunikacji pomiędzy przestrzenią jądra a przestrzenią użytkownika. Jądro Linux udostępnia kilka możliwości prowadzenia takiej komunikacji. Mają one swoje wady i zalety, kluczowym jest więc wybranie tej, która zapewni wystarczające przenośność, przepustowość, rozszerzalność oraz szybkość w informowaniu o nowych zdarzeniach w systemie.

* Przenośność oznacza spójną strukturę danych na przestrzeni wielu różnych platform. Podstawowym problemem w komunikacji pomiędzy przestrzenią użytkownika a jądra może być na przykład wielkość słowa maszynowego które może się zmieniać w zależności od architektury na której się pracuje. Architektura x86_64 pozwala na używane 32 bitowych słów w przestrzeni użytkownika w czasie gdy słowo w przestrzeni jądra może mieć 64 bity. Tego typu różnice są kluczowe podczas wymiany danych takich jak wskaźniki czy wartości `long`.

* Przepustowość decyduje o tym jak szybko i jak duże ilości danych mogą być przekazywane pomiędzy przestrzeniami użytkownika i jądra. W przypadku pojedynczych opcji nie jest to problemem jednak istnieje wiele podsystemów jądra które wymagają transferów znacznych ilości informacji w celu ich konfiguracji. Dobrym przykładem jest system zapory ogniowej w jądrze Linux, zaawansowany routing oraz narzędzia do zarządzania kluczami w protokole IPsec.

* Rozszerzalność zapewnia, że zmiany w formacie przekazywanych danych nie spowodują problemów ze wsteczną kompatybilnością ze starszymi wersjami tego samego oprogramowania. Używanie statycznych struktur lub formatów podobnych do SVC, w których poszczególne dane są po prostu rozdzielone przecinkami lub innymi znakami są trudne w modyfikowaniu bez powodowania poważnych problemów w w poprzednich wersjach. Z uwagi na to dodawanie nowych funkcjonalności może powodować wiele problemów.

* Szybkość w informowaniu o zdarzeniach zazwyczaj oznacza, iż program w przestrzeni użytkownika nie jest zmuszony nieprzerwanie odpytywać jakiś element systemu w celu uzyskania informacji o zmianie stanu. Budzony jest on sam przez jądro w celu poinformowania o zmianie stanu w urządzeniu lub module, o którym informacje próbuje uzyskać dana aplikacja przestrzeni użytkownika.

Interfejsy udostępnione przez jądro można podzielić na trzy podstawowe kategorie. Z czego pierwsza jest tak kluczowa, iż jest ona podstawą dla wszystkich pozostałych. Najbardziej pierwotnym i podstawowym sposobem komunikacji z jądrem są wywołania systemowe. System Linux implementuje ich w obecnej chwili 244 plus 16 zależnych od architektury i liczba ta nie zmienia się często z uwagi na to, iż większość wywołań systemowych było zaprojektowanych zmyślą o wypełnianiu wielu ról jednocześnie. Większość programistów jądra stroni od dodawania nowych wywołań z uwagi na to, iż dotychczas istniejące so prawie w każdym przypadku wystarczające do każdego możliwego zastosowania oraz ponieważ raz dodane wywołanie otrzymuje swój numer identyfikacyjny który musi być umieszczonych w dwóch kluczowych plikach: `arch/sh/kernel/syscalls_32.S` oraz `include/uapi/asm-generic/unistd.h`, i raz tam dodany nie może być usunięty. Numer ten na zawsze odpowiada temu wywołaniu i gdyby przestał być implementowany musi pozostać wśród definicji i zwracać odpowiedni kod błędu w przypadku jego użycia. Prawdą jest że żadna inna forma komunikacji z jądrem nie jest tak szybka, co sprawia, iż jest to kuszące rozwiązanie, jednak z uwagi na wymienione zastrzeżenia tworzenie własnych wywołań systemowych i tym jak niskopoziomowa byłą by taka zmiana w jądrze jest prawie zawsze złym pomysłem i powinno być używane w absolutnej ostateczności.

Pozostałe dwie kategorie to wirtualne systemy plików takie jak `/proc` i `/sys` oraz standardowe gniazda BSD. Wszystkie korzystają z już omówionych wywołań systemowych i implementują istniejące modele używane do komunikacji z plikami na dyskach oraz z gniazdami sieciowymi. W przypadku systemu plików `/proc` dane pobierane są z plików w postaci czytelnej dla człowieka ale mało praktycznej dla programu, który musi przeprowadzić analizę składniową danych w celu otrzymania jakichkolwiek informacji. To samo tyczy się wysyłania danych do jądra przez pliki znajdujące się w systemie `/proc`. Dodatkowo transfer danych jest ograniczony do jednej strony pamięci oraz nie udostępnia żadnego mechanizmu do informowania programu o zdarzeniach w jądrze. Wielkość strony pamięci w jądrze jest definiowana przy pomocy makra `PAGE_SIZE` i zazwyczaj wynosi 8192 bajtów co będzie raczej niewystarczające w przypadku przesyłania większych ilości danych. Oryginalnie system `procfs` miał być używany jedynie w celu przekazywania informacji na temat istniejących procesów, niestety w obecnej chwili jest on nadużywany do wielu innych zdań i przez cały czas trwają pracę nad przeniesieniem całej funkcjonalności do systemu plików `/sys` który udostępnia kilka usprawnienie w stosunku do jego poprzednika.

Wirtualny system plików znajdujący się w katalogu `/sys` znany również jako `sysfs` ma znacznie bardziej logiczną strukturę która pozwala na eksportowanie obiektów z przestrzeni jądra jako katalogów, ich atrybutów jako plików oraz relacji z innymi obiektami jako dowiązań symbolicznych. Posiada on również sygnalizację o wydarzeniach zaimplementowaną przy pomocy gniazd Netlink, o których będzie mowa w następnym paragrafie. Niestety system `sysfs` nadal ogranicza transfer danych do jednej strony pamięci oraz format danych potrafi zmieniać się pomiędzy wersjami jądra więc nie jest to stabilny sposób przekazywania informacji do oraz z przestrzeni użytkownika.

Metody komunikacji oparte na gniazdach BSD udostępniają znacznie bardziej przyjazny interfejs do komunikacji z przestrzenią jądra. Bardziej prymitywną metodą z dwóch dostępnych jest używanie tak zwanych surowych gniazd typu `AF_RAW` które pozwalają na przesyłanie struktur danych o stałym formacie oraz korzystania ze specjalnego wywołania systemowego ioctl które przyjmuje jako argument dwie wartości typu `long`, pierwszą definiującą typ komunikatu i drugą wartość samego komunikatu. Jest to nadal bardzo ograniczone jeżeli chodzi o możliwość modyfikowania i rozwijania metody komunikacji ale powala jako pierwsza metoda na przysyłanie dużych ilości danych czego nie można powiedzieć o metodach komunikacji opartych na wirtualnych systemach plików.

Ostatnią do omówienia i coraz powszechniej używaną metodą jaką udostępnia jądro linux jest protokół Netlink oparty na gniazdach BSD. Jest to bezpołączeniowy protokół komunikacji opierający się na datagramach przypominający UDP, który pozwala na transfer dużych ilości danych, zapewnia taki sam poziom przenośności jak każdy inny protokół sieciowy oraz pozwala na łatwe rozwijanie metod komunikacji w miarę rozwoju projektu. Kluczowym atrybutem protokołu Netlink jest stały format zapewniający jasny podział na wiadomości, które wyróżniają się własnym numerem sekwencyjnym, typem komunikatu, zmienną długością, numerem identyfikującym proces źródłowy oraz zestawem specjalnych flag. Flagi obecne w każdym datagramie Netlink pozwalają na dodatkową implementacje niezawodności poprzez przesyłanie wiadomości potwierdzających otrzymanie przekazu pozbywając się tym sposobem ograniczeń wynikających z niepołączeniowej natury protokołu.

Biorąc pod uwagę wszystkie możliwości protokołu Netlink jest on najlepszą opcją jeżeli chodzi o implementowanie sprawnej metody komunikacji, która pozwala na szybki transfer dowolnej ilości danych oraz łatwą identyfikacje poszczególnych wiadomości i ich typu. Aplikacje w przestrzeni użytkownika mogą korzystać z protokołu netlink tak samo jak robią to w przypadku innych protokołów sieciowych. Jedyną różnicą jest potrzeba wywołania funkcji `socket()` z rodziną gniazda ustawioną na `AF_NETLINK` oraz typem protokołu na `SOCK_RAW` i używaniem struktury `nlmsghdr` jako podstawy budowania każdego komunikatu przesyłanego przez gniazdo Netlink.

Oto struktura `nlmsghdr` niezbędnego nagłówka każdej wiadomości Netlink:

    struct nlmsghdr {
        __u32 nlmsg_len;   /* długość wiadomości wliczając nagłówek */
        __u16 nlmsg_type;  /* typ wiadomości                        */
        __u16 nlmsg_flags; /* dodatkowe flagi do kontroli przepływu */
        __u32 nlmsg_seq;   /* numer sekwencyjny dla wiadomości      */
        __u32 nlmsg_pid;   /* identyfikator procesu źródłowego      */
    };

Struktura ta pozwala na łatwą organizacje własnego protokołu komunikacji przy użyciu indywidualnych wiadomości o konkretnym rodzaju, numerze identyfikacyjnym, adresie źródłowym oraz zestawie flag które pozwalają na rozdzielanie dużych wiadomości na części lub żądanie o wiadomości potwierdzające fakt odebrania przekazu po drugiej stronie pary gniazd.

Dodatkowo pokrewieństwo gniazd Netlink z gniazdami TCP/IP jest kolejnym ułatwienie pozwalającym na łatwe przekazywanie komunikatów odebranych z jednego rodzaju gniazda do drugiego. Powinno to znacznie uprościć implementacje procesu serwera odpowiedzialnego za przekazywanie wszystkich operacji plikowych wywołanych w module jądra do zdalnej maszyny posiadającej fizyczny egzemplarz udostępnianego urządzenia. Jest to idealne rozwiązanie na potrzeby tego projektu.

### Komunikacja ze zdalną maszyną

Istnieją dwa podstawowe typy protokołów internetowych: TCP oraz UDP. Oba spełniają kluczowe role i oba mają swoje wady. Wybranie odpowiedniego protokołu na potrzeby tego projektu będzie miało bezpośredni wpływ na niezawodność i wydajność oprogramowania.

TCP to akronim z języka angielskiego: "Transmission Control Protocol", i jest protokołem połączeniowym, co oznacza, iż obie strony połączenia utrzymują aktywne sesje tak długo jak trwa połączenia które zapewniają między innymi dostarczenie wysłanych wiadomości i ponowne ich przesyłanie w razie problemów z połączeniem oraz otrzymanie ich w poprawnej kolejności. TCP gwarantuje że dane wysłane przez jedną stronę połączenia dotrą w całości i w poprawnej kolejności albo nie dotrą w ogóle co zazwyczaj spowoduje utratę połączenia i zakończenie sesji. Dodatkowo lub może przede wszystkim TCP pozwala na kontrole szybkości przesyłania danych poprzez modyfikowanie wielkości tak zwanego "okna". TCP na bieżąco kontroluje jak szybko druga strona jest w stanie odbierać dane, które są przesyłane i modyfikuje wielkość "okna" w celu dopasowania szybkości wysyłania danych do szybkości z jaką drugi koniec połączenie jest w stanie jest odbierać.

Nadaje się on doskonale do zastosowań, w których niedopuszczalne jest utracenie części lub całości pojedynczych pakietów z transmisji. Dzięki kontroli przepływu danych TCP daje programiście pewność, iż dane dotrą nienaruszone lub program zostanie poinformowany przez odpowiedni kod błędu o utracie połączenia. Przykładowe protokoły warstwy aplikacji, które korzystają z TCP to HTTP, SMTP, IMAP, SSH, FTP i wiele wiele innych.

Niestety protokół TCP w celu utrzymania swojego standardu niezawodności dodaje zauważalny koszt do komunikacji. Nie tylko w formie dość dużego nagłówka dodawanego do każdego pakietu TCP ale również w postaci opóźnień spowodowanych potrzebą ustanawiania sesji przez zestaw pakietów kontrolnych w tak zwanym trzy stopniowym uścisku ("3-way handshake") oraz potrzeby odsyłania potwierdzeń o odebranych danych. Wszystkie te koszta sprawiają, iż TCP nie nadaje się do zastosowań, w których bardziej ceni się działanie w czasie rzeczywistym.

Między innymi z tego powodu powstał protokół UDP, z angielskiego "User Datagram Protocol". W przeciwieństwie do TCP nie gwarantuje on praktycznie niczego programiście poza tym że będzie szybki. Nie posiada on żadnej kontroli przepływu danych, poprawności danych, sprawdzania kolejności w jakiej dane się pojawiają u odbiorcy czy metody potwierdzenia odbioru. W skrócie UDP jest protokołem bezpołączeniowym. Daje to jednak szereg zalet, które są kluczowe dla aplikacji mających do czynienia z danymi, które bardzo szybko tracą ważność. Wiele usług przesyłających obraz wideo lub komunikacje głosową korzysta z UDP z tego właśnie powodu. Aplikacje pozwalające na prowadzenie telekonferencji znacznie bardziej traciły by na używalności gdyby wszystkie dane docierały z prawie sekundowym opóźnieniem niż gdyby pare klatek na minutę zostało upuszczonych w trakcie przesyłania.

Problem z UDP polega na tym, iż sama aplikacja jest teraz odpowiedzialna za obsługiwanie otrzymanych danych w poprawnej kolejności lub weryfikowanie czy przesłane dane nie są uszkodzone. Bardzo często robienie tego jest bardziej kosztowne niż było by po prostu użycie bardziej bezpiecznego protokołu TCP. Dodatkowo brak kontroli szybkości przepływu danych może łatwo spowodować przesycenie przepustowości sieci, z której korzysta aplikacja i doprowadzenie jej do stanu, w którym jest nieużywalna.

Pomimo tego że UDP jest powszechnie używane w wielu protokołach kluczowych dla działania sieci lokalnych oraz całego internetu takich jak DNS, DHCP, SNMP czy wiele protokołów dynamicznego routingu to wielu programistów odradza używania UDP w aplikacjach użytkowych. Istnieje tylko kilka konkretnych przykładów, w których szybkość komunikacji jest tak kluczowa, iż UDP jest naprawdę jedynym sensownym wyjściem. Głównym zastosowaniem jest komunikacja wideo oraz audio między ludźmi w czasie rzeczywistym. W większości innych przypadków UDP powoduje więcej problemów niż rozwiązuje.

Z uwagi na to podjęta została decyzja by użyć protokołu TCP w implementacji serwera który będzie przekazywał operacje na plikach urządzeń znakowych. Ponieważ absolutnie kluczowa jest kolejność w jakiej wykonywane są operacje na urządzeniach fizycznych i pominięcie jednej lub więcej operacji może mieć absolutnie katastrofalne skutki gwarancja jaką daje TCP jeżeli chodzi o dostarczenie wysłanych wiadomości jest niezbędna do poprawnego działania tego oprogramowania.

## Rozwiązanie problemu serializacji

Każdy programista piszący programy korzystające z jakiegokolwiek protokołu komunikacji sieciowej musi dobrze zdawać sobie sprawę z problemu serializacji danych przy ich wysyłaniu oraz odbieraniu. Problem serializacji wynika z kilku kwestii. Jednym z głównych czynników jest kolejność bajtów(ang. Endianess) w liczbach całkowitych czy zmiennoprzecinkowych, których wielkość wynosi przynajmniej dwa bajty. Główne dwa rodzaje kolejności bajtów to kolejność Big-endian, w której ważniejszy bajt jest zapisywany w komórce pamięci o najniższym adresie oraz Little-endian, w którym najmniej ważny bat jest zapisywany w komórce o najniższym adresie. Kolejność ta zależy od platformy i jej architektury na jakiej dany program jest uruchamiany.

Obecnie najpopularniejsze architektury takie jak x86 czy x86-64 wykorzystują kolejność little-endian. Big-endian jest raczej formatem używanym w systemach wbudowanych lub niektórych urządzeniach przenośnych takich jak komórki. Układ Big-endian jest również znany jako "konwencja Motoroli" jako że ich procesory jako jedne z pierwszych używały układu Big-endian na dużą skalę. Inne przykłady to coraz rzadziej spotykane systemy IBM POWER lub IBM System 360 i 370. Dodatkowo większość popularnych architektur takich jak MIPS, SPARC, ARM, czy PowerPC pozwalają zmieniać kolejność bajtów podczas uruchamiania systemu(zmiana nie jest możliwa w trakcie działania systemu).

Dodatkowym problemem jest kwestia różnej wielkości podstawowych typów danych takich jak `int` czy `long` pomiędzy platformami. Najbardziej kluczowa jest ta różnica pomiędzy platformami 32 i 64 bitowymi które może zmienić długość takich typów jak `long` który musi być przynajmniej 32 bitowy ale może być większy.

Jądro Linux praktycznie od zawsze używa układu bajtów little-endian i większość ważnych typów danych w systemie jest definiowanych w statyczny sposób jako typy o konkretnej ilości bajtów przy pomocy makr takich jak na przykład `__kernel_uid32_t`, które gwarantują typ danych o długości 32 bitów. Przykładem takiego typu może być powszechnie używany `size_t` lub `loff_t`. Dodatkowo w przestrzeni użytkownika można korzystać z typów danych takich jak `int64_t` oraz `int32_t` które gwarantują odpowiednią długość bitową zmiennych użytych do przesyłania danych niezależnie od platformy. Z uwagi na to, iż projekt może być używany jedynie na platformach z systemem Linux praktycznie gwarantowany jest układ little-endian oraz wielkość używanych podstawowych typów danych, podjęta została decyzja by w dużym stopniu zignorować problem serializacji danych na potrzeby komunikacji pomiędzy maszynami na rzecz uproszczenia kodu i zmniejszenia nakładu pracy. Szansa użycia tego oprogramowania na systemie o innej charakterystyce jest nadal istnieje ale nie jest to bardzo prawdopodobne.

W przyszłości możliwe będzie rozszerzenie kodu o warstwę zapewniającą poprawną serializacje wszystkich danych przed przesłaniem ich z jednego modułu jądra za pośrednictwem programu serwera do drugiego modułu jądra na zdalnej maszynie.

## Sposób implementacji programu serwera

Z uwagi na to że podjęta została decyzja aby zamknąć funkcjonalność serwera oraz klienta w jednym programie przestrzeni użytkownika, który nazywać będziemy demonem, kluczowe jest rozdzielenie poszczególnych funkcji pomiędzy odrębne procesy. W przypadku normalnych programów udostępniających różnego rodzaju usługi rozsądnym pomysłem jest użycie wątków jako że zarządzanie nimi jest dość proste dzięki bibliotece wątków POSIX zwanej [pthread][b16]. Niestety specyfika naszego programu wynikająca z potrzeby komunikowania się z przestrzenią jądra za pośrednictwem gniazd Netlink sprawia że niezbędnym jest aby wątki komunikujące się z jądrem posiadały swój własny odrębny identyfikator procesu(PID). W przypadku systemów GNU/Linux wątki współdzielą identyfikator procesu z procesem macierzystym, który jest stworzył. Z tego powodu użycie wątków POSIX jest niemożliwe.

Jednak fakt że wiadomości Netlink określają swój cel oraz źródło na podstawie identyfikatorów procesów może być znacznym ułatwieniem w implementacji programu demona. Tworząc nowy proces oraz nowe gniazdo Netlink na potrzeby komunikacji z jądrem dla każdego obsługiwanego urządzenia pozbywamy się potrzeby pisania kodu, który musiał by rozróżniać pomiędzy tym, z którego urządzenia pochodzi dany komunikat jeżeli użyty było by tylko jeden proces i jedno gniazdo Netlink. Ponieważ wcześniej opisany nagłówek wiadomości Netlink zawiera w sobie jedynie zmienną definiującą rodzaj komunikatu o nazwie `nlmsg_type`, który programista może użyć w implementacji własnego protokołu komunikacji proces demona musiał by zaglądać do samego ładunku(payload) danej wiadomości w celu zweryfikowania z jakiego urządzenia ona pochodzi. Przydzielając jeden proces do jednego urządzenia tożsamość ta jest oczywista.

Sprowadza to nas na właściwą ścieżkę w projektowaniu programu demona. Potrzebuje on jeden główny proces, który będzie oczekiwał na nowe połączenia od innych demonów na zdalnych maszynach nasłuchując na odpowiednim porcie TCP, jeden nowy proces na potrzeby każdego połączenia od klienta w celu obsłużenia go, czyli dla roli serwera, oraz nowy proces na potrzeby tworzenia połączeń z demonem na maszynie, która posiada rzeczywiste urządzenie, czyli dla roli klienta.

Daje to nam dwie kategorie procesów:

* Proces nasłuchujący(Listener) - Przy starcie tworzy wszystkie procesy klienckie a następnie zaczyna odbierać nowe połączenia i tworzy procesy do ich obsługi.
* Proces pośredniczący(Proxy) - Działa w roli klienta lub serwera i przekazuje wiadomości z jądra do zdalnej maszyny i wiadomości ze zdalnej maszyny do jądra.

## Zabezpieczenia przed paniką jądra

## Problem wywłaszczania oraz współdzielenia zasobów

# Implementacja (70%)

## Podział kodu

Z uwagi na to że projekt jest podzielony na dwa podstawowe elementy, moduł jądra oraz serwer, ważne jest aby rozdzielić ich kod w logiczny sposób aby ułatwić organizacje pracy oraz kompilacje.

Całość rozdzielona jest na trzy foldery:

* `include` - Katalog ten zawiera w sobie pliki nagłówkowe, które definiują wartości niezbędne do kompilacji modułu jądra oraz programu serwera. W obu przypadkach plik `Makefile` zawiera dodatkowy wpis włączający ten katalog do procesu kompilacji
* `kernel` - Tutaj znajduje się cały kod modułu jądra oraz plik `Makefile` definiujący proces kompilacji tego modułu.
* `server` - Posiada wszystkie pliki źródłowe składające się na program serwera oraz plik `Makefile` pozwalający go zbudować. Tutaj znajduje się również przykładowy plik `server.conf` przedstawiający sposób konfiguracji programu.

Dodatkowo w głównym katalogu znajduje się plik `README.md` napisany w formacie [Markdown][b15] dający potencjalnym użytkownikom podstawowe informacje na temat kompilacji oraz użytkowania oprogramowania.

### Kod modułu jądra

Kod modułu jest podzielony na szereg plików źródłowych na podstawie ich funkcji. Oto prosty opis każdego z nich:

* `dbg.h` - Definiuje makro debug() używane podczas rozwijania kodu.
* `netdev.h` - Definicje wszystkich kluczowych stałych i limitów w kodzie modułu.
* `main.c` - Pierwszy i ostatni kod uruchamiany w trakcie działania modułu czyli funkcje `init` oraz `exit`.
* `fo.c, fo.h` - Deklaruje główną strukturę `file_operations` używaną do tworzenia wszystkich urządzeń-atrap.
* `netdevmgm.c, netdevmgm.h` - Kod zarządzający każdym urządzeniem atrapą oraz serwerem.
* `netlink.c, netlink.h` - Kod odpowiedzialny za odbieranie i wysyłanie danych  przez gniazdo netlink.
* `fo_access.c, fo_access.h` - Definiuje kod zarządzający jednym procesem używającym jednego z urządzeń-atrap.
* `fo_comm.c, fo_comm.h` - Kod, który zarządza protokołem komunikacji pomiędzy serwerem a klientem.
* `fo_recv.c, fo_recv.h` - Funkcje wykonujące operacje plikowe na rzeczywistym urządzeniu po stronie serwera.
* `fo_send.c, fo_send.h` - Funkcje odbierające operacje plikowe na urządzeniu-atrapie.

### Kod serwera

Tak samo jak moduł jądra, kod serwera jest również podzielony na poszczególne pliki w celu łatwiejszej organizacji:

* `netprotocol.h` - Definiuje format nagłówka każdej wiadomości przesłanej pomiędzy klientem a serwerem.
* `main.c` - Kod, który analizuje argumenty z konsoli, wczytuje plik konfiguracji oraz uruchamia połączenia klienckie zdefiniowane w pliku konfiguracyjnym oraz nasłuchiwanie nowych połączeń.
* `conn.c, conn.h` - Wszystko odpowiedzialne za komunikacje pomiędzy klientem a serwerem.
* `netlink.c, netlink.h` - Wszystko odpowiedzialne za komunikację z modułem jądra przez gniazdo Netlink.
* `proxy.c, proxy.h` - Główny kod pętli odbierający komunikaty przesyłane pomiędzy klientem a serwerem.
* `signal.c, signal.h` - Kod odpowiedzialny za obsługiwanie sygnałów systemowych w bezpieczny sposób.

### Wspólne nagłówki

Jedynym wspólnym plikiem nagłówkowym jest plik `include/protocol.h` definiujący szereg stałych wartości, które mają bezpośredni wpływ na sposób komunikacji modułów z programami serwerowymi oraz pomiędzy serwerem oraz klientem. Między innymi definiuje on domyślny numer portu serwera przy pomocy definicji makra `NETDEV_SERVER_PORT` ustawionego na wartość 9999 oraz zestaw rodzajów komunikatów jakie mogą być przesyłane jako makra zaczynające się od `MSGT`.

Wprowadzając jakiekolwiek zmiany do wartości zdefiniowanym w tym pliku należy zwiększyć wartość `NETDEV_PROTOCOL_VERSION` o jeden aby zapobiec błędom spowodowanym przez komunikację serwera i klienta o różnych wersjach protokołu.

## Budowanie i uruchamianie kodu

### kbuild

Powszechnie rozprowadzane kopie jądra Linux posiadają wbudowany system kompilowania i budowania jądra oraz ich modułów zwany `kbuild`. Jego dość obszerna dokumentacja znajduje się w folderze `Documentation/kbuild`. Z punktu widzenia projektu kluczową informacją jest sposób budowania modułów jądra. Znajduje się ona w pliku `Documentation/kbuild/modules.txt`.

System `kbuild` zbudowany jest na podstawie narzędzia [GNU Make][b14] pozwala na zamknięcie wszystkich informacji niezbędnych do zbudowania modułu jądra w pliku `Makefile`. Jedyne co jest potrzebne do jego działania to już skompilowany obraz jądra oraz pliki nagłówkowe użyte podczas jego kompilacji. System ten pozwala na kompilowania modułu zarówno bezpośrednio w drzewie jądra jak i w zupełnie odrębnym folderze. Kluczowe jest jedynie posiadanie kodu oraz obrazu jądra w folderze `/usr/src/linux`.

Format plików `Makefile` używanych przez system `kbuild` jest bardzo uproszczony i nie musi zawierać normalnych wpisów definiujących cele kompilacji oraz ich zależności wraz z poleceniem, który zbuduje dany cel. Zamyka on wszystkie niezbędne informacje w dwóch podstawowych zmiennych. Oto lekko okrojony plik `Makefile` użyty do kompilacji modułu składającego się na projekt:

    DEBUG 		:= -g
    WARN   		:= -Wall -Wstrict-prototypes -Wmissing-prototypes
    NOWARN 		:= -Wno-unused-function
    INCLUDE 	:= -I$(src)/../include/
    ccflags-y 	:= -O ${WARN} ${NOWARN} ${DEBUG} ${INCLUDE}

    obj-m       := netdev
	netdev-objs := fo_access.o fo_send.o fo_recv.o fo_comm.o fo.o
                   netdevmgm.o netlink.o main.o

Kluczowymi zmiennymi są `obj-m` oraz `netdev-objs`. Zmienna `obj-m` definiuje główny cel kompilacji czyli nasz końcowy moduł jądra. Zmienna `netdev-objs` definiuje zależności niezbędne do zbudowania pliku końcowego czyli netdev.o oraz po połączeniu plików składowych plik modułu netdev.ko. Wszystkie wymienione pliki binarne muszą posiadać takie same nazwy plików jak ich pliki źródłowe z jedyną różnicą w postaci rozszerzenia pliku. Dzięki temu system `kbuild` może łatwo znaleźć pliki źródłowe `fo_access.c` oraz `fo_access.h` potrzebne do zbudowania pliku `fo_access.o` składającego się na końcowy moduł.

Zmienna `ccflags-y` pozwala programiście na przekazanie dodatkowych parametrów, które mają być użyte przez GCC, domyślny kompilator jądra, podczas budowania modułu. W skład tej zmiennej wchodzą inne zmienne w celu ułatwienia organizacji. W przypadku zmiennej WARN dodawany jest szereg opcji zwiększających czułość kompilatora na niezgodności ze standardem języka C w celu wyłapania jakichkolwiek potencjalnych problemów. Zmienna `NOWARN` pozbywa się ostrzeżeń na temat nie użytych funkcji.

Dodatkowo przy pomocy zmiennej INCLUDE i zawartej w niej opcji `-I` włączany jest katalog `include`, który znajduje się poniżej katalogów z kodem modułu oraz serwera zawiera przede wszystkim plik nagłówkowy protocol.h definiujący szereg stałych wartości niezbędnych do prawidłowej komunikacji po gniazdach Netlink jak i TCP/IP. Bez niego nie dało by się skompilować ani modułu jądra ani serwera.

Z tak zdefiniowanym plikiem `Makefile` znajdującym się w folderze z kodem modułu jądra programista musi jedynie wywołać program make. Wynikiem tej operacji będzie pełen zestaw plików binarnych z rozszerzeniem `.o` oraz plik końcowy modułu `netdev.ko`, który jest gotowy do załadowania do pamięci systemu i rozpoczęcia pracy.

Warto wspomnieć, iż program make bierze argument `-j` który pozwala na podanie ilości procesów jakie może narzędzie make stworzyć w celu szybszego skompilowania kodu źródłowego na maszynach z wieloma procesorami lub rdzeniami.

### insmod i rmmod

W celu załadowania modułu do pamięci systemu i uruchomienia go w systemach GNU/Linux tradycyjnie używa się narzędzia `modprobe`. Jest to dość rozbudowany program, które potrafi zweryfikować czy moduł podany jako argument posiada jakiekolwiek zależności w postaci innych modułów jądra i załadować je w prawidłowej kolejności. Przyjmuje on również jako argument jedynie nazwę modułu.

Na potrzeby odnalezienia prawidłowego pliku modułu korzysta on z plików generowanych przez system `kbuild` umieszczanych w folderze `/lib/modules/[KERNEL_VERSION]`. Kluczowymi plikami są `modules.alias`, `modules.dep` oraz `modules.symbols`. Przy ich użyciu zapewnia on bezbolesne zarządzanie modułami na potrzeby używanego obrazu jądra.

Niestety narzędzie to nie potrafi ładować modułów, które zostały zbudowane poza procesem kompilacji, który stworzył używany przez system obraz jądra. W tym celu programista musi użyć znacznie prostszego narzędzia o nazwie `insmod`. Jego jedyną funkcją jest załadowanie i uruchomienie modułu jądra. Jako argument przyjmuje on ścieżkę do pliku modułu z rozszerzeniem `.ko` oraz może również przyjąć zestaw opcji jeżeli dany moduł takowe definiuje.

W celu usunięcia działającego modułu z systemu programista może użyć kolejnego bardzo prostego narzędzia o nazwie `rmmod`. Przyjmuje ono jako argument nazwę modułu i kilka prostych parametrów takich jak `-f` w celu wymuszenia usunięcia modułu. Oczywiście moduł nie zostanie usunięty jeżeli nie przeprowadzi on poprawnie procesu wyjścia i nie zwolni wszystkich zarezerwowanych przez siebie zasobów.

Narzędzia insmod jak i `rmmod` dostarczają bardzo okrojone komunikaty o błędach. W celu zrozumienia problemu potrzebne jest raczej odczytanie komunikatów systemowych dostępnych przy pomocy komendy `dmesg` lub w pliku `/var/log/messages`.

## Kluczowe struktury danych

Każdy dobry programista wie, iż by zrozumieć czyjś kod należy zacząć od kluczowych struktur danych lub klas w przypadku programowania obiektowego. Oto trzy najważniejsze struktury stworzone na potrzeby modułu netdev.

### Struktura netdev_data

Struktura `netdev_data` przedstawia pojedyncze urządzenie obsługiwane przez moduł jądra bez względu na to czy jest to urządzenie-atrap czy rzeczywiste urządzenie. Jest ona zdefiniowana w pliku `netdevmgm.h` i wygląda następująco:

    struct netdev_data {
        unsigned int nlpid;
        bool active;
        bool dummy;
        char *devname;
        atomic_t users;
        atomic_t curseq;
        spinlock_t nllock;
        struct cdev *cdev;
        struct device *device;
        struct hlist_node hnode;
        struct rw_semaphore sem;
        struct kmem_cache *queue_pool;
        DECLARE_HASHTABLE(foacc_htable, NETDEV_HTABLE_ACC_SIZE);
    };

Po pierwsze `nlpid` przechowuje identyfikator procesu(PID) serwera odpowiedzialnego za dane urządzenie i używany jest w komunikacji poprzez gniazdo Netlink, które identyfikuje przy jego pomocy źródło oraz cel odbieranych lub wysyłanych danych. Zmienna active mówi czy urządzenie jest gotowe obsługiwać operacje plikowe. Podczas tworzenia nowego urządzenia przyjmuje wartość `true` i zmienia się ona na `false` gdy sterownik straci kontakt z procesem serwera lub urządzenie jest w trakcie bycia usuwanym. Kolejna zmienna `dummy` definiuje czy urządzenie reprezentowane przez tą strukturę `netdev_data` to urządzenie-atrapa czy punkt dostępu do rzeczywistego urządzenia. Ta wartość nie zmienia się przez cały okres istnienia obiektu `netdev_data`. Tak samo niezmienna jest nazwa urządzenia znajdująca się zmiennej `devname`. Używana jest ona podczas tworzenia urządzenia-atrapy lub otwierania pliku rzeczywistego urządzenia.

Dwie zmienne `atomic_t` pozwalają na zmianę wartości w sposób atomowy, czyli zapewniający spójność danych na przestrzeni wielu wątków bez potrzeby korzystania z semaforów lub spinlocków. Zmienna `users`, które zlicza ilość referencji do danego obiektu `netdev_data` znajdujących się w różnych wątkach jądra lub strukturach danych pozwala na bezpieczne zwalnianie pamięci zarezerwowanej przez obiekt `netdev_data`. W przypadku gdy licznik referencji wynosi więcej niż jeden pamięć ta nie może być zwolniona gdyż sprawi to że wskaźnik do danego obiektu może spowodować próbę dostępu do pamięci, która nie należy do danego wątku i doprowadzić do paniki jądra. Zmienna `curseq` reprezentuje ostatnio użyty numer sekwencyjny użyty do synchronizowania kolejności wiadomości Netlink. Zmienna ta jest zwiększana o jeden za każdym razem gdy kolejna wiadomość jest wysyłana. Unikalność numeru sekwencyjnego umożliwia identyfikację oczekujących operacji plikowych.

Element `nllock` typu `spinlock_t` istnieje na potrzeby synchronizacji wysyłania wiadomości Netlink. Bez niego mogło by się zdarzać, iż potwierdzenie wysyłane przez funkcję `netlink_recv`, która jest odpowiedzialna za odbieranie wszystkich wiadomości netlink, zostało by w rzeczywistości wysłane po wiadomości wygenerowanej przez jakąś operacje plikową na urządzeniu z uwagi na to, iż za każdym razem tworzą własny wątek jądra. Jako że blokada w każdym przypadku trwa najwyżej kilkaset nanosekund więc używanie semafora mogło by wyraźnie spowolnić wysyłanie wiadomości Netlink. Co prawda spinlocki są raczej rzadko używanymi narzędziami synchronizacji gdyż są bardzo podatne na błędy programistów, w tym przypadku jest to idealne rozwiązanie.

Strukturę `cdev` omówiliśmy w rozdziale "[Urządzenia znakowe]" więc jej funkcja powinna być jasna. Używana jest ona do stworzenia atrapy urządzenia znakowego przypisanego do danego obiektu `netdev_data`. Zawiera w sobie również atrybut typu `dev_t`, który przechowuje niezwykle ważne wcześniej wytłumaczone numery dur oraz moll. Struktura ta jest używana jedynie gdy dany obiekt `netdev_data` jest urządzeniem-atrapą. Tak samo znana już struktura `device` opisana w rozdziale "[Model sterowników urządzeń w jądrze Linux]" reprezentuje plik w wirtualnym systemie plików `/dev` i używana jest do jego stworzenia jak i usunięcia. Tak samo jak atrybut `cdev` ten używany jest tylko z urządzeniem-atrapą.

Struktura `hlist_node` jest niezbędna aby obiekty typu `netdev_data` dało się przechowywać w ogólnej implementacji tablicy haszującej jądra o nazwie `hashtable` znajdującej się w `include/linux/hashtable.h`. Struktura ta zawiera w sobie wszystkie informacje niezbędne do prawidłowego działania tablicy i używana jest na przykład gdy programista chce usunąć dany obiekt z tablicy. Struktura danych `hashtable` zostanie opisana w większym detalu w dalszych rozdziałach jako że gra dość kluczową rolę w module netdev.

Zmienna `sem` typu `struct rw_semaphore` to semafor odczytu i zapisu. Obiekty `netdev_data` są odczytywane i modyfikowane przez wiele wątków jednocześnie. Z tego powodu niezbędne jest aby zabezpieczyć dostęp do tej struktury danych przy pomocy semafora. W tym przypadku został użyty specyficzny rodzaj semafora, który rozróżnia pomiędzy operacjami odczytu oraz zapisu. Ponieważ obiekty `netdev_data` są częściej odczytywane raczej niż modyfikowane użycie tego typu semafora znacznie zmniejsza czas oczekiwania dla wielu wątków, które chciały by się dostać do danych w nich zawartych bez ich modyfikowania.

Następny jest atrybut `queue_pool` typu `kmem_cache`. Przedstawia on pulę pamięci zarezerwowaną specjalnie na potrzeby konkretnych obiektów. W tym przypadku są to obiekty typu `struct fo_req` reprezentujący jedno żądanie wykonania operacji plikowej na urządzeniu. Z uwagi na to że sterownik netdev będzie tworzył i niszczył bardzo wiele obiektów tego typu z racji ilości operacji plikowych jakie różne procesy będą wykonywać zarezerwowanie zawczasu odpowiedniej puli tego typu obiektów potrafi znacznie przyspieszyć ich tworzenie oraz oszczędzić dużo pracy z czyszczeniem i przenoszeniem zwolnionej pamięci jądru.

Ostatnim elementem jest tablica haszująca, która będzie przechowywać referencje do obiektów typu `struct fo_access` reprezentujące pojedynczy proces otwierający urządzenie-atrapę na potrzeby pracy z nim. Jako że obiekty te są identyfikowane przez numery identyfikacyjne procesów(PID), które je stworzyły ich szybkie wyszukiwanie po tej właśnie wartości było kluczowe dla sprawnego działania modułu. Z tego powodu została użyta tablica haszująca.

### Struktura fo_access

Jako że jeden plik urządzenia-atrapy może być otwarty przez wiele procesów jednocześnie sterownik netdev musi zdawać sobie sprawę z tego faktu i przechowywać niezbędne informacje w celu obsłużenia danych procesów w indywidualny sposób. Z tego powodu istnieje struktura `fo_access`, która reprezentuje pojedynczy dostęp do urządzenia-atrapy.

    struct fo_access {
        int access_id;
        struct netdev_data *nddata;
        struct file *filp;
        struct kfifo fo_queue;
        struct rw_semaphore sem;
        struct hlist_node hnode;
    };

Struktura `fo_access` musi być jakoś identyfikowana. Jako że jest ona ekskluzywnie połączona z procesem, który rozpoczął operacje na pliku urządzenia to identyfikator danego procesu(PID) idealnie się do tego nadaje jako że musi być unikalny w środowisku danego systemu. Zmienna `access_id` przechowuje ten identyfikator. Następny jest wskaźnik na strukturę `netdev_data`, który zapewnia łatwy dostęp do obiektu odpowiedzialnego za dane urządzenie.

Kolejna struktura używana jest jedynie przez urządzenie-atrapę. Jak było już powiedziane w rozdziale "[Model VFS]" struktura `file` reprezentuje plik otwarty przez pojedynczy proces i istnieje on tak długo jak dany plik jest otwarty przez dany proces. Tak samo struktura `fo_access` istnieje tak długo jak dany proces korzysta z pliku urządzenia-atrapy i zostanie usunięty gdy dany proces wywoła operację `release` na pliku urządzenia.

Kolejka FIFO `fo_queue` typu reprezentowana przez strukturę `kfifo` przeznaczona jest na obiekty typu `fo_req` odpowiedzialne za pojedynczą operację plikową. Gdy moduł otrzyma odpowiedź na konkretna operację plikową używając identyfikatora procesu korzystającego z danego urządzenia odnajduje odpowiedni obiekt `fo_access` a następnie na jego podstawie wydobywa odpowiedni obiekt operacji `fo_req` z kolejki `fo_queue`.

Jako że tak samo jak `netdev_data` obiekty `fo_access` mogą być używane przez wiele wątków jądra jednocześnie niezbędne jest użycie semafora. Tutaj również operacja odczytu przeważają na operacjami modyfikacji użyty jest więc typ semafora, który rozróżnia pomiędzy tymi operacjami.

Na samym końcu jest obiekt typu `hlist_node` o nazwie `hnode` pozwalający na umieszczanie obiektów `fo_access` w tablicy haszującej `hashtable` z kluczem w postaci wartości atrybutu `access_id` w celu łatwego wydobywania właściwego obiektu na podstawie identyfikatora procesu.

### Struktura fo_req

struct fo_req {
    long        seq;        /* for synchronizing netlink messages     */
    short       msgtype;    /* type of file operation                 */
    int         access_id;  /* identification of opened file */
    void       *args;       /* place for s_fo_OPERATION structures    */
    void       *data;       /* place for fo payload */
    size_t      size;       /* size of the s_fo_OPERATION structure   */
    size_t      data_size;
    int         rvalue;     /* informs about success of failure of fo */
    struct completion comp; /* completion to release once reply arrives */
};

### Struktury argumentów operacji plikowych

### Tablica haszująca

## Wielowątkowość oraz wywłaszczanie
### atomic_t
### Semafory odczytu/zapisu
### spinlock

## Netlink
### Tworzenie gniazda
### odbieranie wiadomości
### wysyłanie wiadomości

## Operacje plikowe
### Serializacja
### Wysyłanie operacji
### Odbieranie operacji
### Wykonywanie operacji
### Zakańczanie operacji

## Architektura serwera
### Podział na procesy
### Użycie select()

##

# Wyniki (5%)

## Porównanie prędkości transmisji danych
## Porównanie opóźnień
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
[b12]: "Linux Kernel - Przewodnik Programisty", str 98-99 i 365-370, Robert Love
[b13]: https://www.gnu.org/software/libc/ "The GNU C Library"
[b14]: https://www.gnu.org/software/make/ "GNU Make"
[b15]: http://daringfireball.net/projects/markdown/ "Markdown format"
[b16]: https://computing.llnl.gov/tutorials/pthreads/ "POSIX Threads"

# Dodatki

## Zawartość CD

* Plik źródłowy LaTeX z treścią pracy
* Pliki graficzne diagramów oraz wykresów
* Plik PDF wygenerowany na podstawie pliku LaTeX
* Pełen kod źródłowy projektu w folderze `netdev`
* Katalog .git zawierający cały opis historii rozwoju projektu zawarty w bazie danych systemu kontroli wersji Git

## Spis tabel
## Spis rysunków
