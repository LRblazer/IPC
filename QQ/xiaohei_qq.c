/*********************************************************************************
 *      Copyright:  (C) 2020 lingyun
 *                  All rights reserved.
 *
 *       Filename:  fifo_chat.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年03月21日)
 *         Author:  xuxinhua <[4~[D[4~xxu>
 *      ChangeLog:  1, Release initial version on "2020年03月21日 10时08分13秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <libgen.h>
#include <stdlib.h>

#define FIFO_FILE1      ".fifo_chat1"
#define FIFO_FILE2      ".fifo_chat2"



int g_stop = 0;



void sig_pipe(int signum)
{
    if (SIGPIPE == signum)
    {
        printf("get pipe broken signal and let programe exit\n");
        g_stop = 1;
    }
}



int main(int argc, char **argv)
{
    int     fdr_fifo;
    int     fdw_fifo;
    int     rv;
    fd_set  rdset;
    char    buf[1024];
    




    
    if (access(FIFO_FILE1, F_OK))
    {
            printf("FIFO file \"%s\" not exist and create it now\n", FIFO_FILE1);
            mkfifo(FIFO_FILE1, 0666);
    }
     
    if (access(FIFO_FILE2, F_OK))
    {
            printf("FIFO file \"%s\" not exist and create it now\n", FIFO_FILE2);
            mkfifo(FIFO_FILE2, 0666);
    }
    


    signal(SIGPIPE, sig_pipe);
   

        
    {                         
        

        printf("start open '%s' for write and it will blocked untill read endpoint opened...\n",
               FIFO_FILE1);
        
        if ((fdw_fifo = open(FIFO_FILE1, O_WRONLY)) < 0)
        {
            printf("Open fifo[%s] for chat write endpoint failure: %s\n", FIFO_FILE1,
                   strerror(errno));
            return -1;
        }
        
        printf("start open '%s' for read...\n", FIFO_FILE2);
        if ((fdr_fifo = open(FIFO_FILE2, O_RDONLY)) < 0)
        {
            printf("Open fifo[%s] for chat read endpoint failure: %s\n", FIFO_FILE2,
                   strerror(errno));
            return -1;
        }
    }
    




    printf("start chating with xiaobai now, please input message now: \n");
    while (!g_stop)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(fdr_fifo, &rdset);
        
        rv = select(fdr_fifo + 1, &rdset, NULL, NULL, NULL);
        if (rv <= 0)
        {
            printf("Select get timeout or error: %s\n", strerror(errno));
            continue;
        }
        
        if (FD_ISSET(fdr_fifo, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            rv = read(fdr_fifo, buf, sizeof(buf));
            if (rv < 0)
            {
                printf("read data from FIFO get errorr: %s\n", strerror(errno));
                break;
            }
            else if (0 == rv)   
            {
                printf("Another side of FIFO get closed and program will exit now\n");
                break;
            }
            printf("xiaobai: %s", buf);
        }
        
        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            memset(buf, 0, sizeof(buf));
            fgets(buf, sizeof(buf), stdin);
            write(fdw_fifo, buf, strlen(buf));
        }
    }
}
