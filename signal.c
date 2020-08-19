/*********************************************************************************
 *      Copyright:  (C) 2020 lingyun
 *                  All rights reserved.
 *
 *       Filename:  signal.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年02月23日)
 *         Author:  xuxinhua <[4~[D[4~xxu>
 *      ChangeLog:  1, Release initial version on "2020年02月23日 17时50分47秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


int wc_door = 0;
int home_door = 0;


void sig_child(int sig_num)
{
    if( SIGUSR1 == sig_num ) 
    {
        wc_door = 1;   
    }
}



void sig_parent(int sig_num)
{
    if( SIGUSR2 == sig_num ) 
    {
        home_door = 1;   
    }
}



int main(int argc, char *argv[])
{    
    int             pid; 
    int             wstatus;

    signal(SIGUSR1, sig_child);   
    signal(SIGUSR2, sig_parent);
    
    if( (pid=fork()) < 0 )   
    {       
        printf("Create child process failure: %s\n", strerror(errno));     
        return -2;   
    }    
    else if(pid == 0)   
    {       
        printf("xiaohei is listening to music\n");
        sleep(5);
        printf("xiaohei finish listen music and tell father door is open\n");
        kill(getppid(), SIGUSR2);
        sleep(3);
        while( !wc_door )     
        {            
            printf("xiaohei ask father to la baba quickly\n");
            sleep(1);   
        }       
        printf("xiaohei finish la baba\n");        
        return 0;  
    }


    
    printf("father start knock the door\n");    
    while( !home_door )
    {      
        sleep(1);
    }
   
    printf("father start running to WC and la xiang\n"); 
    sleep(10);
    printf("father tell xiaohei can come in wc\n");
    kill(pid, SIGUSR1);
    printf("father wait me to go to shopping\n");
    wait(&wstatus);    
    printf("story over and we go to shopping\n");



    return 0;
}
