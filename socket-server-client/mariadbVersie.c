#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <mariadb/mysql.h>

int main(int argc, char **argv)
{
	printf("MariaDB client version: %s\n", mysql_get_client_info());

	exit(0);
}
