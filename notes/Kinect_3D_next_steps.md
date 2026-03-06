# Kinect 3D – stan i dalsze kroki

## Co zrobione (sesja do 2026-03)

- **Wątek 3D** – `KinectDraw3DThread` jest uruchamiany przy włączeniu checkboxa Kinect (`kinect3DThread->start()` w `SlotKinectEnable`) i zatrzymywany przy wyłączeniu (`requestInterruption()` + `wait()`).
- **Bezpieczeństwo wskaźników** – `kinectThread`, `kinect3DThread`, `cameraThread` w `MainWindow.h` inicjalizowane na `nullptr`, żeby uniknąć crasha przy wczesnym wywołaniu slotu (np. przy przywracaniu stanu GUI).
- **Pętla wątku 3D** – `run()` używa `while (!isInterruptionRequested())`, żeby wątek mógł się poprawnie zakończyć.
- **SlotKinectFrustumGen()** – odkomentowana uproszczona wersja: alokacja i liniowe wypełnienie `xPosDepthBigTab`, `yPosDepthBigTab`, `zPosDepthBigTab`, `maskData1` (mapowanie woksel → przestrzeń 0–65535 / 0–255). Przycisk FrustumBox działa bez crasha.
- **Aktualizacja maski w 3D** – w `KinectDraw3DThread::SlotRenderKinectDepthMap()` odkomentowany blok: pobranie depth z `depthBuffers[0]`, mapowanie przez `homographyLookup`, wpis do `MaskData`, na końcu `MaskVolumeStructuredPoints->Modified()`, `VolumeMapper->SetMaskInput(...)`, `Interactor->update()`.
- **Logowanie** – co ~2 s wpis do logera: `[Kinect VolMask] voxels masked=... total=... (...%)` (przez `QMetaObject::invokeMethod` → `SlotAddLogText2`).

## Stan obecny

- Aplikacja nie crashuje; Kinect 2D działa (podgląd depth, FPS).
- Checkboxy: Kinect, Volume Mask, (3DVis) Volume Mask – zaznaczone.
- W widoku 3D **brak wyraźnego efektu** – tylko słaby, rozmyty kształt; użytkownik nie widzi reakcji na ruch ręką.

## Plan na później (gdy wrócimy do tematu)

1. **Sprawdzić, czy ścieżka w ogóle się wykonuje**  
   - Czy w logu pojawiają się wpisy `[Kinect VolMask]`?  
   - Jeśli nie – który warunek w `SlotRenderKinectDepthMap()` odcina wykonanie (np. `homographyLookup == NULL`, `tryLock()` nie wychodzi, brak wolumenu / FrustumBox nie kliknięty)?

2. **Mapowanie frustum**  
   - Obecnie w `SlotKinectFrustumGen()` używane jest **uproszczone liniowe** mapowanie (woksele → równomiernie w przestrzeń 0–65535).  
   - Docelowo: przywrócić lub przepisać **prawdziwą projekcję VTK** (widok kamery 3D → współrzędne w obrazie depth Kinecta), żeby maska odpowiadała realnemu ułożeniu wolumenu względem sensora.  
   - Plik z analizą: `project_files/.../notes/Kinect_volume_sculpting_analysis.md` (linie dla starego `SlotKinectFrustumGen` i bloku maski).

3. **Homografia / lookup**  
   - Sprawdzić, czy `homographyLookup` jest wypełniany i dla jakiego układu (Kinect 2, rozmiary 512×424).  
   - Zweryfikować indeksowanie i `qBound` w pętli maski (xa, ya, xa1, ya1).

4. **Wartości maski i depth**  
   - Dla uproszczonego mapowania `zPosDepthBigTab` i depth z Kinecta mogą być w różnych skalach – sprawdzić logowane `masked`/`total` i ewentualnie skalowanie / próg w `MaskData[pos] = ((depth > val) || (val == 255)) ? 255 : 0`.

5. **VTK / VolumeMapper**  
   - Upewnić się, że `VolumeMapper->SetMaskInput(MaskVolumeStructuredPoints)` i `Modified()` + `Interactor->update()` są wywoływane w odpowiednim momencie i że widżet 3D faktycznie używa maski (np. opcja „use mask for 3D view” / 3DVis).

6. **Opcjonalnie**  
   - Logowanie przy wczesnym wyjściu z `SlotRenderKinectDepthMap()` (np. powód skipu, throttlowane), żeby diagnozować, gdy wpisy `[Kinect VolMask]` się nie pojawiają.

---

Przy następnej sesji: otworzyć ten plik i kontynuować od punktu 1 (logi), potem 2 (frustum) i 3 (homografia).
