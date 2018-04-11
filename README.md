# Particle System

<img src="screenshots/points.png" width="600">

Intro
-----
Particle system exploiting OpenGL and OpenCL interoperability.  
Vertices data are generated and/or animated by OpenCL every frame. OpenGL take up the relay with shading (and emiting vertices in [billboard mode](https://raw.githubusercontent.com/indiedriver/ParticleSystem/master/screenshots/billboards.png))

Usage
-----
`./ParticleSystem [particle_count]`

### Keymap:  
**WASD**  - move around  
**SPACE** - toggle gravity  
**G**     - lock gravity at cursor position  
**F**     - toggle freecam  
**E**     - emit particles  
**Q**     - toggle between points and billboards mode  
**R**     - toggle fullscreen  
**I**     - toggle debug info HUD  
**1**     - sphere init  
**2**     - cube init  


Build
-----
```
git clone --recursive https://github.com/indiedriver/ParticleSystem
cd ParticleSystem
cmake .
```
