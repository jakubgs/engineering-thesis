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

# Teoria/Modele organizacji w jądrze Linux(5%)

* definicja rozważanego zagadnienia
* aktualny stan wiedzy
* modele, wzory
* rzeczywisty stan

## Model sterowników urządzeń w jądrze Linux

Wszystkie urządzenia w systemie linux jak i sterowniki które implementują ich zarządzanie istnieją w jądrze w postaci dokładnie zdefiniowanych struktur, które razem tworzą model sterowników urządzań. W modelu tym ustnieją cztery podstawowe obiekty, również znane jako struktury, bez których żadne urządzenie nie ma prawa istnieć.

    * Szyna(Bus) - Szyna to kanał pomiędzy procesorem a urządzeniem lub wieloma urządzeniami. Na potrzeby modelu sterowników urządzeń linux wszystkie urządzenia, bez względu na to czy fizyczne czy wirtualne, muszą przynależeć do odpowiadającej im szyny. Przykładami szyn mogą być szyny PCI, USB czy I2C. Model ten reprezentuje rzeczywiste połączenia pomiędzy szynami a urządzeniami które one kontrolują. Szyna jest reprezentowana przez strukturę "bus_type". Zaiwera ona informacje o nazwie szyny, informacje o podłączonych urządzeniach, zbiór operacji które można na nich wykonać i wiele innych informacji.

    // można tutaj wkleić definicje struktury

    * Klasa(Class) - Jest to struktura wyższego poziomu abstrakcji która nie skupia się na nieskopoziomowych kwestiach implementacji jak szyna lecz na ogólnym rodzaju urządzenia i jakie operacje da się na nim wykonać, niż jak dokłądnie jest ono podłączone do komputera. Przykłądowe klasy urządzeń to urządzenia audio, sieciowe lub dyski. Klasa reprezentowana jest w jądrze przy pomocy sktruktury "class".

    // można tutaj wkleić definicje struktury

    * Sterownik(Driver) - Obiekt ten opisuje i wskazuje na konkretny sterownik, również nazywany modułem, załadowany w systemie i pozwala na wykonywanie podstawowych operacji na sterownikach takich jak ich zatrzymywanie, uruchamianie, odpytywanie czy wyłączanie i usuwanie sterownika. Implementacja tego obiektu to struktura "device_driver" która przechowuje takie informacje jak nazwa sterownika, prywatne dane sterownika czy rodzaj szyny z której urządzenia obsługuje dany sterownik.

    // można tutaj wkleić definicje struktury

    * Urządzenie(Device) - Na prawie najniszym poziomie istnieje obiekt urządzenia który definiuje konkretną instancje obsługiwanego fizycznego lub wirtualnego urządzenia oraz plik go reprezentujący w wirtualnym systemie plików /dev (devfs). Zdefiniowany jako struktura o nazwie "device" zawiera duży zbiór informacji na temat urządzenia do którego jest przypisany, w tym urządzenie macierzyste, prywatne dane modułu sterownika, szynę do której jest przyłączony, sterownik który obsługuje dane urządzenie czy numery dur(major) oraz moll(minor) które identyfikują wszystkie sterowniki i urzadzenia w systemie.

    // można tutaj wkleić definicje struktury

Wszystkie te struktury są zdefiniowane w pliku nagłówkowym include/linux/device.h wraz z dużą ilością komentarzy opisujących zwierające się w nich atrybuty, struktury oraz wskaźniki do funkcji. Wraz z wieloma mniej ważnymi strukturami które są zazwyczaj atrybutami tych kluczowych obiektów tworzą one model sterowników urządzeń w jądrze Linux. Właściwie żadnej z tych struktur programista nie tworzy lub nie zapełnia własnoręcznie. Istnieje szereg metod opakowujących z których należy korzystać jeżeli programista chce stworzyć lub wykonać jakiekolwiek operacje na którejkolwiek z tych struktur. Ma to na celu ograniczenie błędów płynących z wywpłaszczania wątków jądra jak i problemów z kompatybilności w razie przyszłych zmian w definicjach którejkolwiek z tych kluczowych struktur.

Jednym z najważniejszych elementów identyfikujących urządzenia w systemie Linux wspomniany już w opisie obiekty urządzenia jest para numerów dur oraz moll, znanych w angielskim jako "major" oraz "minor". Numery te są zapisywane kodzie źródłowym jako pojedyńczy typ danych dev_t który od prawie zawsze był definiowanych jako typ unsigned long. Pierwsze 12 bitów przeznaczone jest na numer dur a pozostałe 20 bitów na numer mol. Numer dur definiuje rodzaj sterownika a numer moll definiuje numer urządzenia obsługiwany przez dany sterownik. Wszystkie urządzenia w systemie posiadające swoją wirtualną reprezentacje w systemie plików /dev mają przydzielone numery dur oraz moll. Dobrym przykładem jest bardzoe proste urządzenie generujące losowe dane znadujące się pod nazwą /dev/urandom. Podejrzenie takiego urządzenia przy pomocy polecenia "ls -l" daje nam taki oto wynik:

    $ ls -l /dev/urandom
    crw-rw-rw- 1 root root        1,   9 Aug  7 21:34 urandom

W miejscu w którym w przypadku normalnych plików pojawia się wielkość widnieją dwa oddzielone przecinkiem numery. Numer 1 to dur i identyfikuje on sterownik obsługujący urządzenia urandom a numer 9 to moll i identyfikuje on konkretne urzadzenie obsługiwane przez ten sterownik. Listę sterowników urządzeń znakowych oraz blokowych i przypisanych do nich numerów dur można znaleźć odczytując wirtualny plik /proc/devices:

    $ cat /proc/devices
    Character devices:
      1 mem
      4 /dev/vc/0
      4 tty
      ...

Widać tutaj że sterownik odpowiedzialny za wirtualne urządzenie /dev/urandom nazywa się "mem". Jest to zrozumiałe biorąc pod uwagę iż jest do urzadzenie zupełnie wirtualne, które w razie potrzby na żywo generuje swoje dane ze źródeł entropii w jądrze Linux. Przy okazji warto wspomnieć iż praktycznie wszystkie źrodła entropii(losowości) w systemie Linux to urządzenia fizyczne, na przykład używane są czasy przybycia nowych paczek do interfejsu sieciowego, a dokładnie pojawienia się przerwania IRQ związanego z danym urządzeniem w celu zwiększania entropii czyli losowości generowanych danych. Dokłądniejszy opis można znaleźć w książce [Roberta Love][b12].

Ogólny wgląd w model sterowników urządzeń można znaleźć w plikach dokumentacji rozprowadzanych razem z kodem źródłowym jądra w folderze Documentation/driver-model. Jest to jednak bardzo pobierzne spojrzenie na elementy tworzące ten system i do prawidłowego jego zrozumienia niezbędne jest czytanie samego kodu źródłowego i ciągłe konsultowanie z dokumentacją oraz bardzo często listami mailowymi związanymi z rozwijaniem jądra Linux jako że jest to model wciąż rozwijany. Nic w jądrze Linux nie jest statyczne, i pomimo tego iż dla przeciętnego użytkownika sposób korzystania z systemu nie zmienia się zbytnio na przełomie lat pod maską trwają szeroko zakrojone prace mające na celu ulepszenie jądra Linux. Szybkość z jaką prowadzone są prace nad jądrem Linux jest jedną z jego ogromnych zalet, sprawia ona jednak iż książki opisujące techniki programowania sterowników czy innych elementów jądra bardzo szybko stają się przestarzałe i w dużym stopniu bezużyteczne jako przewodnik dla potencjalnych przyszłych programistów jądra.

## Model VFS

Wirtualny system plików znany przede wszystkiem pod skrótem VFS(Virtual File System) to warstwa abstrakcji wprowadzona w jądrze Linux mająca na celu ujednoliczenie interfejsów przez które przestrzeń jądra oraz przestrzeń użytkownika mają dostęp do różnego rodzaju systemów plików. W dzisiejszych czasach istnieje ogromne bogactwo wszelkiego rodzaju systemów plików, od dość uniwersalnych, takich jak rodzina Ext(2,3 i 4) po stronie GNU/Linux czy FAT32 oraz NTFS po stronie produktów Microsoft, przez przystosowane do konkretnych zastosowań takich jak ZFS, BTRFS, UFS czy JFS, sieciowe systemy plików jak NFS, SMBFS i CIFS, po wirtualne systemy plików takie jak procfs, devfs, sysfs czy tmpfs. Absurdem było by implementowanie oddzielnych narzędzi do wpsółpracy ze wsystkimi tymi systemami. W celu unifikacji sposobu implementowania dostepu oraz kontroli wszelkiego rodzaju systemów plików stworzony został model wirtualnego systemu plików VFS. W jądrze Linux jest to prawdopodobnie system, który najbardziej czerpie z filozofii programowania obiektowego, pomimo tego iż jest ono prawie w 100% napisane w czystym języku C które nie posiada mechanizmów obiektowości takich jak dziedziczenie czy operacje na obiektach znanych z takich języków jak C++ czy Java.

Dzięki wprowadzeniu tego modelu wszystkie sterowniki poszczególnych systemów plików muszą implementować konkretny zestaw struktur oraz funkcji, które w sposób zupełnie przezroczysty pozwalają wszystkim pozostałym elementom systemu na korzystanie z zasobów znajdujących się w tych systemach plików bez zastanawiania się na różnicami pomiędzy poszczególnymi implementacjami, formatem danych czy rodzajem nośnika na którym przechowywane są owe dane. Bez względu na to jak rzeczywiście działa dany system plików, musi on przedstawiać swoje dane przy uzyciu czterech podstawowych obiektów zdefinowanych przez model VFS:

    * Blok główny(Superblock) - Jest to obiekt który zbiera w sobie wszystkie informacje o plikach i strukturze katalogów znajdujących się w danym systemi plików, jak i również jego rodzaj, przypisane do niego urządzenie oraz listę operacji które można wykonać na danym systemie plików. Blok główny bardzo często jest przechowywany w specjalnie do tego przeznaczonym sektorze dysku lub innego nośnika i posiada wszystkie informacje potrzebne by zlokalizować dane składające się na wybrany plik na przestrzeni całego nośnika. Systemy plików nie przywiązane do żadnego urządzenia fizycznego nadal muszą stworzyć obiekt bloku głównego i przechowywać go w pamięci jądra jeżeli chcą w jakimkolwiek wymiarze udostępniać swoje usługi jodru oraz procesom przestrzeni użytkownika. Blog główny definiowany jest jako struktura "super_block".

    * i-węzeł(i-node) - Każdy otwarty plik oraz katalog znajdujący się w obrębie konkretnego systemu plików reprezentowany jest przez jeden egzemplarz tego obiektu. Zbiera on w sobie wszystkie informacje potrzebne do manipulowania plikami i istnieje on tak długo jak długo plik lub katalog jest używany przez jakikolwiek proces w sustemie. Obiekt i-węzła opisuje dowolny plik, bez względu na to czy jest to zwykły plik na dysku, plik strumienia danych, urządzenie z sustemu plików /dev czy plikowa reprezentacja atrybutów sterowników w systemie plików /sys. Zawiera on w sobie informacje o czasie stworzenia i modyfikacji, prawa dostępu, właścicieli, rozmiar pliku i wiele innych informacji niezbędnych procesom do wykonywania na nich operacji. Warto od razu zwrócić uwagę iż w systemach z rodziny unix takich jak Linux katalogi są po prostu specyficznym rodzajem pliku, w związu z tym i-węzeł opisuje zarówno pliki jak i katalogi. Struktura która definiuje ten obiekt nazywa się "inode".

    * Wpis katalogowy(Dentry) - Obiekt ten jest przeznczony raczej do ułatwienia wyszukiwania danych w hierarchicznej strukturze katalogowej wielu systemów plików niż do opisywania rzeczywistych obiektów znajdujących się w pojedyńczym systemie plików. Każdy program przestrzeni użytkownika musi korzystać ze ścieżki dostępu jeżeli chce się dostać do jakiegokolwiek zasobu w systemie plików. Przykładową śceżką dostepu do pliku znajdującego się na płycie CD jest "/mnt/cdrom/README.txt". W tej ścieżce dostępu znajdują się cztery wpisy katalogowe: /, mnt, cdrom oraz README.txt. Trzy pierwsze obiekty, "/" przedstawiający katalog-korzeń i "mnt" oraz "cdrom przdstawiający katalog mnt i zawierający się w nim katalog cdrom, znajdują się na systemie plików ext4. Jednak sam plik README.txt znajduje się w systemie plików płyty CDROM, ISO9660. Jądro utrzymuje bufor wpisów katalogowych, który zbiera w miarę jak procesy otwierają i używają plików dostepnych w systemie. Utrzymywanie tego typu bufora znacznie przyspiesza otwieranie wcześniej otwortych zasobów. Wpisy te reprezentowane się przez strukturę "dentry".

    * Plik(File) - Tak jak i-węzeł reprezentuje każdy przynajmniej raz otwarty plik w jądrze tak obiekt pliku reprezentuje plik otwarty przez konrketny proces. Obiekt ten istnieje tak długo jak dany proces korzysta z rzeczywistego pliku i zostaje skasowany gdy proces ten użyje wywołania close() na przypisanym do niego deskryptora pliku(file descriptor). Posiada on odwołanie do odpowiedzniego obiektu i-węzła oraz zbiór informacji przydatnych przy operowaniu na pliku takich jak obecna pozycja w pliku, tryb dostepu do pliku czy zestaw praw dostepu.

Struktura "file_operations" jest kluczowym elementem w implementowania sterowników urządzeń i będzie grała główną rolę w implementacji projektu.

Z punktu widzenia sterowników urządzeń najbardziej kluczowymi obiektami są i-węzły oraz pliki. Obiekt pliku posiada jedeną z najważniejszych struktru w całym jądrze Linux o nazwie "file_operations", zapisaną jako atrybut "f_op", która posiada kolekcję wskaźników do funkcji które pozwalają wykonywać takie operacje jak open, read, write czy close. Wszystkie urządzenia znakowe oraz blokowe muszą implementować podstawowy zestaw operacji reprezentowany przez tą strukturę i prawie każda z nich bierze jako argument obiekt pliku. Niektóre biorą również obiekt i-węzła. Dzięki temu sterownik otrzymuje niezbędne informacje odnośnie tego który plik urządzenia został otwarty.

Struktura "file_operations" jest kluczowym interfejsem który pozwala ukryć ogromną różnorodność urządzeń pod zestawem prostych operacji, z których każdy proces w przestrzeni użytkownika może skorzystać bez rozrózniania pomiędzy typami urządzeń. Struktura ta jest zdefiniowana w pliku include/linux/fs.h i wygląda następująco:

    struct file_operations {
	    struct module *owner;
	    loff_t (*llseek) (struct file *, loff_t, int);
	    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
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
	    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
	    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	    int (*check_flags)(int);
	    int (*flock) (struct file *, int, struct file_lock *);
	    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	    int (*setlease)(struct file *, long, struct file_lock **);
	    long (*fallocate)(struct file *file, int mode, loff_t offset,
			      loff_t len);
	    int (*show_fdinfo)(struct seq_file *m, struct file *f);
    };

W celu udostępnienia urządzenia znakowego na urządzeniu połączonym przez sieć kluczowe jest stworzenie "fałszywego" pliku urządzenia na komputerze klienckim, udostępnienie zestawu niezbędnych operacji plikowych i przekazanie wszystkich ich wywołań do maszyny posiadającej rzeczywiste urządzenie, a następnie przekazanie wyniku wywołania tej operacji z powrtotem do maszyny klienckiej.

Urządzenie znakowe nie musi jednak implementować wszystkich operacji. Przykładem operacji bezużytecznych dla plików urządzeń jest funkcja readdir(), która ma sens tylko i wyłącznie dla plików które są katalogami, oraz sendpage(), która przeznaczona jest raczej dla urządzeń sieciowych takich jak gniazda unix. Inne operacje takie jak aio_read() i aio_write() są rzadko implementowane dla urządzeń i prawie zawsze wartość ich wskaźnika wynosi NULL co sprawia że kernel po prostu używa zwykłych funkcji read() oraz write().

Wirtualny system plików jest kluczową abstrakcją w zrozumieniu jak jądro linux udostępnia swoje urządzenia do przestrzeni użytkownika. Prawie każde urządzenie w systemie posiada swoją reprezentacje w systemie plików /dev i implementuje większość lub wszystkie z tych operacji. Oczywiście istnieje szereg innych metod komunikacji z procesami specyficznych dla konkretnego rodzaju urządzeń. Na przykład karty graficzne posiadają interfejs DRI(Direct Rendering Infrastructure) która omija interfejs operacji na plikach w celu osiągnięcia jak najwyższej wydajności w obróbce grafik. Na szczęście na potrzeby zwykłych urządzeń znakowych implementacja podstawowych operacji na plikach w zupełności wystarczy.

## Urządzenia znakowe

Urządzenia znakowe to jedne z najprostszych urządzeń jakie można znaleźć w systemie Linux. Główna róźnica pomiędzy urządzeniami zakowymi a urządzeniami blokowymi wynika z użycia buforów i pamięci podręcznej oraz możliwośc wykonywania operacji losowego dostępu(random access). Urządzenia znakowe nie korzystają z buforów ani pamięci podręcznej i nie udostępniają możliwości odczytywania czy zapisywania danych w dowolnie wybranych miejscach. Urządzenia blokowe natomiast pozwalają procesom wybirać punkt z którego będą odyczytywać i do którego będą zapisywać dane oraz korzystają z pamięci podręcznej w celu zwiększenia wydajności jako iż częstym zjawiskiem jest wielokrotne odczytywanie lub zapisywanie tej samej lokalizacji w medium przechowującym dane.

Podstawowym obiektem przedstawiającym urządzenia znakowe w jądrze Linux jest struktura "cdev" zdefiniowan w pliku include/linux/cdev.h. Wygląda ona następująco:

    struct cdev {
	    struct kobject kobj;
	    struct module *owner;
	    const struct file_operations *ops;
	    struct list_head list;
	    dev_t dev;
	    unsigned int count;
    };

Jak prawie każda ważna struktura w jądrze posiada ona k-obiekt(kobject), który jest strukturą używaną przez jądro Linux do zarządzania i organizowania obiektów używanych w trakcie działania systemu. Można ten obiekt nazwać obiektem pomocniczym, ułatwia on na przykład lokalizowanie struktur na podstawie elementów które się w nich zawierają. K-obiekty to złożony mechanizm kontroli wielu kluczowych obiektów istniejących w kodzie jądra Linux i pojawia się on w prawie każdym podsystemie jaki w nim istnieje. Dokłądne wytłumaczenie tej struktury i jej działania wykracza znacznie poza zakres tego projektu i samo w sobie było by dobrym tematem na dość rozbudowaną pracę inzynierską.

Kolejnym atrybutem jest  wskaźnik do struktury "module" który wskazuje na moduł sterownika obsługujący dane urządzenie znakowe. Posiada on oczywiście strukturę "file_operations" którą już zindentyfikowaliśmy jako niezbędny element każdego sterownika który chce udostepnić urządzenie które obsługuje systemowi. Wskaźnik ten jest wypełniany przez sterownik podczas inicjalizacji urządzenia i jądro używa tych operacji do nadpisania domyslnych operacji na plikach które przypisane są plikowi na podstawie konfiguracji sterownika systemu plików w jakim się znajduję. Struktura "list_head" jest pierwszym wskaźnikiem w dwukierunkowej liście obiektów i-węzłów czyli plików w systemie /dev połączonych z tym konkretnym urządzeniem znakowym a count to ilość elementów w liście i-węzłów. Atrubut dev zapisuje przedstawione wcześniej numery dur oraz moll, które w unikalny sposób identyfikują to urządzenia oraz jego sterownik.

Struktura ta nie jest jednak tworzona i wypełniana przez programistę. Jądro udostępnia szereg metod opakowującychy które gwarantują prawidłową obsługę błędów i kompatybilność na przestrzeni wielu wersji jądra. Oto lista najważniejszych z nich:

    struct cdev *cdev_alloc(void);
    Słuzy do alokowania przestrzeni pamięci niezbędnej dla struktury cdev. Zaleca się uyżywania tej fukncji z uwagi na to iż poza alokowaniem pamięci inicjalizuje ona również k-obiekt zawarty w cdev.

    void cdev_init(struct cdev *, const struct file_operations *);
    Odpowiada za inicjalizację struktury cdev przy pomocy wcześniej przygotowanej struktury reprezentującej pełen zestaw operacji plikowych implementowanych przez dany sterownik. Większośc sterowników urządzeń znakowych wywołuję tą funkcje z celu przygotowania do użycia struktury cdev, która istnieje jako atrybut jakiejś wewnętrznej struktury używanej przez sterownik do przechowywania danych u implementowanym urządzeniu. Również inicjalizuje k-obiekt.

    int cdev_add(struct cdev *, dev_t, unsigned);
    Dodaje urządzenie reprezentowane przez strukturę cdev do systemu. Funkcja ta powinno zostać wywołana wtedy i tylko wtedy gdy sterownik wykonał pomyślnie wszystkie operacje przygotowujące urządzenie do obsługi operacji na nim wykonanych. Po poprawnym powrocie z tej funkcji urządzenie jest uznawana za gotowe do pracy. Dodatkowo zwiększa licznik referencji zawarty w k-obiekcie.

    void cdev_del(struct cdev *);
    Usuwa urządzenie z systemu oraz zmniejsza licznik referencji zawarty w k-obiekcie. W efekcie może to spowodować zwolnienie pamięci przeznaczonej dla struktury gdy licznik referencji osiągnie 0.

Funkcje te są zdeklarowane w pliku include/linux/cdev.h oraz zdefiniowane w pliku fs/char_dev.c. Poza wywołaniem funkcji cdev_alloc(), cdev_init() oraz cdev_add() w odpowiedniej kolejności sterownik nie musi w żaden sposób operować na strukturze cdev aż do momentu, w którym gotowy jest usunąć urządzenie i wywołuje cdev_del(). Poza tym struktura cdev pozostaje dostępna raczej jako repozytorium informacji o urządzeniu raczej niż aktywnie modyfikowany obiekt w trakcie działania urządzenia.

W celu przechowywania dokłądniejszych danych o naszym urządzeniu sterownik będzie musiał stworzyć własną wewnętrzną strukturę reprezentującą urządzenie. Zostanie to opisane w dalszych rozdziałach.

## Model oprogramowania

Jasnym jest że na projekt będzie musiał składać się moduł sterownika jądra Linux, który będzie odpowiedzialny za stworzenie urządzenia-atrapy i odbieranie operacji plikowych na nim wikonanych jak i przekazywanie ich do rzeczywistego urządzenia na maszynie króra je posiada. Jednak z uwagi na na wiele decyzji podjętych podczas tworzenia jądra Linux wielu programistów jądra od razu zaznaczy iż nie należy robić w przestrzeni jądra wielu rzeczy które są jak najbardziej naturalne w przestrzeni użytkownika. Dwa takie przykłądy to otwieranie i czytanie plików oraz wykonywanie połączeń sieciowych w kodzie jądra. Istnieje wiele powodów dla których nie należy robić tych rzeczy z przestrzeni jądra. Po pierwsze czytanie plików czy odbieranie danych z gniazda wymaga konwertowania tych danych do formatu używalnego przez kod jądra. W wielku przypadkach jest to wyzwanie które jest bardzo podatne na błędy w postaci rezerwowania zbyt dużych przestrzeni pamięci w trakcie tego procesu lub używanie wartości które nie zostały poprawnie sprawdzone które mogą prowadzić do wykraczania poza przydzieloną nam pamięć. W przypadku plików problemem jest również lokalizacja pliku a w przypadku połączeń sieciowych odnalezienie porpawnego adresu co bardzo często kończy się statycznym ustawianiem ścieżek i adresów w kodzie jądra co będzie wymagać przekompilowania za każdym razem gdy ta lokalizacja lub adres się zmieni.

Dodatkowym problemem jest zagrożenie bezpieczeństwa jądra. Pobieranie danych prosto ze zdalnej lokalizacji do jądra jest wręcz jawnym zaproszeniem dla wszelkiego rodzaju złośliwych użytkowników sieci do próby wykorzystania naszego modułu w celu przejęcia kontroli nad naszym systemem. Jądro powinno być ostatnim bastionem bezpieczeństwa w systemie operacyjnym i wystawianie go dla publicznego dostepu przez sieć jest bardzo złą praktyką. Popełnienie takiego błędu jest wręcz gwarancją ze twój kod nie zostanie przyjęty do projektu jądra Linux i najprawdopodobniej programista taki zostanie pouczony na temat podstawowych zasad projektowania systemów operacyjnych.

Z uwagi na te obiekcje projekt musi zostać podzielony na dwa odrębne elementy. Przede wszystkim moduł jądra odpowiedzialny za stronę sprzętową oraz tworzenie urządzenia atrapy. Drugim elementem będzie program przestrzeni użytkownika odpowiedzialny za wczytanie odpowiedniej konfiguracji, połączenie się z modułem jądra oraz zdalną maszyną i ustanowieniem połączenia pomiędzy oboma końcami transkcji. Dzięki takiemu modelowi wszystkie problemy związane z wczytywaniem konfiguracji, kontrolą modułu oraz bezpieczeństwem zostaną przeniesione do warstwy użytkownika co powinno znacznie uprościć kod modułu i przyspieszyć jego działanie oraz sprawić że końcowy produkt będzie bardziej elastyczny w użytkowaniu i konfiguracji. Poniżej przedstawiam prosty diagram opisujący przykłądowe połączenie pomiędzy urządzeniem fizycznym udostępniającym swoje zasoby na maszynie serwerowej a urządzeniem-atrapą udającym rzeczywiste urządzenie na maszynie klienckiej:

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

Przyjmując taki model projektu rozszerza projekt o dodatkową warstwę komunikacji pomiędzy przestrzenią jądra a przestrzenią użytkownika co w wyraźny sposób komplikuje proces komunikacji pomiędzy fałszywym urządzeniem a rzeczywistym fizycznym urządzeniem. Jest to jednak utrudnienie niezbędne biorąc pod uwagę polisy jakie rządzą rozwojem kodu jądra Linux. Jako że chcemy udostępniać urządzenia fizyczne za pośrednictwem tego oprogramowania kluczowa jest wysoka przepustowaość w przesyłaniu dużych ilości danych i niezawodność która zapewni że żadna operacja na pliku nie zostanie pominięta podczas przesyłania. Dokłądny opis podjęcia tej decyzji znajduje się w rozdziale [TODO](Wybór metod komunikacji - z przestrzenią użytkownika).

W celu zmniejszenia nakłądu pracy kosztem małego zwiększenia złożoności kodu podjęta została decyzja aby zaimplementować jeden moduł jądra, który będzie wykonywał rolę serwera oraz klienta i jeden program przestrzeni użytkownika, który tak samo będize wypełniał rolę serwera oraz klienta. Powinno to znacznie zmniejszyć ilość linijek kodu niezbędnych do ukończenia tego projektu i uprości konfiguracje dla potencjalnych użytkowników. 

# Wybrane rowziązania (10%)

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

Metody komunikacji oparte na gniazdach BSD udostępnieją znacznie bardziej przyjazdny interfejs do komunikacji z przestrzenią jądra. Bardziej prymitywną metodą z dwóch dostepnych jest używanie tak zwanych surowych gniazd typu AF_RAW które pozwalają na przesyłanie struktur danych o stałym formacie oraz korzystania ze specjalnego wywołania systemowego ioctl które przyjmuje jako argument dwie wartości typu long, pierwszą definiującą typ komunikatu i drugą wartość samego komunikatu. Jest to nadal bardzo ograniczone jeżeli chodzi o możliwość modyfikowania i rozwijania metody komunikacji ale powala jako pierwsza metoda na przysyłanie dużych ilości danych czego nie można powiedzieć o metodach komunikacji opartych na wirtualnych systemach plików.

Ostatnią metodą jąką udostępnia jądro linux jest protokół Netlink oparty na gniazdach BSD. Jest to bezpołączeniowy protokół komunikacji opierający się na datagramach przypominający UDP, który pozwala na transfer dużych ilości danych, zapewnia taki sam poziom przenośności jak każdy inny protokół sieciowy oraz pozwala na łatwe rozwijanie metod komunikacji w miare rozwoju projektu. Kluczową charakterystuką protokołu Netlink jest stały format zapewniający jasny podział na wiadomości, które wyróżniają się własnym numerem sekwencyjnym, typem komunikatu, zmienną długością, numerem identyfikującym proces źródłowy oraz zestawem specjalnych flag. Flagi obecne w każdym datagramie Netlink pozwalają na dodatkową implentacje niezawodności porzez przesyłanie wiadomości potwierdzających otrzymanie przekazu pozbywając się tym sposobem ograniczeń wynikających z niepołączeniowej natury protokołu.

Biorąc pod uwagę wszystkie możliwości protokołu Netlink jest on najlepszą opcją jeżeli chodzi o implementowanie sprawnej metody komunikacji, która pozwala na szybki transfer dowolnej ilości danych oraz łatwą identyfikacje poszczególnych wiadomości i ich typu. Aplikacje w przestrzeni użytkownika mogą korzystać z protokołu netlink tak samo jak robią to w przypadku innych protokołów sieciowych. Jedyną różnicą jest potrzeba wywołania funkcji socket() z rodziną gniazda ustawioną na AF_NETLINK oraz typem protokołu na SOCK_RAW i używaniem struktury "nlmsghdr" jako podstawę budowania każdej wiadomości przeznaczonej do przestrzeni użytkownika.

Kluczowym elementem protokołu Netlink jest struktura nagłówka "nlmsghdr":

    struct nlmsghdr {
        __u32 nlmsg_len;   /* długość wiadomości wliczając nagłówek        */
        __u16 nlmsg_type;  /* typ wiadomości                               */
        __u16 nlmsg_flags; /* dodatkowe flagi do kontroli przepływu        */
        __u32 nlmsg_seq;   /* numer sekwencyjny do rozróżniania wiadomości */
        __u32 nlmsg_pid;   /* identyfikator procesu źródłowego             */
    };

Struktura ta pozwala na łatwą organizacje własnego protokołu komunikacji przy użyciu indiwidualnych wiadomości o konkretnym rodzaju, numerze identyfikacyjnym, adresie źrółowym oraz zestawie flag które pozwalają na rozdzielanie dużych wiadomości na cześci lub proszenie o wiadomościn potwierzające fakt oderbania przekazu po drugiej stronie pary gniazd.

Dodatkowo pokrewieństwo gniazd Netlink z gniazdami TCP/IP jest kolejnym ułątwienie pozwalającym na łatwe przekazywanie komunikatów odebranych z jednego rodzaju gniazda do drugiego. Powinno to znacznie uporosić implementacje procesu serwera odpowiedzialnego za przekazywanie wszystkich operacji plikowych wywołanych w module jądra do zdalnej maszyny posiadającej fizyczny egzaemplarz udostępnianego urządzenia. Jest to idealne rowziązanie na potrzeby tego projektu.

### Ze zdalnym urządzeniem

Istnieją dwa podstawowe typy protokołów internetowych: TCP oraz UDP. Oba spełniają kluczową rolę i oba mają swoje wady i zalety dlatego kluczowym jest wybranie odpowiedniego protokołu na porzeby tego projektu.

TCP oznacza "Transmission Control Protocol" i jest protokołem połączeniowym, co oznacza iż obie strony połączenia utrzymują aktywne sesje tak długo jak trwa połączenia które zapewniają między innymi dostarczenie wysłanych wiadomości i ponowne ich przesłąnie w razie problemów z połączeniem oraz otrzymanie ich w poprawnej kolejności. TCP gwarantuje że dane wysłanę przez jedną stronę połączenia dotrą w całości i w poprawnej kolejności albo nie dotrą w ogóle co zazwyczaj spowoduje utratę połączenia i zakończenie sesji. Dodatkowo lub może przede wszystkim TCP pozwala na kontrole szybkości przesyłania danych poprzez modyfikowanie wielkości tak zwanego "okna". TCP na bierząco kontroluje jak szybko druga strona jest w stanie odbierać dane które są przesyłane i modyfikuje wilekość "okna" w celu dopasowania szybkościo wysyłania danych do szybkości z jaką drugi koniec połączenie jest w stanie jest odbierać.

Nadaje się on doskonale do zastosowań w których niedopuszczalne jest utracenie cześci lub całości pojedycyzch pakietów z transmisji. Dzięki kontroli przepływu danych TCP daje programiście pewność iż dane dotrą nienaruszone lub program zostanie poinformowany przez odpowiedni kod błędu o utracie połączenia. Przykładowe protokoły warstwy aplikacji które korzystają z TCP to HTTP, SMTP, IMAP, SSH, FTP i wiele wiele innnych.

Niestety protokół TCP w celu utrzymania swojego standardu niezawodności dodaje zauważalny koszt do komunikacji. Nie tylko w formie dość dużęgo nagłówka dodawanego do każdego pakietu TCP ale również w postaci opóźnień spowodowanych potrzebą ustanawiania sesji przez zestaw pakietów kontrolnych w tak zwanym trzy stopniowym uścisku ("3-way handshake") oraz potrzby odsyłania potwierdzeń o oderbancyh danych. Wszystkie te koszta sprawiają iż TCP nie nadaje się do zastosowań, w których bardziej ceni się działanie w czasie rzeczywistym.

Między innymi z tego powodu powstał protokół UDP, z angielskiego "User Datagram Protocol". W przeciwieństwie do TCP nie gwarantuje on przktycznie nieczego programiście poza tym że będzie szybki. Nie posiada on żadnej kontroli przepływu danych, poprawności danych, sprawdzania kolejności w jakiej dane się pojawiają u odbiorcy czy metody potwierdzenia odbioru. W skrócie UDP jest protokołwem bezpołączeniowym. Daje to jednak szreg zalet które są kluczowe dla takich aplikacji które mają do czynienia z danymi które bardzo szybko tracą ważność. Wiele usług przesyłających obraz wideo lub komunikacje głosową korzysta z UDP z tego właśnie powodu. Aplikacje pozwalające na prowadzenie telekonferencji znacznie bardziej traciły by na używalności gdyby wszystkie dane docierały z prawie sekundowym opóźnieniem niż gdyby pare klatek na minutę zostało upuszczonych w trakcie przesyłania.

Proglem z UDP polega na tym iż sama aplikacja jest teraz odpowiedzialna za obsługiwanie otrzymanych danych w poprawnej kolejności lub weryfikokwanie czy przesłąne dane nie są uszkodzone. Bardzo często robienie tego jest bardziej kosztowne niż było by po prostu użycie bardziej bezpiecznego protokołu TCP. Dodakowo brak kontroli szybkości przepływu danych może łatwo spowodować przepełnienie sieci z której korzysta aplikacja i doprowadzenie jej do stanu, w którym jest nieużywalna.

Pomimo tego że UDP jest powszechnie używane w wielku protokołach kluzowych dla działania sieci lokalnych oraz całego internetu takich jak DNS, DHCP, SNMP czy wiele protokołów dynamicznego routingu to wielu programistów odradza używania UDP w aplikajach użytkowych. Istnieje tylko kilka konkretnych przykłądów w których szybkość komunikacji jest tak kluczowa iż UDP jest naprawdę jedynym sensownym wyjściem. Głównym zastosowaniem jest komunikacja wideo oraz audio między ludźmi w czasie rzeczywistym. W wiekszości innych przypadków UDP powoduje więcej problemów niż rozwiązuje.

Z uwagi na to podjęta została decyzja by użyć protokołu TCP w implementacji serwera który będzie przekazywał operacje na plikach urządzeń znakowych. Ponieważ absolutnie kluczowa jest kolejność w jakiej wykonywane są operacje na urządzenaich fizycznych i pominięcie jednej lub więcej operacji może mieć absolutnie katastrofalne skutki gwarancja jaką daje TCP jeżeli chodzi o dostarczenie wysłanych wiadomości jest niezbędna do poprawnego działania tego projektu.

## Rezerwacja pamięci w jądrze

## Zabezpieczenia przed paniką jądra

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
[b12]: "Linux Kernel - Przewodnik Programisty", str 98-99 i 365-370, Robert Love

# Dodatki
## Zawartość CD
## Spis tabel
## Spis rysunków
