## RibnXtr2026 – Notatki projektowe

Ten plik zawiera **stały kontekst projektu**: ogólny opis, architekturę, ważne decyzje i linki do innych dokumentów. Aktualizujemy go wtedy, gdy zmienia się struktura projektu albo podejście architektoniczne.

### Zakres
- Cele i opis projektu.
- Zarys architektury (moduły, biblioteki, główne komponenty).
- Kluczowe decyzje techniczne i ich uzasadnienie.
- Odnośniki do innych plików (`README.md`, `TODO.md`, katalog `notes/`).

---

## 1. Cel i ogólny opis

RibnXtr2026 to aplikacja **desktopowa (Windows)** z GUI opartym o **Qt5**, przeznaczona do:
- wizualizacji i przetwarzania danych 3D (głównie wolumetrycznych, medycznych),
- analizy i segmentacji struktur (np. tętnice, kości, inne narządy),
- rejestracji obrazów (volumetrycznej i powierzchniowej),
- integracji z czujnikami **Kinect v2** oraz innymi źródłami obrazu (kamera, wideo, dane DICOM, NIfTI itd.).

Aplikacja jest rozwijana jako kontynuacja starszego projektu (`RibnXtr2022`), którego kod źródłowy i pliki uruchomieniowe znajdują się w katalogu `project_files/RibnXtr2022-project_files/ribnxtr2_Files`.

---

## 2. Architektura – widok z góry

### 2.1. Główne komponenty

- **Warstwa GUI (Qt5)**  
  - Główne okno aplikacji: `MainWindow` (`MainWindow.h/.cpp`) – centrum logiki aplikacji.
  - Widżety:
    - `VtkWidget`, `View2DSliceWidget`, `ImageWidget`, `HistWidget`, `GUIPanelWidget`, `PropertyWidget` itd.
    - Widoki 2D/3D, okna dodatkowe, panele z parametrami, logi, itp.
  - Wątki GUI i dane:
    - Klasy `KinectDrawThread`, `KinectDraw3DThread`, `CameraDrawThread` – osobne wątki rysujące/odświeżające dane z czujników.

- **Silnik przetwarzania danych wolumetrycznych i masek**  
  - Klasy danych:
    - `RawDataSet`, `RawDataLayer`, `DistanceMap`, `VolumeElasticRegistration`, `ImageSetRegistration`, `ImageSetElasticRegistration` itd.
  - Operacje na maskach i wolumenach:
    - Segmentacja, morfologia (erozja/dylacja), generowanie masek 3D, statystyki, operacje boolean, generowanie masek z różnych źródeł.
  - Funkcje związane z filtrowaniem:
    - `LayerGenericFilter`, `LayerFilters`, `Filters3D`, `HybridMedianFilter`, `OtherImageFunctions`, `OtherCommonFunctions`, `OtherCommonFunctionsVTK`.

- **Silnik powierzchni i geometrii 3D (VTK)**  
  - Dane powierzchniowe:
    - `SurfaceData`, `SurfaceCollection`, `PolygonSurfaceData`, `SurfaceProcessing`, `SegCriterion`, `ExplodeSegmentation` itd.
  - Reprezentacja:
    - `vtkPolyData`, `vtkActor`, `vtkVolume`, `vtkGPUVolumeRayCastMapper`, różne typy mapperów i filtrów VTK.
  - Operacje:
    - Generowanie powierzchni (np. marching cubes), wygładzanie, decymacja, sortowanie, ekstrakcja wysp, deformacje, wycinanie, generowanie map odległości, map błędu, voxelizacja powierzchni.

- **Rejestracja obrazów i powierzchni (ITK / własne algorytmy)**  
  - ITK/TubeTK:
    - uogólnione klasy typu `ImageSetRegistration`, `ImageSetElasticRegistration`, `SurfaceRegistration`, `SurfaceElasticRegistration`, `VolumeElasticRegistration`, `Registration` itd.
  - Rejestracja:
    - dopasowywanie wolumenów, powierzchni, linii szkieletowych, ekstrakcja punktów cechowych, obliczanie misregistracji i map różnic.

- **Obsługa czujników i wideo (Kinect, kamera, FFmpeg, QCamera)**  
  - Kinect v2:
    - integracja poprzez `IKinectSensor`, `IDepthFrameReader`, `IColorFrameReader`, `ICoordinateMapper`.
    - Bufory głębi, mapowanie do przestrzeni kolorów, generowanie chmury punktów, maskowanie, kalibracja.
  - Kamera/QCamera:
    - `QCamera`, `QCameraImageCapture`, `QMediaRecorder`, `QVideoEncoderSettings`.
  - Wideo/FFmpeg:
    - Klasa `FFMPEGTools` – zapis/odczyt sekwencji, generowanie animacji, nagrywanie i odtwarzanie.

- **Integracja z zewnętrznymi narzędziami i skryptami**  
  - Python / segmentatory:
    - Ścieżki do skryptów i modeli (np. `jadraSegmentatorPythonPath`, `jadraSegmentatorScriptPath`, `jadraSegmentatorConfigPath`, `jadraSegmentatorCheckpointPath`).
    - Sloty typu `SlotGenerateMaskUsingNeuralSegmentator()`, `SlotGenerateMaskUsingJadraSegmentator()`.
  - Zewnętrzne programy:
    - `dopplerSegmentatorPath`, `dopplerSegmentatorModelPath`, `ffplayexePath`, itp.

- **Analiza naczyń / tętnic**  
  - Klasy i funkcje:
    - `ArteryData`, `HeartSimulator`, operacje typu `SlotArteryTrace`, `SlotArterySegmentation`, `SlotGenerateArteryMesh`, `SlotGenerateArteryResults`, `SlotGenerateSurfaceDifferenceMapForTubes`, itd.
  - Dane:
    - śledzenie tętnic, obliczanie średnic, pól przekrojów, gęstości, statystyk i parametrów geometrycznych.

---

## 3. Biblioteki i zależności (CMake)

Konfiguracja odbywa się przez główny `CMakeLists.txt` (projekt `RibnXtr2023`):

- **Qt5**  
  - Wymagane komponenty:
    - `Core`, `Gui`, `Widgets`, `Network`, `Multimedia`, `MultimediaWidgets`.
  - Włączenie automatycznego MOC/UIC:
    - `CMAKE_AUTOMOC`, `CMAKE_AUTOUIC` ustawione na `ON`.
  - Ścieżki:
    - `Qt5_DIR` konfigurowane w `deps.cmake` (poza repo),
    - alternatywnie używany `QTDIR` z env jako `CMAKE_PREFIX_PATH`.

- **OpenMP**  
  - Używany do przyspieszenia obliczeń (pętle równoległe w kodzie C++).

- **Kinect SDK v2**  
  - Znajdowany przez własny moduł `FindKinectSDK2.cmake` z katalogu `cmake/Modules`.
  - Tworzy target `KinectSDK2::KinectSDK2` używany do linkowania.

- **OpenCV**  
  - Używany do przetwarzania obrazów 2D/3D, filtrów, analizy.

- **ITK (i TubeTK)**  
  - Korzystamy z `find_package(ITK REQUIRED)` oraz `include(${ITK_USE_FILE})`.
  - Używane w rejestracji obrazów, przetwarzaniu wolumenów, analizie kształtów.

- **Armadillo**  
  - Biblioteka algebry liniowej, wykorzystywana m.in. do operacji na macierzach (`arma::mat H` w `MainWindow`).

- **QtXlsx**  
  - Do generowania raportów w formacie `xlsx` (`QXlsx::Document xlsx` w `MainWindow`).

- **FFmpeg (opcjonalnie)**  
  - Włączany przez zmienne w `deps.cmake`:
    - `FFMPEG_INCLUDE_DIR`, `FFMPEG_AVCODEC_LIB`, `FFMPEG_AVFORMAT_LIB`, `FFMPEG_SWSCALE_LIB`, `FFMPEG_AVUTIL_LIB`.
  - Jeśli nie są ustawione, CMake ustawia puste ścieżki – linkowanie je „przełknie”, ale funkcjonalność FFmpeg będzie wyłączona.

---

## 4. Struktura katalogów (wysoki poziom)

- `CMakeLists.txt` – główny plik budowania (projekt `RibnXtr2023`).
- `cmake/Modules` – własne moduły CMake (np. `FindKinectSDK2.cmake`).
- `project_files/RibnXtr2022-project_files/ribnxtr2_Files/`:
  - `src/include` – nagłówki C++ (GUI, algorytmy, modele danych).
  - `src/source` – implementacje C++ (logika aplikacji, GUI, filtry, rejestracja, obsługa czujników).
  - `run_files` – konfiguracje, presety, pliki `.layers`, `.prop`, pliki z ustawieniami opacities (`volopacity`), skrypty, sample danych, itp.
- `tools/` – (w repo) skrypty PowerShell do konfiguracji/projektów Visual Studio i budowania (np. `build-vs2022-debug.ps1`, `configure-vs2022.ps1`).
- `README.md` – opis setupu CMake i zasady dotyczące `deps.cmake`.
- `PROJECT_NOTES.md` – ten plik (opis architektury, decyzji).
- `TODO.md` – lista zadań.
- `notes/` – dzienne dzienniki sesji (`YYYY-MM-DD-sesja.md`).

---

## 5. Budowanie projektu (skrót)

Szczegóły i kroki są w `README.md`, tu tylko **zarys**:

- Wymagane narzędzia:
  - CMake (min. 3.17),
  - Visual Studio 2022 (lub 2019) z toolsetem MSVC,
  - zainstalowane biblioteki z sekcji „Biblioteki i zależności”.
- Lokalne ścieżki do bibliotek konfigurowane w **`deps.cmake`** (poza repo, nie commitujemy):
  - Ścieżki do Qt5, OpenCV, ITK, Armadillo, QtXlsx, Kinect SDK 2, FFmpeg itd.
- Projekt posiada skrypty w `tools/` upraszczające:
  - konfigurację projektów VS (`configure-vs2022.ps1`, `configure-vs2019.ps1`),
  - budowanie w trybach Debug/Release (`build-vs2022-debug.ps1`, `build-vs2022-release.ps1`, itd.).

---

## 6. Praca na dwóch maszynach

- Repozytorium git jest **jedynym źródłem prawdy** (kod + konfiguracja CMake + notatki).
- Asystent AI korzysta z:
  - `PROJECT_NOTES.md` (architektura, decyzje),
  - `TODO.md` (zadania),
  - `notes/YYYY-MM-DD-sesja.md` (dzienniki)  
  jako głównego kontekstu **i jest odpowiedzialny za ich bieżącą aktualizację w trakcie pracy** (bez konieczności przypominania).
- Po zakończeniu dnia:
  - aktualizujemy `TODO.md` oraz dzisiejszy plik w `notes/`,
  - commitujemy zmiany i robimy `git push`,
  - druga maszyna po `git pull` ma pełen kontekst.

---

## 7. Zasady commitów (Git)

- Każdy commit powinien mieć:
  - **krótką, jednowierszową nazwę** (summary) – po polsku, opisującą sens zmiany (np. „Ujednolicenie konfiguracji builda i dokumentacji na obu maszynach”),
  - **dłuższy opis (body)** w kilku punktach, zaczynających się od `- `, opisujących co konkretnie zostało zmienione.
- Dzięki temu:
  - w **GitHub Desktop** w zakładce `History` widoczny jest zarówno tytuł, jak i pełny opis,
  - łatwo odtworzyć kontekst zmian po czasie (co się działo w poszczególnych dniach/sesjach).
- Jeśli proszę asystenta o wykonanie commita:
  - commit ma być przygotowany właśnie w tym formacie (tytuł + punktowany opis),
  - push na GitHub odbywa się **tylko na wyraźną prośbę**, inaczej commit zostaje lokalnie.

---

## 8. Zamykanie dnia pracy („kończę pracę na dziś”)

- Ustalone hasło: **„kończę pracę na dziś”** (lub bardzo podobne sformułowanie) oznacza:
  - asystent:
    - aktualizuje dzisiejszy dziennik w `notes/YYYY-MM-DD-sesja.md` (sekcje: plan/co zrobione/notatki),
    - w razie potrzeby domyka `TODO.md` (przenosi wykonane rzeczy do „Zrobione”),
    - przygotowuje i wykonuje **jeden commit** z czytelnym opisem dnia,
  - użytkownik:
    - może następnie wykonać `Push` w GitHub Desktop (lub poprosić asystenta o `git push`).
- Dzięki temu po zakończeniu dnia:
  - stan kodu + notatki z dnia są zawsze utrwalone w jednym commicie,
  - druga maszyna widzi pełen kontekst po zwykłym `pull`.
