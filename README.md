simple-shader
=============

This is an implementation of a simple Phong shader in C++ using OpenGL for OS X. The code also works in Windows, but the Windows version may be buggy and is not supported. Some sample images created with this code can be seen [here](http://inst.eecs.berkeley.edu/~cs184-dj/as1.html)

To build the code, clone this repository and use the "make" command.

## Specifying Material Properties

### Ambient Shading
An ambient coefficient can be specified as a command line argument with '-ka' followed by the rgb values of the ambient coeficient (between 0 and 1)
E.g.:
'-ka 0.1 0.1 0.1'

### Diffuse Shading
A diffuse coefficient can be specified with '-kd' followed by the rgb values of the diffuse coeficient (between 0 and 1)
E.g.:
'-kd .8 .5 .2'

### Specular Shading
A specular coefficient can be specified with '-ks' followed by the rgb values of the specular coefficient. A specular exponent can be specified with '-sp' followed by a number 0 to maxfloat. The greater the specular exponent the "shiner" the object will appear
E.g.:
'-ks .8 .8 .8 -sp 10'

## Specifying Light Sources
This shader supports up to 5 point lights and 5 directional lights

### Point lights
A point light can be added using the command line argument '-pl' using the following format:
'-pl x y z r g b'

(x,y,z) is the location of the point light and (r,g,b) is the intensity. Values can be between 0 and maxfloat

### Directional lights
A point light can be added using the command line argument '-pl' using the following format:
'-pl x y z r g b'

(x,y,z) represents the direction of the light as a vector and (r,g,b) is the intensity. Values can be between 0 and maxfloat

## Extra features

### Torus
Instead of rendering a sphere, we can render a torus if we add "-tor x y" to the parameters. The x and y were initially meant to denote the inner and outer radius of the torus, but currently they don't do anything, so any number will suffice. 

Example:
'-kd 0.7 0.7 0.7 -ks 0.7 0.2 0.2 -pl 4.0 0.0 2.0 0 1 1 -pl 0.0 4.0 2.0 1 0 1 -pl -4.0 -4.0 1.0 1 1 0 -pl 0.0 0.0 1.0 0.4 0.5 0.9 -sp 20.0 -pl -.5 0 5 .2 .2 .2 -tor 5 5'

### Cel Shading
We implemented toon shading which is used when the "-toon" parameter is added.

Example:
'-ka .25 0 .2 -kd .7 .7 .7 -ks .7 .7 .7 -sp 20 -dl .5 -.5 -1 .42 .44 .77 -pl 2 -2 -1 .1 .1 .1 -toon'
