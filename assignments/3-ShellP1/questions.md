1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

   > **Answer**: fgets() is a good choice because it will essentially read input line by line automactically up to the amount needed by the user. this prevents the need to manually limit the amount of a line read, and simplifies detecting where lines end. it makes the process of working with stdin much easier for us.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

   > **Answer**: We use malloc, because although we know the maximum amount of memory needed to for cmd_buff, we dont know the minimum. y using malloc(), we allocate exactly the amount of memory needed to store the user's input, which allows us to handle various sized inputs efficiently. This avoids wasting memory when the user enters short commands.

3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

   > **Answer**: We trim leading and trailing spaces from each command in build_cmd_list() to ensure that the command is properly formatted, before we attempt to extract the commands and arguments. If we did not, it would become harder to find were a line actually begins and ends. For example, once we trim the spaces, we now can be certain that the first character is the start of the first command, and that any space after that is not followed by a pipe, denotes a seperation of arguments. if not for the trimming we would have to add many checks to be certain that that was true.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

  > **Answer**: Appending, output, and input redirection are 3 types of redirections we should implement. These types of redirection require the shell to be able to handle opening, reading, overwriting, and adding to files properly which could have numerous issues in implementation.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

  > **Answer**: Redirection allows you to change the source or destination of input/output while Piping connects the output of one command directly to the input of another.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

  > **Answer**: By seperating standard output and errors we can handle them differently, such as redirecting output to a file while still displaying errors on the terminal

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

  > **Answer**: we should provide an option handle them separately, for example maybe a user wants to redirect errors to one file and the standard output to another or the terminal. alternatively, prehaps they want them in the same place. it would depend on the users use case.
