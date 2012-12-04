graphics-final-project
======================

David Lopes and Isaac Harrington's final project for CSE40166

============================================================================================
-- MEETING THE REQUIREMENTS --
1. Interactivity
-> Movement with WASD, point and click for certain things
--> Shooting could be point and click.
-> Two different cameras
--> Side scroller and an orthographic side-map that can be toggled. ASK PROF IF OKAY

2. Structured Animation
-> Does our particle system thing count as structured animation? 
--> If not, we can make up some scenery or enemy that is structured.
--> Quarks, with spring forces?

3. Two meaningful texture applications
-> Back wall could be a texture 
--> Crazy idea -- moving texture coordinates, coordinated with an invisible particle system
-> A particle system could use a texture
--> Subatomic particles could have their own spherical texture
--> We could billboard (or not billboard) quads for some cool spark effects

4. Lighting
-> Electrons are light sources
-> Collisions increase light intensity momentarily
-> Play around with the colors

5. Shaders
-> Use a shader to warp particles as they move faster and faster
-> Shadows (this might be pretty tough)
-> Stretch and warp textures? Back wall idea?

6. Sound
-> Soundtrack
-> Bops and beeps (make sure these are localized)

=============================================================================================
-- GAME DESCRIPTION, PROJECT OUTLINE -- 

Setting: the subatomic world, a dark place.
2.5 dimensional platformer.
-> Movement is in 2D, but interactions are in 3D. 
User is capable of throwing subatomic particles, such as electrons and neutrons
These particles do damage to enemies, somehow, doing different amounts of damage
CRAZY IDEA: Calculate Bezier curve to launch particles at when we throw them

THE HERO: 
- Could be a moving person
- More easily, could be a special particle, the "singularity"

THE OBJECTIVE:
- Assimilate every subatomic particle. These particles rotate around you as you gather them,
  colliding with each other.

THE ENEMIES: 
- Not really enemies, there are just atoms that you can break apart and steal particles from

THE METHOD:
- By throwing the subatomic particles that you currently have at atoms, you can split them
  apart, and their stray particles will gravitate toward you once you draw a box around them

THE PHYSICS:
- All particles collide with each other
- Electrons are a light source that don't collide, neutrons and protons do damage to enemies
  or the hero
- All particles (including electrons) can become "stray", or not attached to anything, and 
  available for the hero to pick up using a box click and drag

THE LIGHTS:
- There is almost no ambient light
- Each electron is a dim light source
- We need to calculate shadows inside shaders


-- ATOMS --
- Atoms contain particle systems for neutron/protons and electrons. 