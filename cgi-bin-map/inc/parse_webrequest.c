/*
 * This function takes any GET/POST request url and converts it into two arrays containing parameters and values.
 * WARNING: This function will completely destroy your request string. Make a stringcopy of it if you need it later!
 * */
short parse_webrequest(char* request, char params[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH], char values[MAX_ARRAY_LENGTH][MAX_STRING_LENGTH]) {
		long unsigned int tmp_index = 0;
		long unsigned int arrays_index = 0;
		short params_set = 0;

		/* Find the position of the question mark, to do a string split */
		for(tmp_index = 0; tmp_index < strlen(request); tmp_index++) {
				if(request[tmp_index] == '?') {
						/* The position of the questionmark was found, remove the first X chars from the request now */
						request+=(int)tmp_index+1;
						/* and stop looking for another question mark */
						break;
				}
		}


		/*
		 * Now To parse all the values
		 * */
		while(strstr(request, "=")) {
				/* Get the first param by searching for the next '=' */
				for(tmp_index = 0; tmp_index < strlen(request); tmp_index++) {
						if(request[tmp_index] == '=') {
								/* string copy the param into the params array */
								strncpy(&params[arrays_index][0], request, (size_t)tmp_index);
								/* increase the string pointer, so C starts looking at a later point in memory */
								request+=(int)tmp_index+1;
								/* do not continue searching for the next '=' */
								break;
						}
				}
				/* Get it's value */
				/* Get the first value by searching for the next '&' */
				for(tmp_index = 0; tmp_index < strlen(request); tmp_index++) {
						if(request[tmp_index] == '&') {
								/* string copy the value into the values array */
								strncpy(&values[arrays_index][0], request, (size_t)tmp_index);
								/* increase the string pointer, so C things the string starts at the given offset */
								request+=(int)tmp_index+1;
								/* do not continue to look for the next '&' */
								break;
						}
				}
				/* move on to the next param/value pair */
				arrays_index++;
		};
		/* Now, copy the last value into place manually, the while loop has already ended here */
		/* stringcopy the last value into place, telling C the string is long */
		strncpy(&values[arrays_index-1][0], request, (size_t)MAX_STRING_LENGTH);
		return arrays_index-1;
}
