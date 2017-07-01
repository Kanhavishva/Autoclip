#include <stdio.h>
#include <fcntl.h>   // open
#include <stdlib.h>
#include <string.h>  // strerror
#include <errno.h>
#include <unistd.h>  // daemon, close
#include <linux/input.h>
#include <stdbool.h>
#include <time.h>/* time_t, time, ctime */
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <pwd.h>


#define KEY_RELEASE 0
#define KEY_PRESS 1

static bool is_ctrl = false;
static bool is_c = false;

static bool is_alt = false;
static bool is_shift = false;
static bool is_d = false;

static bool do_work = false;
static time_t _time;


char log_file[100]; // = "~/Documents/clipboard.log";
char xclip_cmd[50] = "xclip -o >> ";
char dashed_cmd[200] = "echo -------------------------------------------------------------------------------- >> ";
char echo_cmd[200] = "echo >> ";
char notify_cmd[100] = "notify-send -t 900 ";
char autoclip[20] = "Autoclip: ";
char cmd[500];

//char notify_msg[50];

typedef struct input_event input_event;

static void rootCheck();
static int openKeyboardDeviceFile(char *deviceFile);
static char *getKeyboardDeviceFileName();
static void notify(char * msg);

/**
 *
 */
static void notify(char * msg)
{
    memset(cmd, 0, sizeof(cmd));
    strcat(cmd, notify_cmd);
    strcat(cmd, autoclip);
    strcat(cmd, msg);
    system(cmd);
}

/**
 * Exit with return code -1 if user does not have root privileges
 */
static void rootCheck() {
    if (geteuid() != 0) {
        printf("Must run as root\n");
        exit(-1);
    }
}

/**
 * Opens the keyboard device file
 *
 * @param  deviceFile   the path to the keyboard device file
 * @return              the file descriptor on success, error code on failure
 */
static int openKeyboardDeviceFile(char *deviceFile) {
    int kbd_fd = open(deviceFile, O_RDONLY);
    if (kbd_fd == -1) {
        printf("%s", strerror(errno));
        exit(-1);
    }

    return kbd_fd;
}

/**
 * Detects and returns the name of the keyboard device file. This function uses
 * the fact that all device information is shown in /proc/bus/input/devices and
 * the keyboard device file should always have an EV of 120013
 *
 * @return the name of the keyboard device file
 */
static char *getKeyboardDeviceFileName() {
    static const char *command =
            "grep -E 'Handlers|EV' /proc/bus/input/devices |"
                    "grep -B1 120013 |"
                    "grep -Eo event[0-9]+ |"
                    "tr '\\n' '\\0'";

    FILE *pipe = popen(command, "r");
    if (pipe == NULL) {
        printf("Could not determine keyboard device file");
    }

    char result[20] = "/dev/input/";
    char temp[9];
    fgets(temp, 9, pipe);

    pclose(pipe);
    return strdup(strcat(result, temp));
}


int main(int argc, char **argv) {

    rootCheck();

    //create_autoclip_dir();

    char *deviceFile;

    deviceFile = getKeyboardDeviceFileName();

    int kbd_fd = openKeyboardDeviceFile(deviceFile);

    // Daemonize process. Don't change working directory but redirect standard
    // inputs and outputs to /dev/null
    if (daemon(1, 0) == -1) {
        printf("%s", strerror(errno));
        exit(-1);
    }

    notify("Loaded");

    input_event event;

    while (read(kbd_fd, &event, sizeof(input_event)) > 0)
    {
        if (event.type == EV_KEY)
        {
            if (event.value == KEY_PRESS)
            {
                if(((event.code == KEY_LEFTCTRL) || (event.code == KEY_RIGHTCTRL) || (event.code == KEY_C)) && do_work)
                {
                    if ( (event.code == KEY_LEFTCTRL) || (event.code == KEY_RIGHTCTRL))
                    {
                        is_ctrl = true;
                    }
                    else if ((event.code == KEY_C) && is_ctrl == true)
                    {
                        is_c = true;
                    }

                    if (is_ctrl && is_c)
                    {
                        memset(cmd, 0, sizeof(cmd));
                        strcat(cmd, echo_cmd);
                        strcat(cmd, log_file);
                        system(cmd);
                        memset(cmd, 0, sizeof(cmd));
                        strcat(cmd, dashed_cmd);
                        strcat(cmd, log_file);
                        system(cmd);
                        memset(cmd, 0, sizeof(cmd));
                        strcat(cmd, xclip_cmd);
                        strcat(cmd, log_file);
                        system(cmd);

                        is_c = false;
                        is_ctrl = false;
                    }
                }
                else
                {
                    if((event.code == KEY_LEFTSHIFT) ||
                            (event.code == KEY_RIGHTSHIFT) ||
                            (event.code == KEY_RIGHTALT) ||
                            (event.code == KEY_LEFTALT) ||
                            (event.code == KEY_D))
                    {
                        if ((event.code == KEY_LEFTSHIFT) || (event.code == KEY_RIGHTSHIFT))
                        {
                            is_shift = true;
                        }
                        else if((event.code == KEY_LEFTALT) || (event.code == KEY_RIGHTALT))
                        {
                            is_alt = true;
                        }
                        else if ((event.code == KEY_D) && is_shift == true && is_alt == true)
                        {
                            is_d = true;
                        }

                        if(is_alt && is_shift && is_d)
                        {
                            if (do_work == true)
                            {
                                do_work = false;
                                notify("Stopped");
                            }
                            else
                            {
                                memset(log_file, 0, sizeof(log_file));
                                time (&_time);
                                sprintf(log_file,"~/Documents/Autoclip/clipboard_%s.log", ctime(&_time));
                                char *p = log_file;
                                for(; *p; ++p)
                                {
                                    if(*p ==' ')
                                        *p ='_';
                                    if(*p =='\n')
                                        *p ='_';
                                }
                                do_work = true;
                                notify("Started");
                            }

                            is_d = false;
                            is_shift = false;
                            is_alt = false;
                        }
                    }
                }

            }

        }
    }


    close(kbd_fd);
    return 0;
}
