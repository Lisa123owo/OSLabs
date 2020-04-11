/* This program implements the basic functional of bash-terminal
by Shaginova Elizaveta, 2020 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

     char argv[16][80];
     char *a[17];
     int i = 0;
     int j = 0;

     printf("������� (��� ���������� ������� Ctrl + C): ");
     while (1) {
          int ch = getchar();
          if (ch != '\n' && ch != ' ') {
               argv[i][j++] = ch;
          }
          if (ch == ' ') {
               argv[i][j++] = '\0';
               i++;
               j = 0;
          }
          if (ch == '\n') {
               argv[i][j++] = '\0';

               for(j=0;j<=i;j++)
                    a[j] = argv[j];
               a[j] = NULL;

               /* id �������� */
               pid_t pid;
               /* ������ ���������� ������� ������������ */
               int status;
               /* ������� �������� �������, � ���� �� �� ��������, ������� ������ */
               if ((pid = fork()) < 0) {
                    perror("execvp: fork child process failed\n");
                    exit(EXIT_FAILURE);
               }
               /* ����� ��������� ������� ������������, � ���� ��� �� �����������, ������� ������*/
               else if (pid == 0) {
                         if (execvp(argv[0], a) < 0) {
                              perror("execvp: exec failed\n");
                              exit(EXIT_FAILURE);
                         }
                    }
                    /* ����� ������� ���������� ������ ������� ������������ */
                    else {                                 
                         while (wait(&status) != pid);      
                    }

               printf("\n�������: ");
               i = 0;
               j = 0;
          }
     }

     return EXIT_SUCCESS;
}