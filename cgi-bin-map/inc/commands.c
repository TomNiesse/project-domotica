void command_test() {
	printf("test succesvol!");
}

void print_css_file_contents() {
	FILE *css_file;
	char current_char;
	long int file_size;
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

void print_css_file_robotstofzuiger() {
	FILE *css_file;
	char current_char;
	long int file_size;
	int file_index;
	css_file = fopen("html_root/css/robotstofzuiger.css", "r");
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
	}else if(strcmp(command, "print_css_file_robotstofzuiger()") == 0) {
		print_css_file_robotstofzuiger();
		return;
	}
	/* Commands voor tijden van de robotstofzuiger uitlezen uit de database*/
	else if(strcmp(command, "robotstofzuiger_get_time(1, 0)") == 0) 
	{
		robotstofzuiger_get_time(1, 0);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(0, 0)") == 0) 
	{
		robotstofzuiger_get_time(0, 0);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(1, 1)") == 0) 
	{
		robotstofzuiger_get_time(1, 1);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(0, 1)") == 0) 
	{
		robotstofzuiger_get_time(0, 1);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(1, 2)") == 0) 
	{
		robotstofzuiger_get_time(1, 2);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(0, 2)") == 0) 
	{
		robotstofzuiger_get_time(0, 2);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(1, 3)") == 0) 
	{
		robotstofzuiger_get_time(1, 3);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(0, 3)") == 0) 
	{
		robotstofzuiger_get_time(0, 3);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(1, 4)") == 0) 
	{
		robotstofzuiger_get_time(1, 4);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(0, 4)") == 0) 
	{
		robotstofzuiger_get_time(0, 4);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(1, 5)") == 0) 
	{
		robotstofzuiger_get_time(1, 5);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(0, 5)") == 0) 
	{
		robotstofzuiger_get_time(0, 5);
	} 
	else if(strcmp(command, "robotstofzuiger_get_time(1, 6)") == 0) 
	{
		robotstofzuiger_get_time(1, 6);
	}
	else if(strcmp(command, "robotstofzuiger_get_time(0, 6)") == 0) 
	{
		robotstofzuiger_get_time(0, 6);
	}

	/* Commands voor handmatige bedienening van de robotstofzuiger */
	else if(strcmp(command, "robotstofzuiger_get_state(aan)") == 0) 
	{
		robotstofzuiger_get_state("aan");
	} 
	else if(strcmp(command, "robotstofzuiger_get_state(uit)") == 0) 
	{
		robotstofzuiger_get_state("uit");
	}

	/* Command voor accuspanning uitlezen uit de database */
	else if(strcmp(command, "robotstofzuiger_get_voltage()") == 0) 
	{
		printf("%.2f", robotstofzuiger_get_voltage());
	}

	/* Commands Airco */
	else if(strcmp(command, "airco_get_current_value()") == 0) {
		/* Haal de waarde op uit de database */
		printf("%d", airco_get_current_value());
	} else if(strcmp(command, "airco_get_desired_value()") == 0) {
		/* Haal de waarde op uit de database */
		printf("%d", airco_get_desired_value());
	} else {
		printf("<div style='background: red; color: white; padding-left: 4px; margin: 8px;'>Neef wtf! Commando \"%s\" bestaat niet.<br>Spontane typaids ontwikkeld?<br>Een grove denkfout?<br>Iets anders?<br>Wat zou het probleem toch kunnen zijn?</div>", command);
	}
}
