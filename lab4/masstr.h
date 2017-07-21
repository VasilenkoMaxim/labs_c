struct masstr{
	char **str;
	int *nchar;
	int nstr;
};
typedef struct masstr masstr;
int init_masstr(masstr *M, int nstr);
int destroy_massstr(masstr *M);
int add_str_masstr(masstr *M, int n, char *str);
int free_str_masstr(masstr *M, int n);
