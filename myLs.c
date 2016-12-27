#include "apue.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>


/*function declarations*/
int hiddenFiles(const struct dirent *entry);
void printEntries(int n, struct dirent **entries);
int myAlphasort(const struct dirent **d1, const struct dirent **d2);
void printReverse(int n, struct dirent **entries);

/*
first element = .exe file
second element = arugument
argc is automatically generated but not included in argv[]
*/
int main(int argc, char *argv[])
{
  /*type def*/
  /*represents directory*/
	DIR			*dp;

  /*directory entries*/
  /*dirp has two astericks because a string is a char array and in a list of strings, each index is an array inside of an array*/
	struct dirent		**dirp;

	/*represents number of dirents*/
	int n;

  /*Case 1: only the exe is provided*/
	if (argc == 1)
	{
		if( (dp = opendir(".") ) == NULL)
			 err_sys("Current directory has no entries.");
		else
		{
	    /*scandir(directory, pointer to directory entires, function that acts as a filter, function for sorting*/
	    /*scandir returns the number of directory entries*/
	    /*each directory is passed into function hiddenFiles*/
			n = scandir(".", &dirp, hiddenFiles, myAlphasort);

			/*print sorted list*/
			printEntries(n, dirp);
		}
	}

  /*Case 2: the exe file is provided and the diretory OR -a OR -r*/
	else if(argc == 2)
	{
	    /*if the user provided exe and -a*/
	    /*0 indicates equality of strings*/
	    if(strcmp(argv[1], "-a") == 0)
	    {
            if ( (dp = opendir(".")) == NULL )
		         err_sys("Can't open current directory");
		    else
		    {
		        /*filter is not provided*/
			    n = scandir(".", &dirp, NULL, myAlphasort);
			    printEntries(n, dirp);
		    }
	    }

	    /*if the user provided exe and -r*/
	    else if(strcmp(argv[1], "-r") == 0)
	    {
            if ( (dp = opendir(".")) == NULL )
		        err_sys("Can't open current directory");
		    else
		    {
			    n = scandir(".", &dirp, hiddenFiles, myAlphasort);
				/*print contents in reverse*/
			    printReverse(n, dirp);
		    }
	    }

	    /*if the user provided exe and directory name*/
	    else
	    {
	        if ( (dp = opendir(argv[1])) == NULL )
		        err_sys("Can't open current directory");
		    else
		    {
			    n = scandir(argv[1], &dirp, hiddenFiles, myAlphasort);
			    printEntries(n, dirp);
		    }
	    }
	}

	/*Case 3: the exe file, directory and -a OR -r is provided*/
	else if(argc == 3)
	{
		/*user provided exe, directory and -r*/
		if(strcmp(argv[2], "-r") == 0)
		{
			if ( (dp = opendir(argv[1])) == NULL )
		  		err_sys("can't open %s",argv[1]);

		  	else
	    	{
		    	n = scandir(argv[1], &dirp, hiddenFiles, myAlphasort);
				/*print contents in reverse*/
				printReverse(n, dirp);
			}
		}

		  /*user provided exe, directory and -a*/
	    else
	    {
	    	if ( (dp = opendir(argv[1])) == NULL )
		  		err_sys("can't open %s",argv[1]);
		  	else
		  	{
				/*filter is not provided*/
		    	n = scandir(argv[1], &dirp, NULL, myAlphasort);
				printEntries(n, dirp);
		  	}
		}
	}

	closedir(dp);
	exit(0);

	return 0;
}

/*
Prints and deallocates directory entries that were malloced by scandir
*/
void printEntries(int n, struct dirent **entries)
{
	int i = 0;

	if(n < 0)
	{
		perror("No directory entries.");
	}

	else
	{
		while(i < n)
		{
			printf("%s\n", entries[i++]->d_name);

			/*each entrie needs to be free'd because scandir allocates the directory entries using 'malloc'*/
			free(entries[n]);
		}

		/*don't forget to free index 0*/
		free(entries[n]);
	}
}

/*
Determines whether the directory entry begins with a '.'
*/
int hiddenFiles(const struct dirent *entry)
{
    /*if the entry is a hidden file*/
	if(entry->d_name[0] == '.')
	    /*return 0 so it is not included*/
		return 0;
    else
        return 1;
}

/*Compares directory entries*/
int myAlphasort(const struct dirent **d1, const struct dirent **d2)
{
	/*
	strcasecmp only compares two pointers to a dirent *variable, but scandir requires a function that accepts two dirent **variables, so
	i met the function requirements and then casted my parameters to a single asterick
	*/
	return(strcasecmp((*(struct dirent **)d1)->d_name,
	    (*(struct dirent **)d2)->d_name));
}

/*Prints directory entries in reverse and deallocates directory entries*/
void printReverse(int n, struct dirent **entries)
{
	int i = 0;

	if(n < 0)
	{
		perror("No directory entries.");
	}

	else
	{
		while(i < n)
		{
			/*predecrement to stay within bounds*/
			printf("%s\n", entries[--n]->d_name);

			/*free entries that were automatically malloced by function scandir*/
			free(entries[n]);
		}
	}
}
