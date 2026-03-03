W paczce są następujące, ważne skrypty:


	src/utils/rdata_to_numpy.py - pozwala na przetworzenie wolumenu .rdata na wolumen numpy

	src/segmentation.py   - przetwarza wolumen numpy na wolumen z maskami (też numpy). Wymaga ścieżki do wolumenu w formacie numpy, configu.yaml i checkpointu.pth z wagami sieci. 
				Docelowo z argumentów wypadnie .yaml, bo będzie zawarty w checkpoint.pth
				Skrypt na podstawie checkpoint.pth buduje sieć, wczytuje wagi i przetwarza wejściowy wolumen. 