import cv2


ROT_DISABLED = 0
ROT_90 = 1
ROT_ANY = 2

#flip - tuple z osiami do flipowania (czy flipy przy segmentacji ktora rozróżnia lewą i prawą strone to dobry pomysł?)
#rotation - co 90 stopni, w przypadku całego zakresu będą obcięte rogi - zoomować, zignorować czy tak nie obracać?
# ? jednoczesny flip (w x i y) i rotacje co 90 stopni generują 16 możliwych kombinacji przekształceń

#strech - delikatny, niezależny w x,y,z o kilka procent
#crop/zoom - zoom to w sumie crop, sam zoom niewiele robi, bo finalnie obraz przekształcam do 256 x 256. 

#random noise - jak zaszumiać? Normalizuje dane przed nauką, więc pewnie jakąś stałą wartością a nie procentowo

#filters (median, gaussian, ?) - czy jest sens w danych medycznych?

#random erasing part of image (bardziej do autoencodera?) - czy jest sens bez autoencodera?

#mixing images? - nie leży mi zbytnio w danych medycznych, ewentualnie mógłbym blendować sąsiadujące slice'y, ale lepiej chyba po prostu użyć wycinka i na jego podstawie uczyć.
#                 ponadto i tak finalnie chcę uczyć na 3D.

class Augmentator:
    #flip - list / tuple of flipable axes
    #rotation - ROT_DISABLED, ROT_90, ROT_ANY
    def __init__(self, flip=None, rotation=ROT_DISABLED):
        self.flip = flip
        self.rotation = rotation


    def apply(self, img):
        if self.flip:
            pass
            # cv2.flip(img, cv2.)