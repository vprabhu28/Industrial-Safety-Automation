#ifndef _GSM
#define _GSM

#define SMS_DATA_LEN_MAX 170
typedef struct {

unsigned char index[3];
unsigned char phone_num[15] ;
unsigned char dat[SMS_DATA_LEN_MAX+1];

}sms_data;

extern void intGsm( void );
extern unsigned char sendSMS(unsigned char *, unsigned char * );
extern void recvResponse(unsigned char * );
extern unsigned char getSMS(sms_data * );
extern void rec_call_check(void);
extern unsigned char deleteSms(unsigned char * );
extern void recivecontr(void);
extern void recvResponse(unsigned char * );
extern unsigned char getSMS(sms_data * );
#endif 
