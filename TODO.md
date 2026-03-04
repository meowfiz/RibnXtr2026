## Zadania – RibnXtr2026

Ten plik zawiera **bieżącą listę zadań** dla projektu, wspólną dla obu maszyn.

### Konwencja
- Zadania zapisujemy w formie listy:
  - `[ ]` – zadanie do zrobienia,
  - `[~]` – zadanie w toku,
  - `[x]` – zadanie ukończone.
- Gdy jakieś zadanie wymaga więcej kontekstu, dodajemy odnośnik do odpowiedniego dziennika w `notes/` albo do sekcji w `PROJECT_NOTES.md`.

### Do zrobienia
- [ ] Ustalić i spisać architekturę głównych modułów w `PROJECT_NOTES.md`.
- [ ] Dopracować opis standardowego workflow buildowania na obu maszynach (w oparciu o skrypty w `tools/`).
- [ ] Rozszerzyć opis pipeline’u `JadraSegmentator` (wejście/wyjście, struktura masek, etykietowany wolumen) w `PROJECT_NOTES.md`.

### W toku

### Zrobione
- [x] Ujednolicono konfigurację builda i dokumentacji na obu maszynach (skrypty w `tools/`, CMake, README).
- [x] Skonfigurowano i przetestowano działanie `JadraSegmentator` na machine 2 (w tym logowanie i maski `.jadra.mask`).
