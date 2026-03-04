# RibnXtr2026 – Setup i Build (VS2019 / VS2022)

Ten dokument pozwala odtworzyć środowisko projektu na nowej maszynie.
Jeśli wszystko zapomnisz – wystarczy przeczytać ten plik i wykonać kroki.

Projekt używa:
- CMake
- Visual Studio generator
- lokalnego pliku deps.cmake z zależnościami

Projekt działa na:
- Visual Studio 2019
- Visual Studio 2022

--------------------------------------------------

1. Lokalizacja projektu

Repo powinno być w:

D:\programming\RibnXtr2026

--------------------------------------------------

2. Struktura repo

RibnXtr2026
│
├─ CMakeLists.txt
├─ CMakePresets.json
├─ README.md
│
├─ cmake
│   └─ Modules
│       └─ FindKinectSDK2.cmake
│
├─ tools
│   ├─ configure-vs2019.ps1
│   ├─ configure-vs2022.ps1
│   ├─ build-vs2019-debug.ps1
│   ├─ build-vs2019-release.ps1
│   ├─ build-vs2022-debug.ps1
│   └─ build-vs2022-release.ps1
│
└─ (tu pojawią się katalogi build)

--------------------------------------------------

3. Plik deps.cmake (lokalny)

W katalogu głównym projektu musi istnieć:

D:\programming\RibnXtr2026\deps.cmake

Ten plik NIE jest w repo.
Zawiera ścieżki do bibliotek.

Przykład:

set(OpenCV_DIR "D:/tools/opencv/build")
set(Qt_DIR "D:/tools/Qt/...")
set(KinectSDK2_DIR "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409")

Biblioteki używane w projekcie:
- OpenCV
- Qt
- Kinect SDK
- OpenMP
- ITK
- TubeTK
- VTK (ładowane przez ITK)

--------------------------------------------------

4. Ważne decyzje w CMake

VTK
Nie wolno robić:

find_package(VTK REQUIRED)

VTK jest już ładowane przez ITK / TubeTK.
Dodanie tego drugi raz powoduje błąd:
targets already defined

OpenMP

W CMake musi być kolejność:

project(... LANGUAGES C CXX)
find_package(OpenMP)

--------------------------------------------------

5. Pierwsze uruchomienie na nowej maszynie

Otwórz PowerShell

cd D:\programming\RibnXtr2026

Sprawdź czy istnieje deps.cmake:

Test-Path .\deps.cmake

powinno zwrócić:
True

--------------------------------------------------

6. Configure

Maszyna z VS2019

.\tools\configure-vs2019.ps1

Maszyna z VS2022

.\tools\configure-vs2022.ps1

Powstanie katalog:

build-vs2019
lub
build-vs2022

W środku:

RibnXtr2026.sln

--------------------------------------------------

7. Build

VS2019 Debug

.\tools\build-vs2019-debug.ps1

VS2019 Release

.\tools\build-vs2019-release.ps1

VS2022 Debug

.\tools\build-vs2022-debug.ps1

VS2022 Release

.\tools\build-vs2022-release.ps1

--------------------------------------------------

8. Praca w Visual Studio

Otwórz solution:

build-vs2019\RibnXtr2026.sln
lub
build-vs2022\RibnXtr2026.sln

Pracujesz normalnie:
Debug / Release
Build
F5

--------------------------------------------------

9. Git ignore

Te pliki NIE powinny być w repo:

deps.cmake
build-vs*
.vs
CMakeCache.txt
CMakeFiles

--------------------------------------------------

10. Typowy workflow

cd D:\programming\RibnXtr2026

.\tools\configure-vs2019.ps1
.\tools\build-vs2019-release.ps1

Otwórz:

build-vs2019\RibnXtr2026.sln

--------------------------------------------------

11. Jeśli build się popsuje

Najczęstsze przyczyny:
1. złe ścieżki w deps.cmake
2. brak biblioteki
3. zmiana generatora
4. uszkodzony cache

Reset:

Remove-Item build-vs2019 -Recurse -Force
.\tools\configure-vs2019.ps1