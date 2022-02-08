# PROJECT - PACKAGE DELIVERY

![game banner](/_GameProject/screenshots/game_banner.jpg)

## Cel gry
Gra polega na tym, że jeździmy *paczko-robotem* po mapie i zbieramy paczki i odwozimy je do paczkomatu. 
<br>Robot maksymalnie może podnieść **3 paczki** czyli po zebraniu maksymalnej ilości paczek, którą robot może podnieść należy je odwieźć do paczkomatu i wrócić po pozostałe.
<br>Rozgrywka kończy się wtedy, kiedy robot znajdzie i odwiezie wszystkie paczki porozrzucane po scenie do paczkomatu.

## Rendering

1. Wczytywanie obiektów 3D z zewnętrznych plików (np. plików .OBJ): **✔**
2. tekstury i tekstury przezroczyste (np. kwaity) **✔**
3. implementacja materiałów (błyszczące matowe) **✘** / **✔**
4. implementacja oświetlenia kierunkowego (np. słońce) lub punktowego (np. latarnia) **✔**
5. implementacja cieni w oświetleniu kierunkowym lub punktowym **✔**
6. tekst na ekranie (np. informacje o punktach, czas trwania gry) **✔**
7. rendering pozaekranowy (np. minimapa, lustro) **✔**
8. implementacja skyboxa **✔**
9. implementacja enviroment mappingu (np. odbicie, refrakcja) **✔**
10. wykorzystanie shadera geometrii **✘**

## Scena

1. podłoże - siatka powierzchni z implementacją poruszania się po niej **✔**
2. obiekty statyczne i ruchome (np. obracające się/ przesuwające się) **✔**
3. wiele obiektów wygenerowanych metodą *instance rendering* (np. trawa, kwiaty) **✔**
4. obiekty kolizyjne z naszą postacią (kamerą) i obiekty niekolizyjne **✔**
5. obiekty dobre i złe (zwiększające i zmniejszające wartość funkcji celu gry) **✔**
6. nasza postać, która porusza się po scenie, widok pierwszoosobowy lub trzecioosobowy **✔**

## Inne wymagania

Zaimplementowanie **menu kontekstowego**, w którym mamy możliwość zmiany niektórych opcji gry, m.in.

1. włączanie/wyłaczanie świetlenia **✔**
2. włączanie/wyłaczanie cieni **✔**
3. włączanie/wyłaczanie skyboxa **✔**
4. zmiana trybu rozdzielczości (np. fullscreen) **✔**

## Klawiszologia

| Klawisz | czynność w **PLAYER VIEW**  | czynność w **FREE CAM**   |
| ------- | --------------------------- |-------------------------  |
| W       | ruch do przodu **postacią** | ruch do przodu **kamerą** |
| S       | ruch do tyłu **postacią**   | ruch do tyłu **kamerą**   |
| A       | skręt w lewo **postacią**   | skręt w lewo **kamerą**   |
| D       | skręt w prawo **postacią**  |  skręt w prawo **kamerą** |
| SHIFT   | przyśpieszenie **postacią** |  ruch w dół **kamerą**    |
| SPACE   | -                           | ruch w górę **kamerą**    |
| 🢁      | -                           | ruch do przodu **postacią**|
| 🢃      | -                           |  ruch do tyłu **postacią** |
| 🢀      | -                           | skręt w lewo **postacią** |
| 🢂      | -                           | skręt w prawo **postacią** |

## Menu
|                                                     |                                                        |
| --------------------------------------------------- | ------------------------------------------------------ | 
| ![](/_GameProject/screenshots/menu.jpg)             | ![](/_GameProject/screenshots/menu_hud_options.jpg)    |
| ![](/_GameProject/screenshots/menu_player_view.jpg) | ![](/_GameProject/screenshots/menu_video_settings.jpg) |


## Player view
|                                                     |                  |
| --------------------------------------------------- | ---------------- |
| ![](/_GameProject/screenshots/player_view_1-person.jpg) | First Person |
| ![](/_GameProject/screenshots/player_view_3-person.jpg) | Third Person | 
