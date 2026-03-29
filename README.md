# NCURSES 3D engine

![rotation](screenshots/rotation.GIF)

# How to compile?

You can compile this by first installing ncurses lib:

On Debian/Ubuntu:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```


Then compile it, using, for example, gcc compiler:
```bash
gcc engine.c -o game -lncurses -lm
```

After compiling, you can launch binary file:
```bash
./game
```

# How to move?
Use W S buttons to move forward/backward respectively, A D to strafe, and the J L buttons to look around.

```text
*********************IMPORTANT!!!************************
* The game only works with the English keyboard layout. *
*********************************************************
```

# Non‑Euclidean illusion

![non-euclidean](screenshots/non-euclidean.GIF)

This project doesn’t just fake 3D from a 2D map — it also implements a non‑Euclidean trick.  
Go inside the building and try to reach the end: you never will. The interior is infinite.  
But step outside, and you’ll see the same building is actually tiny.  
The effect is achieved by seamless world switching and a hidden teleport trigger that resets your progress without you noticing.
