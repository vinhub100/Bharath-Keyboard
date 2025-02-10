
#include <QDebug>
#include "libusb.h"

extern void USB_Init(void);
extern void USB_Loop(void);
extern void LIBUSB_CALL USB_CB(struct libusb_transfer *transfer);
extern void USB_Exit(void);


extern uint8_t USB_Configed;
extern int res, config;  /* return codes from libusb functions */
extern libusb_device_handle* handle;  /* handle for USB device */
extern libusb_context *ctx;

//      int kernelDriverDetached     = 0;  /* Set to 1 if kernel driver detached */
//      int numBytes                 = 0;  /* Actual bytes transferred. */
extern uint8_t buffer[16];
extern int NumBytesTransferred;

extern bool dirtyFlag;
extern libusb_transfer *transfer_in;
