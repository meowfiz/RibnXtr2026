# RibnXtr2026 (CMake + VS2022)

Projekt budowany przez CMake. Kod + CMake są w repo, a lokalne ścieżki do zależności są **poza repo** w `deps.cmake`.

## Zasada
- `CMakeLists.txt` jest wspólny na wszystkie maszyny.
- `deps.cmake` jest lokalny (NIE commitujemy).
- `deps.cmake` jest ładowany przez `CMAKE_PROJECT_TOP_LEVEL_INCLUDES`.

Repo zawiera też `cmake/Modules` (m.in. `FindKinectSDK2.cmake`) potrzebne do `find_package(KinectSDK2)`.

---

## Maszyna 2 — start (krok po kroku)

### 1) Clone
```powershell
git clone https://github.com/meowfiz/RibnXtr2026
cd RibnXtr2026