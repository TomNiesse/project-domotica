void print_html_page_contents(char* file_path) {
        FILE *html_file;
        char current_char;
        char current_command[50];
        int command_buffer_index;
        html_file = fopen(file_path, "r");
        /* Zolang het einde van het bestand nog niet is bereikt... */
        while((current_char = (char) fgetc(html_file)) != EOF) {
                /* ...moeten er karakters worden geprint */
                /* Kijk of er een commando op de huidige positie staat */
                if(current_char == '#') {
                        /* Een commando is mogelijk gevonden! Stop even met printen, en voer m uit */
                        current_char = (char) fgetc(html_file);
                        if(current_char == '>') {
                                /* Clear any previous commands */
                                current_command[0] = '\0';
                                command_buffer_index = 0;
                                current_char = (char) fgetc(html_file);
                                /* Get the string of the current command */
                                while(current_char != ')') {
                                        if(command_buffer_index < 50) {
                                                strncat(current_command, &current_char, 1);
                                        } else {
                                                printf("<div style='margin: 8px; padding-left: 4px; background: red; color: white;'>Wat de neuk is dit? Dit commando is te lang. Ik kap ermee! >:(<br>Veel plezier met debuggen!</div>");
                                                exit(1);
                                        }
                                        current_char = (char) fgetc(html_file);
                                        command_buffer_index++;
                                }
                                strncat(current_command, &current_char, 1);
                                current_char = (char) fgetc(html_file);
                                execute_command(current_command);
                        } else {
                                printf("#%c", current_char);
                        }
                }
                printf("%c", current_char);
        }
        fclose(html_file);
}
