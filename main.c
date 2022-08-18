#include <stdio.h>     // main()
#include <X11/Xlib.h>  // is_down() and main()

/*
  This script is intended for use with jwm (Joe's Window Manager).
  It extends the keybinding to make key combinations with
   multiple non-modifier keys, such as super + up + right
   to move your window to the top right of the screen.

  I've hardcoded in the locations and sizes for my monitor
   since xdotool can't account for the size of jwm's taskbar.
  Depending on your keyboard, some key combinations just won't work;
   on my (otherwise nice) Thinkpad T470, I can't do super + down + right.
  This is called key ghosting.

  If this is your first time dealing with C code in a while,
   install libraries with       `pacman -Syu libx11`
    (or appropriate package/manager for your distro)
   find compiler flags with     `pkg-config --cflags --libs X11`
   find pkg-config files with   `pacman -Qlq libx11 | grep .pc`
   compile with                 `gcc -lX11 main.c -o main`
   run with                     `./main`
*/


// is_down() returns 0 if key is not down and 1 if key is down.
// The keyString should be appropriate for XStringToKeysym.
int is_down(Display *dis, char *keyString)
{
    KeySym targetSym = XStringToKeysym(keyString);
    KeyCode targetCode = XKeysymToKeycode(dis, targetSym);

    int targetByte = targetCode / 8;
    int targetBit = targetCode % 8;
    int targetMask = 1 << targetBit;
    
    char keys_return[32] = {0};
    XQueryKeymap(dis, keys_return);

    // && 1 normalizes positive integers to 1
    return (keys_return[targetByte] & targetMask) && 1;
}

// main() checks which arrow keys are down and prints an appropriate command
//  for xdotool to move the focused window.
// You can run the output of a command by enclosing the command in backticks;
//  `./main` will evaluate whatever main prints.
int main (int argc, char* argv[])
{
    Display *dis;
    dis = XOpenDisplay (NULL);

    char *keysToCheck[] = {
        "Up", "Down", "Left", "Right"
    };

    int resultVector = 0;
    for (int i = 0; i < 4; i++)
    {
        char* keyString = keysToCheck[i];
        resultVector |= is_down(dis, keyString) << i;
    }
    
    char *windowMoveCommands[] = {
        "", // None
        "xdotool getactivewindow windowsize 1910 510 windowmove 0 0",     // Up
        "xdotool getactivewindow windowsize 1910 510 windowmove 0 510",   // Down
        "", // Up Down
        "xdotool getactivewindow windowsize 955 1020 windowmove 0 0",     // Left
        "xdotool getactivewindow windowsize 955 510 windowmove 0 0",      // Left Up
        "xdotool getactivewindow windowsize 955 510 windowmove 0 510",    // Left Down
        "", // Left Down Up
        "xdotool getactivewindow windowsize 955 1020 windowmove 955 0",   // Right
        "xdotool getactivewindow windowsize 955 510 windowmove 955 0",    // Right Up
        "xdotool getactivewindow windowsize 955 510 windowmove 955 510",  // Right Down
        "", // Right Down Up
        "", // Right Left
        "", // Right Left Up
        "", // Right left Down
        "", // Right Left Up Down
    };
    
    printf("%s\n", windowMoveCommands[resultVector]);
    return 0;
}

