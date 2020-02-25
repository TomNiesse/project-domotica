void command_test() {
        printf("test succesvol!");
}

void execute_command(char* command) {
        if(strcmp(command, "command_test()") == 0) {
                command_test();
                return;
        } else if(strcmp(command, "print_css_file_contents()") == 0) {
                print_css_file_contents();
                return;
        } else {
                printf("<div style='background: red; color: white; padding-left: 4px; margin: 8px;'>Neef wtf! Commando \"%s\" bestaat niet.<br>Spontane typaids ontwikkeld?<br>Een grove denkfout?<br>Iets anders?<br>Wat zou het probleem toch kunnen zijn?</div>", command);
        }
}

