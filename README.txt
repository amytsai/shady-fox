Names: Amy Tsai, Jason Ye

Platform: OSX

Location: Amy Tsai (cs184-dj) is submitting on OSX.

Extra Features

Torus: Instead of rendering a sphere, we can render a torus if we add "-tor x y" to the parameters. The x and y were initially meant to denote the inner and outer radius of the torus, but currently they don't do anything, so any number will suffice. 

Example: -kd 0.7 0.7 0.7 -ks 0.7 0.2 0.2 -pl 4.0 0.0 2.0 0 1 1 -pl 0.0 4.0 2.0 1 0 1 -pl -4.0 -4.0 1.0 1 1 0 -pl 0.0 0.0 1.0 0.4 0.5 0.9 -sp 20.0 -pl -.5 0 5 .2 .2 .2 -tor 5 5

Toon: We implemented toon shading which is used when the "-toon" parameter is added.

Example: -ka .25 0 .2 -kd .7 .7 .7 -ks .7 .7 .7 -sp 20 -dl .5 -.5 -1 .42 .44 .77 -pl 2 -2 -1 .1 .1 .1 -toon

Example of a toon torus: -kd 0.7 0.7 0.7 -ks 0.7 0.2 0.2 -pl 4.0 0.0 2.0 0 1 1 -pl 0.0 4.0 2.0 1 0 1 -pl -4.0 -4.0 1.0 1 1 0 -pl 0.0 0.0 1.0 0.4 0.5 0.9 -sp 20.0 -pl -.5 0 5 .2 .2 .2 -tor 5 5 -toon