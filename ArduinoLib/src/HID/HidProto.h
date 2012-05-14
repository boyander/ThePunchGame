/*
 * HidProto.h
 *
 *  Created on: 13/05/2012
 *      Author: boyander
 */


#define WIIREMOTE_DEBUG 0
// {{{
#if WIIREMOTE_DEBUG
void Serial_print_P(const prog_char *str) {
    char c;

    if (!str) return;
    while ((c = pgm_read_byte(str++)) != 0) {
        Serial.print(c, BYTE);
    }
}

void Serial_println_P(const prog_char *str) {
    Serial_print_P(str);
    Serial.println("");
}

#define DEBUG_PRINT(c, f)   Serial.print(c, f)
#define DEBUG_PRINTLN(c, f) Serial.println(c, f)
#define DEBUG_PRINT_P(c)    Serial_print_P(c)
#define DEBUG_PRINTLN_P(c)  Serial_println_P(c)
#else

#define DEBUG_PRINT(c, f)
#define DEBUG_PRINTLN(c, f)
#define DEBUG_PRINT_P(c)
#define DEBUG_PRINTLN_P(c)
#endif

enum eHCI {
    // {{{
    /* Bluetooth HCI states for HCI_task() */
    HCI_INIT_STATE = 0,
    HCI_RESET_STATE,
    HCI_BD_ADDR_STATE,
    HCI_INQUIRY_STATE,
    HCI_READY_CONNECT_STATE,
    HCI_CONNECT_OUT_STATE,
    HCI_CONNECTED_STATE,

    /* HCI OpCode Group Field (OGF) */
    HCI_OGF_LINK_CNTRL  = (0x01 << 2),  // OGF: Link Control Commands
    HCI_OGF_LINK_POLICY = (0x02 << 2),  // OGF: Link Policy Commands
    HCI_OGF_CTRL_BBAND  = (0x03 << 2),  // OGF: Controller & Baseband Commands
    HCI_OGF_INFO_PARAM  = (0x04 << 2),  // OGF: Informational Parameters

    /* HCI OpCode Command Field (OCF) */
    HCI_OCF_INQUIRY           = 0x001,  // OGF = 0x01
    HCI_OCF_INQUIRY_CANCEL    = 0x002,  // OGF = 0x01
    HCI_OCF_CREATE_CONNECTION = 0x005,  // OGF = 0x01
    HCI_OCF_RESET             = 0x003,  // OGF = 0x03
    HCI_OCF_READ_BD_ADDR      = 0x009,  // OGF = 0x04

    /* HCI events managed */
    HCI_EVENT_INQUIRY_COMPLETE   = 0x01,
    HCI_EVENT_INQUIRY_RESULT     = 0x02,
    HCI_EVENT_CONNECT_COMPLETE   = 0x03,
    HCI_EVENT_CONNECT_REQUEST    = 0x04,    // do nothing so far
    HCI_EVENT_DISCONN_COMPLETE   = 0x05,
    HCI_EVENT_QOS_SETUP_COMPLETE = 0x0d,    // do nothing so far
    HCI_EVENT_COMMAND_COMPLETE   = 0x0e,
    HCI_EVENT_COMMAND_STATUS     = 0x0f,
    HCI_EVENT_NUM_COMPLETED_PKT  = 0x13,    // do nothing so far

    /* HCI event flags for hci_event_flag */
    HCI_FLAG_COMMAND_COMPLETE = 0x01,
    HCI_FLAG_COMMAND_STATUS   = 0x02,
    HCI_FLAG_CONNECT_COMPLETE = 0x04,
    HCI_FLAG_CONNECT_OK       = 0x08,
    HCI_FLAG_DISCONN_COMPLETE = 0x10,
    HCI_FLAG_INQUIRY_RESULT   = 0x20,
    HCI_FLAG_INQUIRY_COMPLETE = 0x40,

    // used in control endpoint header for HCI Commands
    /*bmREQ_HCI_OUT = (USB_SETUP_HOST_TO_DEVICE|
                     USB_SETUP_TYPE_CLASS|
                     USB_SETUP_RECIPIENT_DEVICE),*/
    HCI_COMMAND_REQ = 0,
    // }}}
};

/* HCI macro */
#define hci_timeout             (hci_counter_-- == 0)

/* Macros for HCI event flag tests */
#define hci_command_complete    (hci_event_flag_ & HCI_FLAG_COMMAND_COMPLETE)
#define hci_command_status      (hci_event_flag_ & HCI_FLAG_COMMAND_STATUS)
#define hci_connect_complete    (hci_event_flag_ & HCI_FLAG_CONNECT_COMPLETE)
#define hci_connect_ok          (hci_event_flag_ & HCI_FLAG_CONNECT_OK)
#define hci_disconn_complete    (hci_event_flag_ & HCI_FLAG_DISCONN_COMPLETE)
#define hci_inquiry_result      (hci_event_flag_ & HCI_FLAG_INQUIRY_RESULT)
#define hci_inquiry_complete    (hci_event_flag_ & HCI_FLAG_INQUIRY_COMPLETE)



class HCIProto {
public:

	HCIProto();

    /* variables used by high level HCI task */
    uint8_t hci_state_;     // current state of bluetooth HCI connection
    uint16_t hci_counter_;  // counter used for bluetooth HCI loops

    /* variables filled from HCI event management */
    uint8_t hci_event_flag_;    // flag of received bluetooth events
    uint8_t hci_handle_;


    void HCI_task(void);
    void HCI_event_task(void);
    uint8_t hci_reset(void);
    uint8_t hci_inquiry(void);
    uint8_t hci_inquiry_cancel(void);
    uint8_t hci_connect(uint8_t *);
    uint8_t HCI_Command(uint16_t, uint8_t *);

    virtual ~HCIProto();
};
