Autoclip
========
An automatic clipboard copied text saving utility. Sometimes I need to copy text articles and other online text materials for later reading. I preffer to save web aricles in text format instead of pdf or other document formats and I dont wanna to switch to a text editor for paste everytime when I copy text. So I tried to make a automatic tool that save text continuosly when I copy text with `Ctrl+c` and save clipboard in a folder for later. I found a very nice tool named SKeylogger [https://github.com/gsingh93/simple-key-logger].
Everytime copied text is pasted separated by a 80 character long dashed line so it will be clear to read it again.
There is a custom shortcut keys combo for enable and disable automatic saving clipboard text.

Feature
-------
1. Automatic start at boot time
2. Run as daemon process in background
3. Automatic clipboard text saving at pressing `Ctrl + c`
4. Enable and disable automatic saving with shortcut keys
5. Pasted text are saperated by a dashed line of 80 character long, so its easy to differentiate again between different text content
6. It will create a new file for saving clipboard everytime when we disable and re-enable it, the file name is based on current system date and time so there is no need to worry about overriting files with same names
7. It notify us about enable and disable trigger using `notify-send` cli utility

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
