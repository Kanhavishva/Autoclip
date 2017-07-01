Autoclip
========
An automatic clipboard copied text saving utility. Sometimes I need to copy text articles and other online text materials for later reading. I prefer to save web articles in text format instead of pdf or other document formats and I don't want to switch between web browser and text editor for paste everytime when I copy something. So I tried to make a automatic tool that save text continuously when I copy text with `Ctrl+c` and save clipboard in a folder for later. I found a very nice tool names Keylogger [https://github.com/gsingh93/simple-key-logger], this is a little keylogger program for linux.
In `Autoclip`, there is a custom shortcut keys for enable and disable automatic saving clipboard text.

Feature
-------
1. Automatic start at boot time
2. Run as daemon process in background
3. Automatic clipboard text saving at pressing `Ctrl + c`
4. Enable and disable automatic saving with shortcut keys
5. Pasted text are separated by a dashed line of 80 character long, so its easy to differentiate again between different text content
6. It will create a new file for saving clipboard everytime when we disable and re-enable it, the file name is based on current system date and time so there is no need to worry about overwriting files with same names
7. It notify us about enable and disable status using `notify-send` cli utility

Shortcut
--------
`Alt + Shift + d` for enable and disable automatic saving clipboard text.

Usage
-----
0. Create `Autoclip` directory in `~/Documents/`
1. Just run `sudo ./autoclip`, this will start a autoclip process daemon and enable automatic saving using `Alt + Shift + d`
2. To disable automatic saving again use `Alt + Shift + d`
3. To start at boots up in Ubuntu, follow the guide as described on SKeylogger [https://github.com/gsingh93/simple-key-logger]:
    * Edit `/etc/rc.local` and add `/path/to/autoclip` above the line with `exit 0`. Replace `/path/to/autoclip` with the full path to the keylogger binary.

    * Allow sudo access without a password (Note: this may be a security threat. Do at your own risk). To do this, add the following line to `/etc/sudoers` making sure to replace the path with the path you used above and the username with your username:
    ```
    username ALL = NOPASSWD: /path/to/autoclip
    ```
    * Reboot ubuntu. Open a terminal and type `pgrep autoclip`. You should find one `autoclip` process running.

Source
------
Functions `rootCheck`, `openKeyboardDeviceFile` and `getKeyboardDeviceFileName` are used from SKeylogger [https://github.com/gsingh93/simple-key-logger].
