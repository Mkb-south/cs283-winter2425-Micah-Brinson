1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

   > **Answer**: So that the child process and parent process both execute, rather then the parent process being replaced

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

   > **Answer**: it returns negative 1, and my code just exits with a error

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

   > **Answer**: It checks the PATH variable andthe directories inside

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

   > **Answer**: to make sure that the parent process doesnt continue until the child process has finished.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

   > **Answer**: it checks the childs exit status before it ended

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

   > **Answer**: it checks for the opening and closing qoutes, keeping everything inside them as 1 arguemnt without any edits to make sure the arguemnts are passed as they are given. it allows things like names with spaces to be passed to the shell

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

   > **Answer**: The big changes were handling qouted arguments properly and adding in the branching for the external commands. Also, I no longer needed to work with list of arguemtns and had t orefactor my parsing logic. Handling qoutes was the hardest surprsiingly.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

  > **Answer**: Signals are used used to provide asyncronous notifcations between processes. their asyncronous nature makes them very different from other forms of IPC.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

  > **Answer**: SIGKILL, ends a process no matter what. SIGTERM, terminates a process but can be caught and responded too. similairy SIGINT interrupts a process butcan be caught and respondeed too first.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

  > **Answer**: The process is stopped, no matter any attempts to catch or ignore it, until the proper SIGCONT continues it.
