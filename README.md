# 3D-Point-Cloud-PSX-Demo


This is my entry into the PSXDEV Codeblast '14 competition. The demo is named 3D Point Cloud PSX because it contains a 3D point cloud viewer. But this not all, here the technical features includes in the demo:
* dynamic lighting
* gouraud shading
* procedural 3D object generation
* vertex shading
* lag moving effect
* 3D point cloud viewer
* two xa music

see http://www.psxdev.net/forum/viewtopic.php?f=68&t=590&sid=7eb875e1784ddb69a8d50100e1659e3f

The demo run well both in emulators (epsxe, no$cash, ...), or with a playstation (except during the sphere part where some glitches appeared on a real playstation).

The credits text is the folllowing:

The 3D point cloud contains exacly
40k vertices! This latter was
obtained using a real 3D scanner.
All assets of demo except the XA
music are included in the psx-exe!
Yes, the point cloud is included in
the psx-exe too. Without any
specifical optimization, bigger
point cloud could be shown.

This demo was really short. Indeed,
I discovered the sdk very late (at
the end of april) then I started to
develop tools and small apps to test
the little boy machine :D Source code
of this demo is provided with this
demo. Of course, the demo does not
use any \"buggy\" implementation of
malloc[23]() that comes from Sony.

I hope learn more about the psx,
and doing funny projects like reverse
engineering, game enhancement, ...
So if you are interesting in doing
game translation, or any amazing
project involving psx please pm me.

Thanks to psxdev community, and
all psxdev members, for keeping
interesting information about
the psx, the forum, tutorials,...

You can join us in the psxdev irc
channel on the
EFNET server(irc.eversible.com/6667)
channel: #psxdev

@Stamba: merci de m'avoir transmis
la musique dans les temps :D

```
!* S.T.U-Emvivre
*************************************
* CREDITS *
* *
* Coder: !* S.T.U-Emvivre *
* GFX: !* S.T.U-Vash *
* Music 0: Stamba *
* Music 1: !* S.T.U-Vash *
*************************************
```

The point cloud viewer is the one of the most interesting part of the demo, because it show that the playstation machine has the full capacity (I recall, CPU at 33.8 Mhz with a vector coprocessor, 2 MB of RAM, 1 MB of VRAM) to display a point cloud relatively high to display something with reasonable framerate. I use the TILE_1 structure to represent a vertex of the point cloud. Thus to the GPU point of view, the draw framebuffer is fill with only one dot without any polygon filling process required because no polygons are used! 
