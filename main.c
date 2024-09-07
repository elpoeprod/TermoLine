/*
        TermoLine - by elpoep
        @ 2024
*/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <dirent.h>

char *strcasestr(const char *haystack, const char *needle);

char *mycmd;
int works=1;
int dummy=0;
//char *buffer=malloc(2048);
char *name;
char *curdrive;
char *curdir;
int check_commands();

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif


int get_ready() {
    printf(ANSI_COLOR_GREEN "%s@%s~ %s" ANSI_COLOR_RESET"> ",name,curdrive,curdir);
    char buf[256];//=malloc(256);
    strncpy(mycmd,gets(&buf),256);
    //free(buf);
    return 1;
}

int main(int argc, char *argv[]) {
    system("@echo off");
    name=(char *)malloc(sizeof(char *)*256);
    curdrive=(char *)malloc(16);
    strcpy(curdrive,"C:\\");
    strcpy(name,getenv("USERNAME"));
    mycmd=(char *)malloc(1024);
    curdir=(char *)malloc(512);
    strcpy(curdir,"");
    puts("TermoLine v0.1\nby elpoep 2024");
    while(works) {
        get_ready();
        check_commands();
    }

    free(name);
    return 0;
}

int check_commands() {
    char *h=(char *)malloc(sizeof(char *)*256);

    if(strcasestr(strncpy(h,mycmd,4),"exit")) {
        works=0;
    }
    else
    if(strcasestr(strncpy(h,mycmd,4),"help")) {
        if(strlen(mycmd)>4) {
            if(strcasestr(strcpy(h,mycmd+4),"help")) puts("HELP [command] - shows every available command or shows help about argumented command.");
            if(strcasestr(strcpy(h,mycmd+4),"dir")) puts("DIR [directory] - shows every file and directory in current dir (if no arguments) and in argumented directory if it exists.\n -A - alternative showing of dir. (UNSTABLE)");
            if(strcasestr(strcpy(h,mycmd+4),"cd")) puts("CD [directory] - changes current directory or clears it if no arguments written.");
            if(strcasestr(strcpy(h,mycmd+4),"exit")) puts("EXIT - exits program.");
            if(strcasestr(strcpy(h,mycmd+4),"echo")) puts("ECHO [text] - outputs text on the screen.");
            if(strcasestr(strcpy(h,mycmd+4),".")) puts(". [program]- executes argumented file if it exists.");
            if(strcasestr(strcpy(h,mycmd+4),"start")) puts("START [program] - executes argumented file if it exists.");
        } else
        puts("HELP:\necho - prints a text on the screen\nhelp - shows help\nexit - exit program\ndir - show directory's files\ncd - change current dir\n. - execute file\nstart - execute file");
    }
    else
    if(strcasestr(strncpy(h,mycmd,4),"echo")) {
            if(strlen(mycmd)>5) {
                char *echo=(char *)malloc(sizeof(char *)*256);
                printf("%s\n",strcpy(echo,mycmd+5));
                free(echo);} else printf(" \n");
    }
    else
    if(strcasestr(strncpy(h,mycmd,3),"dir")) {
        if(strlen(mycmd)>6) {
            if(strcasestr(strncpy(h,strchr(mycmd,' '),256),"-a")) {
                char *dirname=(char *)malloc(512);
                if(strcasestr(strcpy(h,mycmd+7),curdrive)==0) {
                    strcpy(dirname,curdrive);
                    strcat(dirname,mycmd+7);
                } else strcpy(dirname,mycmd+7);

            DIR *dir;
            struct dirent *ddir;
            dir=opendir(dirname);
            if(dir) {
                while((ddir=readdir(dir))!=NULL) {
                    float filesize=-1;
                    WIN32_FILE_ATTRIBUTE_DATA attr;
                    char *fname=malloc(1024);
                        strcpy(fname,dirname);
                        strcat(fname,ddir->d_name);
                    int len=MultiByteToWideChar(CP_UTF8,0,fname,-1,NULL,0);
                    wchar_t *wname=(wchar_t *)malloc(len*2);
                    MultiByteToWideChar(CP_UTF8,0,fname,-1,wname,len);
                    HRESULT res=GetFileAttributesExW(wname,GetFileExInfoStandard,&attr);
                    if(res) filesize=(float)((unsigned __int64)attr.nFileSizeLow|((unsigned __int64)attr.nFileSizeHigh<<32));
                    char *repeated=malloc(256);
                    strcpy(repeated," ");
                    for(int i=0;i<32-strlen(ddir->d_name);i++) strcat(repeated," ");

                    if(filesize>1023)
                        printf("%s %s %.2f%s\n",ddir->d_name,repeated,filesize/1024,"KB");
                    else if(filesize>0&&filesize<1024)
                        printf("%s %s %.0f%s\n",ddir->d_name,repeated,filesize,"B");
                    else if(ddir->d_type==DT_DIR)
                        printf("%s%s %s %s\n",ddir->d_name,"\\",repeated,"DIR");
                    else printf("%s %s\n",ddir->d_name,repeated);

                    free(wname);
                    free(repeated);
                    free(fname);
                }
            closedir(dir);
        free(dirname);
            } else {printf("No directory exists: A%s\n",strcpy(h,mycmd+4));}
            } else {
                char *dirname=(char *)malloc(512);
                if(!strcasestr(strcpy(h,mycmd+4),curdrive)) {
                    strcpy(dirname,curdrive);
                    strcat(dirname,mycmd+4);
                } else strcpy(dirname,mycmd+4);

                DIR *dir;
                struct dirent *ddir;
                dir=opendir(dirname);
                if(dir) {
                    while((ddir=readdir(dir))!=NULL) {
                        printf("%s%s        ",ddir->d_name,(ddir->d_type==DT_REG) ? "" : "\\");
                    }
                    closedir(dir);
                    printf("\n");
                } else {printf("No directory exists: AA%s\n",strcpy(h,mycmd+4));}
            }
        } else {
            if(strcasestr(strcpy(h,mycmd+4),"-a")) {
                DIR *dir;
                struct dirent *ddir;
                char *mydirr=(char *)malloc(512);
                        strcpy(mydirr,curdrive);
                        strcat(mydirr,curdir);

                dir=opendir(mydirr);
                if(dir) {
                    while((ddir=readdir(dir))!=NULL) {
                        float filesize=-1;
                        WIN32_FILE_ATTRIBUTE_DATA attr;
                        char *fname=malloc(256);
                            strcpy(fname,curdrive);
                            strcat(fname,ddir->d_name);
                        int len=MultiByteToWideChar(CP_UTF8,0,fname,-1,NULL,0);
                        wchar_t *wname=(wchar_t *)malloc(len*2);
                        MultiByteToWideChar(CP_UTF8,0,fname,-1,wname,len);
                        HRESULT res=GetFileAttributesExW(wname,GetFileExInfoStandard,&attr);
                        if(res) filesize=(float)((unsigned __int64)attr.nFileSizeLow|((unsigned __int64)attr.nFileSizeHigh<<32));
                        char *repeated=malloc(256);
                        strcpy(repeated," ");
                        for(int i=0;i<32-strlen(ddir->d_name);i++) strcat(repeated," ");

                        if(filesize>1023)
                            printf("%s %s %.2f%s\n",ddir->d_name,repeated,filesize/1024,"KB");
                        else if(filesize>0&&filesize<1024)
                            printf("%s %s %.0f%s\n",ddir->d_name,repeated,filesize,"B");
                        else if(ddir->d_type==DT_DIR)
                            printf("%s%s %s %s\n",ddir->d_name,"\\",repeated,"DIR");
                        else printf("%s %s\n",ddir->d_name,repeated);

                        free(wname);
                        free(repeated);
                        free(fname);
                    }
                closedir(dir);
            }
        free(mydirr);
        } else {
            char *dirname=(char *)malloc(512);
                strcpy(dirname,curdrive);
                strcat(dirname,curdir);

                DIR *dir;
                struct dirent *ddir;
                dir=opendir(dirname);
                if(dir) {
                    while((ddir=readdir(dir))!=NULL) {
                        printf("%s%s        ",ddir->d_name,(ddir->d_type==DT_REG) ? "" : "\\");
                    }
                    closedir(dir);
                    printf("\n");
                } else {printf("No directory exists: %s\n",strncpy(h,mycmd+7,256-7));}
    }
    }
    }
    else if(strcasestr(strncpy(h,mycmd,strlen("cls")),"cls")) {
        system("cls");
    }
    else if(strcasestr(strncpy(h,mycmd,strlen("clear")),"clear")) {
        system("cls");
    }
    else if(strcasestr(strncpy(h,mycmd,2),"cd")) {
        if(strlen(mycmd)>2) {
        strcat(curdir,strcpy(h,mycmd+3));
        if(strcasestr(strcpy(h,mycmd+3),"\\")==0) strcat(curdir,"\\");
        } else strcpy(curdir,"");
    }
    else if(strcasestr(strncpy(h,mycmd,1),".")&&strlen(mycmd)>2) {
        char *myname=(char *)malloc(256);
            strcpy(myname,"start ");
            if(strcasestr(mycmd,curdrive)==0) {strcat(myname,curdrive);strcat(myname,curdir);}
            strcat(myname,strcpy(h,mycmd+2));

        system(myname);
        free(myname);
    }
    else if(strcasestr(strncpy(h,mycmd,5),"start")&&strlen(mycmd)>6) {
        char *myname=(char *)malloc(256);
            strcpy(myname,"start ");
            if(strcasestr(mycmd,curdrive)==0) {strcat(myname,curdrive);strcat(myname,curdir);}
            strcat(myname,strcpy(h,mycmd+6));

        system(myname);
        free(myname);
    }
    else if(strcasestr(strncpy(h,mycmd+1,1),":")&&strlen(mycmd)==2) {
        //strcpy(curdrive,"");
        strcpy(curdrive,strncpy(h,mycmd,2));
        strcat(curdrive,"\\");
    }

    else printf("Invalid command: %s\n",mycmd);
    free(h);
    return 1;
}
