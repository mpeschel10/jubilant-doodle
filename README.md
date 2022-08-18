# jubilant-doodle

This script is supposed to be called whenever a simple key combination is pressed. It checks if other non-modifier keys are pressed, thereby extending the keybinding to do things like `super + up + right` to move your window to the top right of the screen. I use this script with jwm (Joe's Window Manager).

I've hardcoded in the locations and sizes for my monitor since `xdotool` can't account for the size of jwm's taskbar. Depending on your keyboard, some key combinations just won't work; on my (otherwise nice) Thinkpad T470, I can't do super + down + right. This is called key ghosting.

If this is your first time dealing with C code in a while,
```
pacman -Syu libx11             # install libraries
pkg-config --cflags --libs X11 # find compiler flags
pacman -Qlq libx11 | grep .pc  # find pkg-config files
gcc -lX11 main.c -o main       # compile
./main                         # run
```
