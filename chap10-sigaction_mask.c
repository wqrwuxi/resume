
void sigaction_mask(const char *str,struct sigaction sigset)
{
   int			errno_save;
   printf("%s", str);
   if (sigismember(&sigset.sa_mask, SIGINT))
	  printf("include SIGINT");
   if (sigismember(&sigset.sa_mask, SIGQUIT))
	  printf("include SIGQUIT");
   printf("\n");
   errno = errno_save;	

}
