Advanced Technologies: Blueprint level generation

The prupose of this project is to demonstrate a model that is capable of building a level from a blueprint image file, this particular model is also capable of producing some detailed terrain from a blueprint image aswell as a multistory level.

Developed with: Unity v5.0.1

The project can be accessed by opening the project folder in unity (5.0.1 recommended version) or by running the executable files also provided in the project folder.

There are two executables for demonstrating different features, one for a multistory level using multiple blueprints and combining them and one for generating terrain from a blueprint. 

Furthermore you can toggle parameters in unity:

To toggle the various features open the project in unity, load the "DemoScene" scene and click on the "Landscape" object, here you will see a script attached with some parameters that can be toggled.

Image 1, 2, 3 (Texture2D) - These are the blueprints that can be assigned, each blueprint is a seperate level and can be changed or commented out in the "Start()" function of the "blocks" script, more than one blueprint being loaded will result in the level being multistory.

Height(integer) - The maximum height of the mountains if the terrain boolean is true.

Wall Height(integer) - The maximum height of walls loaded in from a blueprint file

Terrain Gen(boolean) - Dictates whether or not the landscape is loaded as terrain and applies textures to different heights if so, to test this try appliing the "level" asset to image 1 and disabling image 2 and 3 in the "Start()" function

Creator: Grant Medlyn
12016119





