
#include <stdio.h>
#include <iostream>
#include <thread>

#include "usbutil.h"


uint8_t USB_Configed = 0;
int res                    = 0;/* return codes from libusb functions */
int config;
libusb_device_handle* handle = nullptr;  /* handle for USB device */
libusb_context *ctx = nullptr;

uint8_t buffer[16];
int NumBytesTransferred;

bool dirtyFlag;
libusb_transfer *transfer_in;


void LIBUSB_CALL USB_CB(struct libusb_transfer *transfer){

    libusb_submit_transfer(transfer_in);
//    qDebug() << transfer_in->status;
    if(transfer->status == LIBUSB_TRANSFER_COMPLETED){
        dirtyFlag = true;
    }

}

void USB_Init(void){
      /* Initialise libusb. */
        res = libusb_init(&ctx);
        if (res != 0)
        {
          fprintf(stderr, "Error initialising libusb.\n");
          return;
        }

//        libusb_set_option(ctx,LIBUSB_OPTION_LOG_LEVEL,LIBUSB_LOG_LEVEL_DEBUG);

        handle = libusb_open_device_with_vid_pid(ctx, 0x04d8, 0x0055);
          if (!handle)
          {
            fprintf(stderr, "Unable to open device.\n");
            return;
          }


//          res = libusb_get_configuration(handle, &config);
//            if(res!=0)
//                {
//                    libusb_close(handle);
//                    return;
//                }

//           res = libusb_set_configuration(handle, 1);
//            if(res!=0)
//                {
//                qDebug() << res << " in Set Config";
//                    libusb_close(handle);
//                    return;
//                }



          /* Claim interface #0. */
          res = libusb_claim_interface(handle, 1);
          if (res != 0)
          {
            fprintf(stderr, "Error claiming interface.\n");
            return;
          }

          transfer_in  = libusb_alloc_transfer(0);

          libusb_fill_bulk_transfer(transfer_in,handle,0x82,buffer,16,USB_CB,nullptr,0);

          libusb_submit_transfer(transfer_in);

          USB_Configed = 1;


}


void USB_Loop(void){

//    res = libusb_bulk_transfer(handle,0x82,&buffer[0],8,&NumBytesTransferred,100);
//    if(res == 0){
//        dirtyFlag = true;
//        qDebug() << "Data : " << buffer[0] << NumBytesTransferred;
//    } else {
//        qDebug() << res << NumBytesTransferred;
//    }


        res =  libusb_handle_events(ctx);
                    if (res < 0){   // negative values are errors
                        return;
                    }


}


void USB_Exit(void){

    res = libusb_release_interface(handle, 0); //release the claimed interface
        if(res!=0) {
            qDebug() << "Cannot Release Interface" << endl;
            return;
        }
        qDebug() << "Released Interface" << endl;

        libusb_close(handle); //close the device we opened
        libusb_exit(ctx); //needs to be called to end the


}


