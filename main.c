#include <stdio.h>


//config git
/*
  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"

  update mf C/c++ vscode extension
*/
int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    return 0;
}