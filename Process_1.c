#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define MAX 100

void  INThandler(int);

time_t timer = 0;
long timer1 = 0;
long timer2 = 0;

struct mesg_buffer1 {
    long mesg_type;
    char mesg_text[100];
    int number;
    int pid;
} message1;
  
  
  struct mesg_buffer2 {
    long mesg_type;
    char mesg_text[100];
    int number;
    int pid;
 } message2;
  

int msgid1=0,msgid2=0;
  
int main()
{
  long key1 = 0xfffffffe;
  long key2 = 0xfffffffd;
   
    msgid1 = msgget(key1, 0666 | IPC_CREAT);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    
    message1.mesg_type = 1;
    message1.number = 1;
    message1.pid = getpid();
    signal(SIGINT, INThandler);
    
      while(1)
      {
        timer1 = time(&timer);
        sleep(1);
        message1.number++;
        msgsnd(msgid1, &message1, sizeof(message1), 0); // msgsnd to send message
        msgrcv(msgid2, &message2, sizeof(message2), 1, 0);  // msgrcv to receive message
        timer2 = time(&timer);    
        printf("A_Timer:%ld A_PID:%d B_msqid:%d B_key:%lX B_smessage: %d \n", timer2 - timer1, message1.pid , msgid2, key2, message2.number); // display the message
      }
    
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
     
    return 0;
}

void INThandler(int sig)
{
  signal(sig, SIG_IGN);
  msgctl(msgid1, IPC_RMID, NULL);
  msgctl(msgid2, IPC_RMID, NULL);
  kill(message2.pid,SIGSEGV);
     
     exit(0);
}