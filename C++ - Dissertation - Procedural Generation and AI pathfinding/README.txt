This is a readme file for the Artificial Intelligence in a procedurally generated world project application

Controls

Arrow keys to move along the x and y coordinate of the map. Move the mouse cursor to the button of the screen to access the configuration menu

Configuration buttons (moving from left to right)

Mountaineer - Configures the costs of terrain to to prefer passing over mountainous and other elevated areas.

Amphibian - Configures the costs of terrain to prefer passing over deep and shallow water.

Prefers Grass - Configures the costs of terrain to prefer passing over grass.

Stay on contours - When enabled changes the agent behaviour to aim to stay on the same level and avoid ascending or descending at any point.

Note that only one of the above four options may be enabled at any oen time as they all affect the agent behaviour.

Pheromone (ant) - When enabled this button configures the mouse click to place a pheromone on that grid location in the view, the pheromone will affect a certain number of grid locations around it with different values depending on their distance from grid location that the pheromone was placed. The agent will be attractd to these pheromones except for when the stay on contours setting is enabled.

Goal node / Blocker (path nodes / no entry sign) - This is a toggle button and will take affect providing the pheromone button is not enabled and overiding them. This button will alternate the mouse pointer between placing the goal node and placing blockers for grid locations that are clicked on in the view.

Current states list

Gradients - Current number of grid locations being calculated
posX, posY - current position in the world
viewOffsetX, viewOffsetY - current offset from the nearest 	gradient vector origin point
Cost of path - current cost of the highlighted path (represented by the red path node circles in the view)

Generating a new world

On the very right of the panel is the New World Seed button, this is cause the current world to change and may be useful for finding a desired piece of terrain quickly without having to scroll through the world. 

Note a known issue with this is that on occasion the world seed will generate a very obvious linear looking world full of different types of terrain perfectly lining up together. When this happens it is recommended to click the New World Seed button again to find a more suitable seed.

