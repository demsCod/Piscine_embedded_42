#ifndef INTER_USER_H
#define INTER_USER_H


#define  US_PROMPT "Enter your Login:\r\n    username: \0"
#define  PS_PROMPT "    Password: "
#define WELCOME_PROMPT "Correct Password !\r\n Welcome !!!\r\n"


typedef struct user_t
{
    char username[50];
    char password[50];
    char user_case[50];
    char psw_case[50];
} user_t;


void fill_string(char *dest, char *src)
{
   int i = 0;
   while (src[i])
   {
       dest[i] = src[i];
       i++;
   }
   dest[i] = '\0';
}


void init_interface(char *usr, char *psw, user_t x)
{
    fill_string(x.username, usr);
    fill_string(x.password, psw);
}

#endif