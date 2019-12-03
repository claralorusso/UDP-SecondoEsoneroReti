#include <string.h>

int count(char* c){
	int countv = 0;
	int i;
	for(i = 0; i <= strlen(c); i++) {
	    switch (c[i])
	    {
	    case 'a': case 'A':
	    	countv++;
	    	break;
	    case 'e': case 'E':
	    	countv++;
	    	break;
	    case 'i': case 'I':
	    	countv++;
	    	break;
	    case 'o': case 'O':
	    	countv++;
	    	break;
	    case 'u': case 'U':
	    	countv++;
	    	break;
	    }
	}
return countv;
}


