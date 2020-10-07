default:
	@gcc -Wall -c linux_shell.c
	@gcc ls_code.c -o ls_code
	@gcc rm_code.c -o rm_code
	@gcc date_code.c -o date_code
	@gcc mkdir_code.c -o mkdir_code
	@gcc cat_code.c -o cat_code
	@gcc linux_shell.o -lreadline -o a
