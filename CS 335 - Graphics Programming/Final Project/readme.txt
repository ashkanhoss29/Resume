Ashkan Hosseini
Ken Imai

Instruction to run:
This program runs on Netbeans 7.1.2 with JOGL library.
No other libraries are required.
Copy the folder titled CS335_Program_Final into the NetbeansProjects, open Netbeans and open it.

Implementation:

Graphics (code):
For the graphics, a programmable pipeline model has been used.
There are three render paths each with seperate shaders: skybox, models, and postprocess.
For skybox, Opengl Cube_Map_Texture was used with textureCube in the shader.
For the models, we use the cube map generated for the skybox to perform per pixel reflection.
Finally, for postprocess, the scene is rendered to color and depth textures using Opengl FrameBuffer Objects and sent to postprocessing shaders, where depth of field is performed.

Graphics (models):
A combinations of three tools were used to make and display models: obj importer, Autodesk Maya, and Autodesk Mudbox.
The obj loader was hand written, and supports triangulated models.
Base models were made in Maya, then they were imported to Mudbox for texturing.
Finally, obj was exported from Maya and textures exported from Mudbox, then both were imported into our program.

Camera:
The camera movement uses gluLookAt.
Two positions are calculated in world space: camera position and look at position.
When the user moves the camera forward and backwards, the camera position and look at position move together in the same direction.
When the user turns the camera side to side, the camera position does not change, but only the look at position is effected.
The base camera was developed for the free mode and then integrated with the car movement, so the positions are set by the car movement.

Physics:
For the physics, we did collision detection and AI movement based on lines of edges imported by obj importer.
So, the coordinates needed to be hard coded.
Player car gets bouncy because the lines of edges are not a perfect line.
For extra credit, we did a treasure boost. Only player car gets it and gets the double max speed for 10sec.
While the car is boosting up, the treasure is dissappeared and after the 10sec, it comes up again.


AI:
For AI movement, AIs can correct their direction angle, so after the collisions, AI cars move the way they supposed to be.
For collision, we used elastic collision, so when a car hits a wall, it does not lose any speed. But the angle changes.
For car to car collision, the speed of cars and the angle switches, but after a certain time, AI cars speed back to the default.
For loop timer, we had a white line on the track to detect the loop start time and end time. It also counts the number of loop.
