/*
 * EXECVE SİSTEM ÇAĞRISI DETAYLI ÖĞRENİM ALIŞTIRMASİ
 * ================================================
 * 
 * Bu alıştırma execve() sistem çağrısının tüm detaylarını öğretir:
 * - Temel kullanım
 * - Parametre yapısı
 * - Çevre değişkenleri
 * - Hata yönetimi
 * - Güvenlik konuları
 * - İleri seviye özellikler
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

// environ değişkeni için global tanım
extern char **environ;

// Renkli çıktı için ANSI kodları
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

void print_header(const char* title) {
    printf("\n" CYAN "=== %s ===" RESET "\n", title);
}

void print_step(int step, const char* description) {
    printf(YELLOW "\nAdım %d: %s" RESET "\n", step, description);
}

// BÖLÜM 1: Temel execve kullanımı
void exercise_1_basic_execve() {
    print_header("BÖLÜM 1: Temel execve Kullanımı");
    
    printf("execve() prototipi:\n");
    printf("int execve(const char *pathname, char *const argv[], char *const envp[]);\n\n");
    
    print_step(1, "Basit bir program çalıştırma");
    
    pid_t pid = fork();
    if (pid == 0) {
        // Çocuk süreç
        char *argv[] = {"echo", "Merhaba execve!", NULL};
        char *envp[] = {NULL};  // Boş çevre
        
        printf("Çocuk süreç: execve ile echo çalıştırılıyor...\n");
        
        if (execve("/bin/echo", argv, envp) == -1) {
            perror("execve failed");
            exit(1);
        }
        // Bu satır asla çalışmaz çünkü execve başarılı olursa süreç imajı değişir
        printf("Bu satır asla yazdırılmaz!\n");
    } else if (pid > 0) {
        // Ana süreç
        int status;
        wait(&status);
        printf("Ana süreç: Çocuk süreç tamamlandı (çıkış kodu: %d)\n", WEXITSTATUS(status));
    } else {
        perror("fork failed");
    }
}

// BÖLÜM 2: Parametre dizisi (argv) detayları
void exercise_2_argv_details() {
    print_header("BÖLÜM 2: Parametre Dizisi (argv) Detayları");
    
    print_step(1, "Çoklu parametrelerle program çalıştırma");
    
    pid_t pid = fork();
    if (pid == 0) {
        // ls -la /tmp komutunu çalıştır
        char *argv[] = {
            "ls",           // argv[0] - program adı
            "-la",          // argv[1] - ilk parametre
            "/tmp",         // argv[2] - ikinci parametre
            NULL            // argv dizisi NULL ile bitmeli
        };
        char *envp[] = {NULL};
        
        printf("Çalıştırılan komut: ls -la /tmp\n");
        printf("argv[0] = %s\n", argv[0]);
        printf("argv[1] = %s\n", argv[1]);
        printf("argv[2] = %s\n", argv[2]);
        printf("argv[3] = %s (NULL)\n", argv[3] ? argv[3] : "NULL");
        
        execve("/bin/ls", argv, envp);
        perror("execve failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
    
    print_step(2, "argv[0]'ın önemini gösterme");
    
    pid = fork();
    if (pid == 0) {
        // argv[0]'ı farklı bir isim yaparak programı çalıştır
        char *argv[] = {
            "benim_echo_programim",  // Gerçek program adından farklı
            "argv[0] farklı olabilir!",
            NULL
        };
        char *envp[] = {NULL};
        
        printf("\nargv[0]'ı 'benim_echo_programim' olarak ayarlayıp echo çalıştırıyoruz:\n");
        execve("/bin/echo", argv, envp);
        perror("execve failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}

// BÖLÜM 3: Çevre değişkenleri (envp)
void exercise_3_environment() {
    print_header("BÖLÜM 3: Çevre Değişkenleri (envp)");
    
    print_step(1, "Mevcut çevre değişkenlerini kullanma");
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"env", NULL};
        
        printf("Mevcut tüm çevre değişkenlerini göster:\n");
        execve("/usr/bin/env", argv, environ);
        perror("execve failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
    
    print_step(2, "Özel çevre değişkenleri oluşturma");
    
    pid = fork();
    if (pid == 0) {
        char *argv[] = {"env", NULL};
        char *envp[] = {
            "OZEL_DEGISKEN=Bu özel bir değişken",
            "KULLANICI=execve_ogrencisi",
            "SEVIYE=ileri",
            "PATH=/bin:/usr/bin",  // PATH önemli!
            NULL
        };
        
        printf("\nSadece belirlediğimiz çevre değişkenleriyle:\n");
        execve("/usr/bin/env", argv, envp);
        perror("execve failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}

// BÖLÜM 4: Hata durumları ve errno
void exercise_4_error_handling() {
    print_header("BÖLÜM 4: Hata Durumları ve errno");
    
    print_step(1, "Var olmayan dosya hatası (ENOENT)");
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"varolmayan_program", NULL};
        char *envp[] = {NULL};
        
        if (execve("/varolmayan/yol/program", argv, envp) == -1)
		{
            printf("errno: %d\n", errno);
            printf("Hata: %s\n", strerror(errno));
            printf("ENOENT değeri: %d\n", ENOENT);
        }
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
    
    print_step(2, "İzin hatası (EACCES)");
    
    // Çalıştırma izni olmayan dosya oluştur
    int fd = open("/tmp/izinsiz_dosya", O_CREAT | O_WRONLY, 0644);  // Çalıştırma izni yok
    if (fd != -1) {
        write(fd, "#!/bin/bash\necho 'test'\n", 24);
        close(fd);
        
        pid = fork();
        if (pid == 0) {
            char *argv[] = {"izinsiz_dosya", NULL};
            char *envp[] = {NULL};
            
            if (execve("/tmp/izinsiz_dosya", argv, envp) == -1) {
                printf("errno: %d\n", errno);
                printf("Hata: %s\n", strerror(errno));
                printf("EACCES değeri: %d\n", EACCES);
            }
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        }
        
        unlink("/tmp/izinsiz_dosya");  // Temizle
    }
}

// BÖLÜM 5: execve vs diğer exec fonksiyonları
void exercise_5_exec_family() {
    print_header("BÖLÜM 5: execve vs Diğer exec Fonksiyonları");
    
    printf("exec ailesinin karşılaştırması:\n\n");
    printf("execve(path, argv, envp)     - En temel hali, tam kontrol\n");
    printf("execv(path, argv)            - Mevcut çevreyi kullanır\n");
    printf("execvp(file, argv)           - PATH'te arar, mevcut çevreyi kullanır\n");
    printf("execl(path, arg0, arg1, ...) - Değişken argüman listesi\n");
    printf("execle(path, arg0, ..., envp)- Değişken argüman + çevre\n");
    printf("execlp(file, arg0, arg1, ...)- PATH'te arar\n\n");
    
    print_step(1, "execve ile tam kontrol");
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"date", "+%Y-%m-%d %H:%M:%S", NULL};
        char *envp[] = {"TZ=UTC", "LC_ALL=C", NULL};
        
        printf("execve ile UTC zamanı:\n");
        execve("/bin/date", argv, envp);
        perror("execve failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}

// BÖLÜM 6: Güvenlik konuları
void exercise_6_security() {
    print_header("BÖLÜM 6: Güvenlik Konuları");
    
    print_step(1, "PATH güvenliği");
    
    printf("GÜVENLİK UYARISI: execve() tam yol kullanır, PATH aramaz.\n");
    printf("Bu, PATH injection saldırılarına karşı koruma sağlar.\n\n");
    
    printf("Güvenli: execve(\"/bin/ls\", argv, envp)\n");
    printf("Tehlikeli olabilir: execvp(\"ls\", argv)  // PATH'te arar\n\n");
    
    print_step(2, "Çevre değişkeni temizleme");
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"env", NULL};
        char *safe_envp[] = {
            "PATH=/bin:/usr/bin",
            "HOME=/tmp",
            "USER=safe_user",
            NULL
        };
        
        printf("Güvenli, temizlenmiş çevre:\n");
        execve("/usr/bin/env", argv, safe_envp);
        perror("execve failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}

// BÖLÜM 7: İleri seviye kullanım
void exercise_7_advanced() {
    print_header("BÖLÜM 7: İleri Seviye Kullanım");
    
    print_step(1, "Shebang (#!) desteği");
    
    // Geçici script oluştur
    int fd = open("/tmp/test_script.sh", O_CREAT | O_WRONLY, 0755);
    if (fd != -1) {
        write(fd, "#!/bin/bash\necho 'Script execve ile çalıştırıldı: $0 $*'\n", 57);
        close(fd);
        
        pid_t pid = fork();
        if (pid == 0) {
            char *argv[] = {"test_script.sh", "param1", "param2", NULL};
            char *envp[] = {NULL};
            
            printf("Shebang ile script çalıştırma:\n");
            execve("/tmp/test_script.sh", argv, envp);
            perror("execve failed");
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        }
        
        unlink("/tmp/test_script.sh");
    }
    
    print_step(2, "execve performans analizi");
    
    printf("\nexecve performans notları:\n");
    printf("- execve çağrısı başarılı olursa asla geri dönmez\n");
    printf("- Süreç ID'si değişmez, sadece süreç imajı değişir\n");
    printf("- Açık dosya tanımlayıcıları (FD_CLOEXEC hariç) korunur\n");
    printf("- Signal handler'lar varsayılana döner\n");
    printf("- Memory mapping'ler temizlenir\n");
}

// BÖLÜM 8: Pratik örnekler
void exercise_8_practical() {
    print_header("BÖLÜM 8: Pratik Örnekler");
    
    print_step(1, "Mini shell implementasyonu");
    
    char command[256];
    printf("\nMini shell (çıkmak için 'exit' yazın):\n");
    
    while (1) {
        printf("mini-shell> ");
        fflush(stdout);
        
        if (!fgets(command, sizeof(command), stdin)) break;
        
        // Newline karakterini kaldır
        command[strcspn(command, "\n")] = 0;
        
        if (strcmp(command, "exit") == 0) break;
        if (strlen(command) == 0) continue;
        
        pid_t pid = fork();
        if (pid == 0) {
            // Basit parse - gerçek shell çok daha karmaşık
            char *argv[64];
            int argc = 0;
            char *token = strtok(command, " ");
            
            while (token && argc < 63) {
                argv[argc++] = token;
                token = strtok(NULL, " ");
            }
            argv[argc] = NULL;
            
            if (argc > 0) {
                // Tam yol oluştur (basit PATH arama)
                char full_path[512];
                snprintf(full_path, sizeof(full_path), "/bin/%s", argv[0]);
                
                execve(full_path, argv, environ);
                
                // /usr/bin de dene
                snprintf(full_path, sizeof(full_path), "/usr/bin/%s", argv[0]);
                execve(full_path, argv, environ);
                
                printf("Komut bulunamadı: %s\n", argv[0]);
            }
            exit(1);
        } else if (pid > 0) {
            wait(NULL);
        }
    }
}

// BÖLÜM 9: Debugging ve izleme
void exercise_9_debugging() {
    print_header("BÖLÜM 9: Debugging ve İzleme");
    
    print_step(1, "execve çağrısını izleme");
    
    printf("execve debugging teknikleri:\n\n");
    printf("1. strace kullanımı:\n");
    printf("   strace -e execve ./program\n\n");
    printf("2. ltrace kullanımı:\n");
    printf("   ltrace -e execve ./program\n\n");
    printf("3. Manual debug output:\n");
    
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"whoami", NULL};
        char *envp[] = {NULL};
        
        printf("DEBUG: execve çağrısı öncesi\n");
        printf("DEBUG: Program: /usr/bin/whoami\n");
        printf("DEBUG: argv[0]: %s\n", argv[0]);
        printf("DEBUG: PID: %d\n", getpid());
        
        if (execve("/usr/bin/whoami", argv, envp) == -1) {
            printf("DEBUG: execve başarısız: %s\n", strerror(errno));
        }
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
        printf("DEBUG: Çocuk süreç tamamlandı\n");
    }
}

// Ana menü fonksiyonu
void show_menu() {
    printf("\n" GREEN "EXECVE ÖĞRENİM PROGRAMI" RESET "\n");
    printf("=====================\n");
    printf("1. Temel execve kullanımı\n");
    printf("2. Parametre dizisi (argv) detayları\n");
    printf("3. Çevre değişkenleri (envp)\n");
    printf("4. Hata durumları ve errno\n");
    printf("5. execve vs diğer exec fonksiyonları\n");
    printf("6. Güvenlik konuları\n");
    printf("7. İleri seviye kullanım\n");
    printf("8. Pratik örnekler (Mini shell)\n");
    printf("9. Debugging ve izleme\n");
    printf("0. Çıkış\n");
    printf("\nSeçiminiz: ");
}

int main() {
    printf(BLUE "execve() Sistem Çağrısı Detaylı Öğrenim Alıştırması" RESET "\n");
    printf("Bu program execve'nin tüm detaylarını öğretecek.\n");
    
    int choice;
    while (1) {
        show_menu();
        scanf("%d", &choice);
        getchar(); // newline'ı temizle
        
        switch (choice) {
            case 1: exercise_1_basic_execve(); break;
            case 2: exercise_2_argv_details(); break;
            case 3: exercise_3_environment(); break;
            case 4: exercise_4_error_handling(); break;
            case 5: exercise_5_exec_family(); break;
            case 6: exercise_6_security(); break;
            case 7: exercise_7_advanced(); break;
            case 8: exercise_8_practical(); break;
            case 9: exercise_9_debugging(); break;
            case 0: 
                printf(GREEN "Program sonlandırılıyor..." RESET "\n");
                return 0;
            default:
                printf(RED "Geçersiz seçim!" RESET "\n");
        }
        
        printf("\nDevam etmek için Enter'a basın...");
        getchar();
    }
    
    return 0;
}

/*
 * DERLEME TALİMATLARI:
 * gcc -o execve_exercise execve_exercise.c
 * ./execve_exercise
 * 
 * EK ÇALIŞMA ÖNERİLERİ:
 * 1. strace ile programı izleyin: strace -e execve ./execve_exercise
 * 2. Her bölümü dikkatlice çalıştırın ve çıktıları analiz edin
 * 3. Kendi execve örneklerinizi yazın
 * 4. Man sayfasını okuyun: man 2 execve
 * 5. Linux kernel kaynak kodunu inceleyin
 */