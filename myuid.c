#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

struct passwd pwd;

int default_reaction(){
    pid_t pid = getpid();
    pid_t ppid = getppid();
    uid_t uid = getuid();
    gid_t gid = getgid();
    int len = 128;
    gid_t list[128];
    for (int i = 0; i < len; i++){
        list[i] = 0;
    }

    int n = getgroups(len, list);
    printf("pid=%d\nppid=%d\nuid=%d(%s)\ngid=%d(%s)\n", pid, ppid, uid, getpwuid(uid)->pw_name, gid, getgrgid(gid)->gr_name);
    printf("groups=");
    for (int i = 0; i < n; i++){
        printf("%d(%s), ", list[i], getgrgid(list[i])->gr_name);
    }
    putchar('\n');
    return 0;
}

int main(int argc, char* argv[]){
    if (argc == 1) 
        return default_reaction();
    else {
        printf("%s\n", argv[1]);
        if (getpwnam(argv[1]) == 0){
            printf("id: ‘%s’: no such user\n", argv[1]);
            return 0;
        }
        printf("uid=%d(%s)\ngid=%d(%s)\n", getpwnam(argv[1])->pw_uid, argv[1], getgrnam(argv[1])->gr_gid, argv[1]);
        gid_t groups[128];
        int ngroups=128;
        int n = getgrouplist(argv[1], getgrnam(argv[1])->gr_gid, groups, &ngroups);
        printf("groups=");
        for(int i = 0; i < n; i++){
            printf("%d(%s), ", groups[i], getgrgid(groups[i])->gr_name);
        }
        putchar('\n');
        return 0;
    }
}
