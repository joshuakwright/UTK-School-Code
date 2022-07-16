#include <stdio.h>

int main() {
    int security_check = 0x1111;
    char airport[30];

    setbuf(stdout, NULL);
    setbuf(stdin, NULL);
    setbuf(stderr, NULL);

    puts("Welcome to the Knoxville Airport. Where are you headed?");
    gets(airport);

    if (security_check == 0xcacacaca) {
        puts("You are flying to Califonia!. Here is the flag ticket. Safe Trip!");
        system("/bin/cat flag.answer");
    }
}
