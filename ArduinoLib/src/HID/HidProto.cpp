/*
 * HidProto.cpp
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */

#include "HidProto.h"

HCIProto::HCIProto() {
	// TODO Auto-generated constructor stub

}

void HCIProto::HCI_task(void) {
    HCI_event_task();

    switch (hci_state_) {
      case HCI_INIT_STATE:
        // wait until we have looped 10 times to clear any old events
        if (hci_timeout) {
            hci_reset();
            hci_state_ = HCI_RESET_STATE;
            hci_counter_ = 1000;
        }
        break;

      case HCI_RESET_STATE:
        if (hci_command_complete) {
            DEBUG_PRINT_P( PSTR("\r\nHCI Reset complete") );
            switch (bdaddr_acquisition_mode_) {
              case BD_ADDR_FIXED:
                hci_state_ = HCI_READY_CONNECT_STATE;
                hci_counter_ = 10000;
                break;

              case BD_ADDR_INQUIRY:
                hci_inquiry();
                hci_state_ = HCI_INQUIRY_STATE;
                hci_counter_ = 10000;
                break;

              default:
                break;
            }
        }
        if (hci_timeout) {
            DEBUG_PRINT_P( PSTR("\r\nNo response to HCI Reset") );
            hci_state_ = HCI_INIT_STATE;
            hci_counter_ = 10;
        }
        break;

      case HCI_INQUIRY_STATE:
        if (hci_inquiry_result) {
            DEBUG_PRINT_P( PSTR("\r\nHCI Inquiry responded") );
            hci_inquiry_cancel();
            hci_state_ = HCI_READY_CONNECT_STATE;
            hci_counter_ = 10000;
        }
        break;

      case HCI_READY_CONNECT_STATE:
        if (hci_command_complete) {
            if (hci_inquiry_result) {
                DEBUG_PRINT_P( PSTR("\r\nHCI Inquiry complete") );
            }
            hci_connect(wiiremote_bdaddr_); // connect to Wiimote
            hci_state_ = HCI_CONNECT_OUT_STATE;
            hci_counter_ = 10000;
        }
        break;

      case HCI_CONNECT_OUT_STATE:
        if (hci_connect_complete) {
            if(hci_connect_ok) {
                DEBUG_PRINT_P( PSTR("\r\nConnected to Wiimote") );
                hci_state_ = HCI_CONNECTED_STATE;
                l2cap_state_ = L2CAP_INIT_STATE;
                wiiremote_status_ |= WIIREMOTE_STATE_CONNECTED;
            }
            else {
                hci_connect(wiiremote_bdaddr_); // try again to connect to Wiimote
                hci_counter_ = 10000;
            }
        }
        if (hci_timeout) {
            hci_connect(wiiremote_bdaddr_); // try again to connect to Wiimote
            hci_counter_ = 10000;
        }
        break;

      case HCI_CONNECTED_STATE:
        if (hci_disconn_complete) {
            DEBUG_PRINT_P( PSTR("\r\nWiimote Disconnected") );
            hci_state_ = HCI_INIT_STATE;
            hci_counter_ = 10;
            l2cap_state_ = L2CAP_DOWN_STATE;
            wiiremote_status_ &= ~WIIREMOTE_STATE_CONNECTED;
        }
        break;

      default:
        break;
    }   // switch (hci_state_)

    return;
} // HCI_task

void HCIProto::HCI_event_task(void) {
    uint8_t rcode = 0;  // return code
    uint8_t buf[MAX_BUFFER_SIZE] = {0};

    // check input on the event pipe (endpoint 1)
    /*rcode = Usb.inTransfer(BT_ADDR, ep_record_[ EVENT_PIPE ].epAddr,
                           MAX_BUFFER_SIZE, (char *) buf, USB_NAK_NOWAIT);*/

    /*
    DEBUG_PRINT_P( PSTR("\r\nHCI_event_task: rcode = 0x") );
    DEBUG_PRINT(rcode, HEX);
    */
    if (!rcode) {
        /*  buf[0] = Event Code                            */
        /*  buf[1] = Parameter Total Length                */
        /*  buf[n] = Event Parameters based on each event  */
        DEBUG_PRINT_P( PSTR("\r\nHCI event = 0x") );
        DEBUG_PRINT(buf[0], HEX);
        switch (buf[0]) {   // switch on event type
          case HCI_EVENT_COMMAND_COMPLETE:
            hci_event_flag_ |= HCI_FLAG_COMMAND_COMPLETE;
            break;

          case HCI_EVENT_INQUIRY_RESULT:
            hci_event_flag_ |= HCI_FLAG_INQUIRY_RESULT;

            /* assume that Num_Responses is 1 */
            DEBUG_PRINT_P( PSTR("\r\nFound WiiRemote BD_ADDR:\t") );
            for (uint8_t i = 0; i < 6; i++) {
                wiiremote_bdaddr_[5-i] = (uint8_t) buf[3+i];
                DEBUG_PRINT(wiiremote_bdaddr_[5-i], HEX);
            }
            break;

          case HCI_EVENT_INQUIRY_COMPLETE:
            hci_event_flag_ |= HCI_FLAG_INQUIRY_COMPLETE;
            break;

          case HCI_EVENT_COMMAND_STATUS:
            hci_event_flag_ |= HCI_FLAG_COMMAND_STATUS;

#if WIIREMOTE_DEBUG
            if (buf[2]) {   // show status on serial if not OK
                DEBUG_PRINT_P( PSTR("\r\nHCI Command Failed: ") );
                DEBUG_PRINT_P( PSTR("\r\n\t             Status = ") );
                DEBUG_PRINT(buf[2], HEX);

                DEBUG_PRINT_P( PSTR("\r\n\tCommand_OpCode(OGF) = ") );
                DEBUG_PRINT( ((buf[5] & 0xFC) >> 2), HEX);

                DEBUG_PRINT_P( PSTR("\r\n\tCommand_OpCode(OCF) = ") );
                DEBUG_PRINT( (buf[5] & 0x03), HEX);
                DEBUG_PRINT(buf[4], HEX);
            }
#endif
            break;
          case HCI_EVENT_CONNECT_COMPLETE:
            hci_event_flag_ |= HCI_FLAG_CONNECT_COMPLETE;

            if (!buf[2]) {  // check if connected OK
                // store the handle for the ACL connection
                hci_handle_ = buf[3] | ((buf[4] & 0x0F) << 8);

                hci_event_flag_ |= HCI_FLAG_CONNECT_OK;
            }
            break;

          case HCI_EVENT_NUM_COMPLETED_PKT:
			#if WIIREMOTE_DEBUG
						DEBUG_PRINT_P( PSTR("\r\nHCI Number Of Completed Packets Event: ") );
						DEBUG_PRINT_P( PSTR("\r\n\tNumber_of_Handles = 0x") );
						DEBUG_PRINT(buf[2], HEX);
						for (uint8_t i = 0; i < buf[2]; i++) {
							DEBUG_PRINT_P( PSTR("\r\n\tConnection_Handle = 0x") );
							DEBUG_PRINT((buf[3+i] | ((buf[4+i] & 0x0F) << 8)), HEX);
						}
			#endif
            break;

          case HCI_EVENT_QOS_SETUP_COMPLETE:
            break;

          case HCI_EVENT_DISCONN_COMPLETE:
            hci_event_flag_ |= HCI_FLAG_DISCONN_COMPLETE;
            DEBUG_PRINT_P( PSTR("\r\nHCI Disconnection Complete Event: ") );
            DEBUG_PRINT_P( PSTR("\r\n\t           Status = 0x") );
            DEBUG_PRINT(buf[2], HEX);
            DEBUG_PRINT_P( PSTR("\r\n\tConnection_Handle = 0x") );
            DEBUG_PRINT((buf[3] | ((buf[4] & 0x0F) << 8)), HEX);
            DEBUG_PRINT_P( PSTR("\r\n\t           Reason = 0x") );
            DEBUG_PRINT(buf[5], HEX);
            break;

          default:
            DEBUG_PRINT_P( PSTR("\r\nUnmanaged Event: 0x") );
            DEBUG_PRINT(buf[0], HEX);
            break;
        }   // switch (buf[0])
    }
    return;
} // HCI_event_task

/************************************************************/
/* HCI Commands                                             */
/************************************************************/
uint8_t HCIProto::hci_reset(void) {
    uint8_t buf[3] = {0};

    hci_event_flag_ = 0;    // clear all the flags

    buf[0] = HCI_OCF_RESET;
    buf[1] = HCI_OGF_CTRL_BBAND;
    buf[2] = 0x00; // Parameter Total Length = 0

    return HCI_Command(3, buf);
}

#if 0
uint8_t hci_read_bd_addr(void) {
    uint8_t buf[3] = {0};

    hci_event_flag_ &= ~HCI_FLAG_COMMAND_COMPLETE;

    buf[0] = HCI_OCF_READ_BD_ADDR;
    buf[1] = HCI_OGF_INFO_PARAM;
    buf[2] = 0x00;  // Parameter Total Length = 0

    return HCI_Command(3, buf);
}
#endif

uint8_t HCIProto::hci_inquiry(void) {
    uint8_t buf[8] = {0};

    hci_event_flag_ &= ~(HCI_FLAG_INQUIRY_RESULT | HCI_FLAG_INQUIRY_COMPLETE);

    buf[0] = HCI_OCF_INQUIRY;
    buf[1] = HCI_OGF_LINK_CNTRL;
    buf[2] = 0x05;  // Parameter Total Length = 5
    buf[3] = 0x33;  // LAP: Genera/Unlimited Inquiry Access Code (GIAC = 0x9E8B33)
    buf[4] = 0x8B;
    buf[5] = 0x9E;
    buf[6] = 0x0A;  // Inquiry time
    buf[7] = 0x03;  // Max 1 response

    return HCI_Command(8, buf);
}

uint8_t HCIProto::hci_inquiry_cancel(void) {
    uint8_t buf[3] = {0};

    hci_event_flag_ &= ~HCI_FLAG_COMMAND_COMPLETE;

    buf[0] = HCI_OCF_INQUIRY_CANCEL;
    buf[1] = HCI_OGF_LINK_CNTRL;
    buf[2] = 0x0;   // Parameter Total Length = 0

    return HCI_Command(3, buf);
}

uint8_t HCIProto::hci_connect(uint8_t *bdaddr) {
    uint8_t buf[16] = {0};

    hci_event_flag_ &= ~(HCI_FLAG_CONNECT_COMPLETE | HCI_FLAG_CONNECT_OK);

    buf[0] = HCI_OCF_CREATE_CONNECTION;
    buf[1] = HCI_OGF_LINK_CNTRL;
    buf[2] = 0x0D;  // Parameter Total Length = 13
    buf[3] = *(bdaddr + 5); // 6 octet bluetooth address
    buf[4] = *(bdaddr + 4);
    buf[5] = *(bdaddr + 3);
    buf[6] = *(bdaddr + 2);
    buf[7] = *(bdaddr + 1);
    buf[8] = *bdaddr;
    buf[ 9] = 0x18; // DM1 or DH1 may be used
    buf[10] = 0xCC; // DM3, DH3, DM5, DH5 may be used
    buf[11] = 0x01; // page repetition mode R1
    buf[12] = 0x00; // always 0
    buf[13] = 0x00; // clock offset
    buf[14] = 0x00; // invalid clock offset
    buf[15] = 0x00; // do not allow role switch

    return HCI_Command(16, buf);
}

/* perform HCI Command */
uint8_t HCIProto::HCI_Command(uint16_t nbytes, uint8_t *dataptr) {

	return 0;
    //hci_event_flag_ &= ~HCI_FLAG_COMMAND_COMPLETE;
    /*return Usb.ctrlReq(BT_ADDR,
                       ep_record_[ CONTROL_PIPE ].epAddr,
                       bmREQ_HCI_OUT,
                       HCI_COMMAND_REQ,
                       0x00,
                       0x00,
                       0,
                       nbytes,
                       (char *) dataptr);*/
}



