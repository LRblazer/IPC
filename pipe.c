/*********************************************************************************
 *      Copyright:  (C) 2020 lingyun
 *                  All rights reserved.
 *
 *       Filename:  pipe.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年03月21日)
 *         Author:  xuxinhua <[4~[D[4~xxu>
 *      ChangeLog:  1, Release initial version on "2020年03月21日 09时48分19秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSG_STR "This message is from father: Hello, my son!"

int main(int argc, char **argv)
{
    int     pipe_fd[1];
    int     rv;
    int     pid;
    char    buf[512];
    int     wstatus;
    
    printf("before creat pipe\n");
    if (pipe(pipe_fd) < 0)
    {
        printf("Create pipe failure: %s\n", strerror(errno));
        return -1;
    }


    printf("before fork\n");
    if ((pid = fork()) < 0)
    {
        printf("Create child process failure: %s\n", strerror(errno));
        return -2;
    }
    else if (pid == 0)
    {                    
        printf("son start read message from father\n");
        close(pipe_fd[1]);
        memset(buf, 0, sizeof(buf));
        rv = read(pipe_fd[0], buf, sizeof(buf));
        if (rv < 0)
        {
            printf("Child process read from pipe failure: %s\n", strerror(errno));
            return -3;
        }
        printf("Child process read %d bytes data from pipe: \"%s\"\n", rv, buf);
        return 0;
    }                           



    
    close(pipe_fd[0]);
    if (write(pipe_fd[1], MSG_STR, strlen(MSG_STR)) < 0)
    {
        printf("Parent process write data to pipe failure: %s\n", strerror(errno));
        return -3;
    }
    

    printf("father finish write and wait son read \n");
    wait(&wstatus);
    

    return 0;
}
