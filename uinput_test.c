/*
	     w3zeng create
	function: create virual mouse 
	email: zengweitotty@gmail.com
*/
#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <string.h>
#include <memory.h>

/* Globals */
static int uinp_fd = -1;
struct uinput_user_dev uinp; // uInput device structure
struct input_event event; // Input device structure
/* Setup the uinput device */
int setup_uinput_device()
{
    // Temporary variable
    int i=0;
    // Open the input device
    uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
    if (uinp_fd < 0)
    {

        printf("Unable to open /dev/uinput/n");
        return -1;
    }
    memset(&uinp,0,sizeof(uinp)); // Intialize the uInput device to NULL
    strncpy(uinp.name, "Touch Screen", UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 4;
    uinp.id.bustype = BUS_USB;
	uinp.absmin[ABS_X] = 0;
	uinp.absmax[ABS_X] = 800;
	uinp.absfuzz[ABS_X] = 0;
	uinp.absflat[ABS_X] = 0;
	uinp.absmin[ABS_Y] = 0;
	uinp.absmax[ABS_Y] = 600;
	uinp.absfuzz[ABS_Y] = 0;
	uinp.absflat[ABS_Y] = 0;
    // Setup the uinput device
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
    ioctl(uinp_fd, UI_SET_RELBIT, REL_X);
    ioctl(uinp_fd, UI_SET_RELBIT, REL_Y);
	ioctl(uinp_fd,UI_SET_ABSBIT,ABS_X);
	ioctl(uinp_fd,UI_SET_ABSBIT,ABS_Y);
    for (i=0; i < 256; i++) {
        ioctl(uinp_fd, UI_SET_KEYBIT, i);
    }
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_FORWARD);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_BACK);

	uinp.absmin[ABS_X] = 0;
	uinp.absmax[ABS_X] = 800;
	uinp.absfuzz[ABS_X] = 0;
	uinp.absflat[ABS_X] = 0;
	uinp.absmin[ABS_Y] = 0;
	uinp.absmax[ABS_Y] = 600;
	uinp.absfuzz[ABS_Y] = 0;
	uinp.absflat[ABS_Y] = 0;
    /* Create input device into input sub-system */
    write(uinp_fd, &uinp, sizeof(uinp));
    if (ioctl(uinp_fd, UI_DEV_CREATE))
    {
        printf("Unable to create UINPUT device.");
        return -1;
    }
    return 1;
}
void send_click_events( )
{
	int i = 0;
	
    
	
    // Report BUTTON CLICK - PRESS event
	memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = 4;
    event.code = 4;
    event.value = 589825;
    write(uinp_fd, &event, sizeof(event));

    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = BTN_LEFT;
    event.value = 1;
    write(uinp_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
	
	for(i = 0; i < 25; i++){
		// Move pointer to (0,0) location
		usleep(50000);
		memset(&event, 0, sizeof(event));
		gettimeofday(&event.time, NULL);
		event.type = EV_REL;
		event.code = REL_X;
		event.value = 10;
		write(uinp_fd, &event, sizeof(event));
		event.type = EV_REL;
		event.code = REL_Y;
		event.value = 10;
		write(uinp_fd, &event, sizeof(event));
		event.type = EV_SYN;
		event.code = SYN_REPORT;
		event.value = 0;
		write(uinp_fd, &event, sizeof(event));
	}
	
    // Report BUTTON CLICK - RELEASE event
	memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = 4;
    event.code = 4;
    event.value = 589825;
    write(uinp_fd, &event, sizeof(event));
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = BTN_LEFT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));

	usleep(500000);
	// Move pointer to (0,0) location
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_REL;
    event.code = REL_X;
    event.value = -250;
    write(uinp_fd, &event, sizeof(event));
    event.type = EV_REL;
    event.code = REL_Y;
    event.value = -250;
    write(uinp_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
}
void send_a_button()
{
    // Report BUTTON CLICK - PRESS event
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = KEY_A;
    event.value = 1;
    write(uinp_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
    // Report BUTTON CLICK - RELEASE event
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, NULL);
    event.type = EV_KEY;
    event.code = KEY_A;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
}
/* This function will open the uInput device. Please make
sure that you have inserted the uinput.ko into kernel. */
int main()
{
    int i = 0;
	 
    // Return an error if device not found.
    if (setup_uinput_device() < 0)
    {
        printf("Unable to find uinput device/n");
        return -1;
    }
    //send_a_button(); // Send a "A" key
	for(i = 0; i < 10000 ; i++){ 
 		send_click_events(); // Send mouse event
		sleep(2);
	}
    /* Destroy the input device */
    ioctl(uinp_fd, UI_DEV_DESTROY);
    /* Close the UINPUT device */
    close(uinp_fd);
	
	return 0;
}
