/* This program implements the basic functional of bash-terminal
by Shaginova Elizaveta, 2020 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main() {

     char argv[16][80];
     char *a[17];
     int i = 0;
     int j = 0;
     char* filein;
     char* fileout;
     int in = 0;
     int out = 0;

     printf("Команда (для завершения нажмите Ctrl + C): ");
     while (1) {
          int ch = getchar();
          if (ch != '\n' && ch != ' ' && ch != '>' && ch != '<') {
               argv[i][j++] = ch;	
          }
          if (ch == '<' || ch == '>') {  
              if (ch == '<')
                in = i;
              else  out = i;             
              argv[i][j++] = ch;         
              argv[i][j++] = '\0';       
              i++;                       
              j = 0; 
	            continue;                     
          }                       
          if (ch == ' ') { 
               argv[i][j++] = '\0';
               i++;
               j = 0;
		           continue;
          }
          if (ch == '\n') {
                argv[i][j++] = '\0';

                for (j = 0; j <= i; j++) 
                           a[j] = argv[j];
               a[j] = NULL;
               /* id процесса */
               pid_t pid;
               /* статус выполнения команды пользователя */
               int status;
               /* создаем дочерний процесс, и если он не создался, выводим ошибку */ 
               if ((pid = fork()) < 0) {
                    perror("execvp: fork child process failed\n");
                    exit(EXIT_FAILURE);
               }
               /* иначе выполняем команду пользователя, и если она не выполняется, выводим ошибку*/
               else if (pid == 0) {
                     /* перенаправление ввода, если необходимо*/
                     if (in != 0) {
                         in++;
                         filein = argv[in];
                         int fdin = open(filein, O_RDONLY);
                         if (fdin < 0) {
                             perror("open in");
                             exit(EXIT_FAILURE);
                         }
                         if (dup2(fdin, STDIN_FILENO) < 0) {
                             perror("dup2 in");
                             exit(EXIT_FAILURE);
                         }
			                   a[--in] = NULL;
                     }
                     /* перенаправление вывода, если необходимо*/
                     if (out != 0) {
                         out++;
                         fileout = argv[out];
                         int fdout = open(fileout, O_WRONLY | O_CREAT | O_TRUNC,0666);
                         if (fdout < 0) {
                             perror("open out");
                             exit(EXIT_FAILURE);
                         }
                         if (dup2(fdout, STDOUT_FILENO) < 0) {
                             perror("dup2 out");
                             exit(EXIT_FAILURE);
                         }
			                   a[--out] = NULL;
                     }
                     if (execvp(argv[0], a) < 0) {
                           perror("execvp: exec failed\n");
                           exit(EXIT_FAILURE);
                     }
                    }
                     /* иначе ожидаем завершения работы команды пользователя */
                    else {                                 
                         while (wait(&status) != pid);      
                    }
               /* ожидаем командy пользователя */
               printf("\nКоманда: ");
               i = 0;
               j = 0;
               in = 0;
               out = 0;
          }
     }

     return EXIT_SUCCESS;
}
