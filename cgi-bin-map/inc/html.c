void print_html_page_contents(char* file_path) {
	FILE *html_file;
	char current_char;
	char current_command[50];
	int command_buffer_index, file_index;
	size_t file_size;
	html_file = fopen(file_path, "r");
	/* Haal bestandsgrootte op */
	fseek(html_file, 0, SEEK_END);
	file_size = ftell(html_file);
	fseek(html_file, 0, SEEK_SET);
	/* Print elk karakter uit het html-bestand */
	for(file_index = 0; file_index < file_size; file_index++) {
		current_char = (char) fgetc(html_file);
		/* Kijk of er een commando op de huidige positie staat */
		if(current_char == '#') {
			current_command[0] = '\0';
			command_buffer_index = 0;
			current_char = (char) fgetc(html_file);
			file_index++;
			/* Get the string of the current command */
			while(current_char != ')') {
				if(command_buffer_index < 50) {
					strncat(current_command, &current_char, 1);
				} else {
					printf("<div style='margin: 8px; padding-left: 4px; background: red; color: white;'>Wat is dit? Dit commando is te lang. Ik kap ermee! >:(<br>Veel plezier met debuggen!</div>");
					exit(1);
				}
				current_char = (char) fgetc(html_file);
				file_index++;
				command_buffer_index++;
			}
			strncat(current_command, &current_char, 1);
			current_char = (char) fgetc(html_file);
			file_index++;
			execute_command(current_command);
		}
		printf("%c", current_char);
	}
	fclose(html_file);
}
