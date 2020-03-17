void command_test() {
        printf("test succesvol!");
}

void print_css_file_contents() {
        FILE *css_file;
        char current_char;
        size_t file_size;
        int file_index;
        css_file = fopen("html_root/css/tailwind.min.css", "r");
        /* Haal bestandsgrootte op */
        fseek(css_file, 0, SEEK_END);
        file_size = ftell(css_file);
        fseek(css_file, 0, SEEK_SET);
        /* Print alle chars uit het CSS-bestand uit */
        for(file_index = 0; file_index < file_size; file_index++) {
                current_char = (char) fgetc(css_file);
                printf("%c", current_char);
        }
        fclose(css_file);
}

void execute_command(char* command) {
        if(strcmp(command, "command_test()") == 0) {
                command_test();
                return;
        } else if(strcmp(command, "print_css_file_contents()") == 0) {
                print_css_file_contents();
                return;
	      } else if(strcmp(command, "airco_get_current_temp_value()") == 0) {
		            /* Dit moet later nog worden opgehaald uit de database */
		            printf("27");	
	      } else if(strcmp(command, "airco_get_desired_temp_value()") == 0) {
		            /* Ook dit moet later nog worden opgehaald uit database */
		            printf("99");
        } else {
                printf("<div style='background: red; color: white; padding-left: 4px; margin: 8px;'>Neef wtf! Commando \"%s\" bestaat niet.<br>Spontane typaids ontwikkeld?<br>Een grove denkfout?<br>Iets anders?<br>Wat zou het probleem toch kunnen zijn?</div>", command);
        }
}

