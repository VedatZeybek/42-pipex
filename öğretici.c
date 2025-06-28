#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void	fork_example()
{
	printf("\n======== FORK =========\n");
	pid_t pid = fork();

	if (pid == 0)
	{
		printf("Ben Child process %d\n", getpid());
		printf("Baba process id: %d\n", getppid());
	}
	else if (pid > 0)
	{
		printf("Ben parent process: %d\n", getpid());
		printf("Child pid: %d\n", pid);
		wait(NULL);
	}
	else
		printf("Fork olmadi.");
	printf("Bu satiri iki kez göreceksin çünkü child process actin\n");
}

void	waitpid_example()
{
	printf("\n========= WAIDPID =========\n");

	pid_t pid = fork();
	if (pid == 0)
	{
		printf("Child: HADİ 3 SN UYUYALIM.\n");
		sleep(3);
		printf("Child uyandi\n");
	}
	else if(pid > 0)
	{
		int status;
		printf("Parent: Child bekledim.");
		pid_t waited_pid = waitpid(pid, &status, 0);
		printf("Parent: Child pid %d  bitti\n Status: %d \n", pid, status);
	}

//		waitpid() olmadan:
//		 Parent: Hızlıca kendi işini yapar, çıkar
// 		Child: Hala çalışıyor... (yetim kalır)

// 		waitpid() ile:
// 		Parent: "Dur bakalım, child'ım bitsin"
// 		Child: İşini yapıyor...
// 		Child: Bitti!
// 		Parent: "Tamam şimdi ben devam edebilirim"
}

void	acces_example()
{
	printf("\n===== ACCESS ======\n");

    if (access("test.txt", F_OK) == 0)
        printf("test.txt dosyasi mevcut\n");
	else 
        printf("test.txt dosyasi yok\n");

    if (access("/bin/ls", X_OK) == 0)
        printf("/bin/ls çaliştirilabilir\n");
    else
        printf("/bin/ls çalistirilamaz\n");

    // F_OK: dosya var mı?
    // R_OK: okuma izni var mı?
    // W_OK: yazma izni var mı?
    // X_OK: çalıştırma izni var mı?}
}

void	dup_example()
{
	printf("\n===== DUP ===== \n");

	int	stdout = dup(1);
	printf("stdout: %d\n", stdout);
	write(stdout, "selam\n", 6);

	int fd = open("input.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);

	dup2(fd, 1);
	printf("Bu yazi dosyaya gider.\n");

	dup2(stdout, 1);
	close(stdout);
	close(fd);

	printf("Bu yazi terminale gidecek\n");
}

//PROCESSLER ARASI İLETİŞİM: PIPE
void	pipe_example()
{
	printf("\n ==== PIPE ==== \n");

	int		pipefd[2];
	char 	buffer[100];

	pipe(pipefd);

	pid_t pid = fork();

	if (pid == 0)
	{
		close(pipefd[1]);
		int bytes_read = read(pipefd[0], buffer, sizeof(buffer)); //parent yazmazsa veri gelmezse çocuk sonsuza dek uyur cpu
		printf("Child okudu: %s\n", buffer);
		close(pipefd[0]);
	}
	else if (pid > 0)
	{
		close(pipefd[0]);

		char message[] = "Merhaba Child. Im your father...\n";
		write(pipefd[1], message, sizeof(message));
		close(pipefd[1]);
		wait(NULL); //wait atarsan parent erken ölmez.
	}
}
void unlink_example() 
{
	printf("\n=== UNLINK ===\n");

	int fd = open("temp_file.txt", O_CREAT | O_WRONLY, 0644);
	write(fd, "Test verisi", 11);
	close(fd);

	printf("temp_file.txt oluşturuldu\n");

	if (unlink("temp_file.txt") == 0)
		printf("temp_file.txt silindi\n");
	else 
        printf("Dosya silinemedi!\n");
}
int main()
{
	fork_example();
	waitpid_example();
	acces_example();
	dup_example();
	pipe_example();
	unlink_example();
}