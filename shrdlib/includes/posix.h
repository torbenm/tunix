
// Files and Directories

// char *getcwd(char *buf, size_t size);
// get current working directory
// int mkdir(const char *pathname, mode_t mode);
// create a directory
// int rmdir(const char *pathname);
// delete a directory
// int chdir(const char *path);
// change working directory
// int link(const char *oldpath, const char *newpath);
// make a new name for a file
// int unlink(const char *pathname);
// delete a name and possibly the file it refers to
// int rename(const char *oldpath, const char *newpath);
// change the name or location of a file
// int stat(const char *file_name, struct stat *buf);
// get file status
// int chmod(const char *path, mode_t mode);
// change permissions of a file
// int chown(const char *path, uid_t owner, gid_t group);
// change ownership of a file
// int utime(const char *filename, struct utimbuf *buf);
// change access and/or modification times of an inode
// DIR *opendir(const char *name);
// open a directory
// struct dirent *readdir(DIR *dir);
// read directory entry
// int closedir(DIR *dir);
// close a directory
// void rewinddir(DIR *dir);
// reset directory stream

// Advanced File Operations

// int access(const char *pathname, int mode);
// check user's permissions for a file
// int open(const char *pathname, int flags);
// open and possibly create a file or device
// int creat(const char *pathname, mode_t mode);
// open and possibly create a file or device
// int close(int fd);
// close a file descriptor
// ssize_t read(int fd, void *buf, size_t count);
// read from a file descriptor
// ssize_t write(int fd, const void *buf, size_t count);
// write to a file descriptor
// int fcntl(int fd, int cmd);
// manipulate file descriptor
// int fstat(int filedes, struct stat *buf);
// get file status
// off_t lseek(int fildes, off_t offset, int whence);
// reposition read/write file offset
// int dup(int oldfd);
// duplicate a file descriptor
// int dup2(int oldfd, int newfd);
// duplicate a file descriptor
// int pipe(int filedes[2]);
// create pipe
// int mkfifo ( const char *pathname, mode_t mode );
// make a FIFO special file (a named pipe)
// mode_t umask(mode_t mask);
// set file creation mask
// FILE *fdopen (int fildes, const char *mode);
// associate a stream with an existing file descriptor
// int fileno( FILE *stream);
// return file descriptor of stream

// Processes

// create a child process
// int execl( const char *path, const char *arg, ...);
// execute a file
// execle
// execute a file
// int execlp( const char *file, const char *arg, ...);
// execute a file
// int execv( const char *path, char *const argv[]);
// execute a file
// execve
// execute program
// int execvp( const char *file, char *const argv[]);
// execute a file
// pid_t waitpid(pid_t pid, int *status, int options);
// wait for process termination
// pid_t waitpid(pid_t pid, int *status, int options);
// wait for process termination
// void _exit(int status);
// terminate the current process
// int kill(pid_t pid, int sig);
// send signal to a process
// unsigned int sleep(unsigned int seconds);
// Sleep for the specified number of seconds
// int pause(void);
// wait for signal
// unsigned int alarm(unsigned int seconds);
// set an alarm clock for delivery of a signal
// setuid
// set user identity
// setgid
// set group identity

// Long Jumps

// int setjmp(jmp_buf env);
// save stack context for non-local goto
// int sigsetjmp(sigjmp_buf env, int savesigs);
// save stack context for non-local goto
// void longjmp(jmp_buf env, int val);
// non-local jump to a saved stack context
// void siglongjmp(sigjmp_buf env, int val);
// non-local jump to a saved stack context

// Signal Handling

// struct sigaction *oldact);
// examine and change signal action
// int sigemptyset(sigset_t *set);
// create an empty signal set
// int sigfillset(sigset_t *set);
// create a full set of signals
// int sigaddset(sigset_t *set, int signum);
// add a signal to a signal set
// int sigdelset(sigset_t *set, int signum);
// remove a signal from a signal set
// int sigismember(const sigset_t *set, int signum);
// test a signal set for a selected member
// sigprocmask
// examine and change blocked signals
// int sigpending(sigset_t *set);
// examine pending signals
// int sigsuspend(const sigset_t *mask);
// wait for a signal

// Obtaining Information at Runtime

// pid_t getpid(void);
// get process identification
// pid_t getppid(void);
// get parent process identification
// char * getlogin ( void );
// get user name
// uid_t getuid(void);
// get user identity
// uid_t geteuid(void);
// get effective user identity
// char * cuserid ( char *string );
// get user name
// gid_t getgid(void);
// get group identity
// gid_t getegid(void);
// get effective group identity
// struct passwd *getpwuid(uid_t uid);
// get password file entry based on user id
// struct passwd *getpwnam(const char * name);
// get password file entry based on user name
// struct group *getgrgid(gid_t gid);
// get group file entry based on group id
// struct group *getgrnam(const char *name);
// get group file entry baes on group name
// int getgroups(int size, gid_t list[]);
// get list of supplementary group IDs
// char *ctermid(char *s);
// get controlling terminal name
// int uname(struct utsname *buf);
// get name and information about current kernel
// char *getenv(const char *name);
// get an environment variable
// long sysconf(int name);
// get configuration information at runtime
// long fpathconf(int filedes, int name);
// get configuration values for files
// long fpathconf(int filedes, int name);
// get configuration values for files
// int isatty ( int desc );
// does this descriptor refer to a terminal
// char *ttyname ( int desc );
// return name of a terminal
// clock_t times(struct tms *buf);
// get process times
// void tzset (void);
// initialize time conversion information

// Terminal I/O
// #include <termios.h>

// int tcgetattr ( int fd, struct termios *termios_p );
// get terminal attributes
// tcsetattr
// set terminal attributes
// int tcdrain ( int fd );
// wait for all output to be transmitted to the terminal
// int tcflow ( int fd, int action );
// suspend/restart terminal output
// int tcflush ( int fd, int queue_selector );
// discard terminal data
// int tcsendbreak ( int fd, int duration );
// send a break to a terminal
// speed_t cfgetispeed ( struct termios *termios_p );
// get input baud rate
// speed_t cfgetospeed ( struct termios *termios_p );
// get output baud rate
// cfsetispeed
// set input baud rate
// cfsetospeed
// set output baud rate
// pid_t tcgetpgrp ( int fd );
// get terminal foreground process group ID
// int tcsetpgrp ( int fd, pid_t pgrpid );
// set terminal foreground process group ID

// Process Groups and Job Control

// pid_t setsid(void);
// creates a session and sets the process group ID
// int setpgid(pid_t pid, pid_t pgid);
// set process group
// pid_t getpgrp(void);
// get process group