# PROJECT - PACKAGE DELIVERY

![game banner](/screenshots/game_banner.jpg)

## Game objective
The game involves driving a _parcel-bot_ around the map, collecting parcels and delivering them to a parcel locker.
<br>The robot can pick up a maximum of **3 parcels**, so after collecting the maximum number of parcels it can carry, it must deliver them to the parcel locker and then return for the rest.
<br>The game ends when the robot finds and delivers all the parcels scattered around the scene to the parcel locker.

## Rendering
1. Loading 3D objects from external files (e.g., .OBJ files)
2. Textures and transparent textures (e.g., quality)
3. Implementation of materials (shiny matte)
4. Implementation of directional lighting (e.g., sun) or point lighting (e.g., lantern)
5. Implementation of shadows in directional or point lighting
6. Text on screen (e.g., information about points, game duration)
7. Off-screen rendering (e.g., minimap, mirror)
8. Implementation of skybox
9. Implementation of environment mapping (e.g., reflection, refraction)
10. Utilization of geometry shader

## Scene
1. Ground - a surface grid with movement implementation on it
2. Static and moving objects (e.g., rotating/moving)
3. Multiple objects generated using instance rendering method (e.g., grass, flowers)
4. Collision objects with our character (camera) and non-collision objects
5. Good and bad objects (increasing and decreasing the game's objective function value)
6. Our character moving through the scene, first-person or third-person view

## Additionals
Implementation of a **context menu**, where we have the possibility to change certain game options, including:

1. Turning on/off lighting
2. Turning on/off shadows
3. Turning on/off skybox
4. Changing resolution mode (e.g., fullscreen)

## Keyboard controlls

| Key     | controlls **PLAYER VIEW**    | controlls **FREE CAM**   |
| ------- | --------------------------- |-------------------------  |
| W       | go forward **player**       | go forward **camera** |
| S       | go backwards **player**     | go backwards **camera**   |
| A       | rutn left **player**        | ruen left **kamerą**   |
| D       | turn right **player**       | turn right **kamerą** |
| SHIFT   | acceeleration **player**    | descend **kamerą**    |
| SPACE   | -                           | ascend **kamerą**    |
| 🢁      | -                           | go forward **player**|
| 🢃      | -                           | go backwards **player** |
| 🢀      | -                           | turn left **player** |
| 🢂      | -                           | turn right **player** |

## Menu
|                                                     |                                                        |
| --------------------------------------------------- | ------------------------------------------------------ | 
| ![](/screenshots/menu.jpg)             | ![](/screenshots/menu_hud_options.jpg)    |
| ![](/screenshots/menu_player_view.jpg) | ![](/screenshots/menu_video_settings.jpg) |


## Player view
|                                                     |                  |
| --------------------------------------------------- | ---------------- |
| ![](/screenshots/player_view_1-person.jpg) | First Person |
| ![](/screenshots/player_view_3-person.jpg) | Third Person | 
