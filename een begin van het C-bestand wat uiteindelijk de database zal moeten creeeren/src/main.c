#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

#define MYSQL_HOST "localhost"
#define MYSQL_USERNAME "root"
#define MYSQL_PASSWORD "1234"

int main(int argc, char **argv)
{  
  MYSQL *con = mysql_init(NULL);

  /* Kijk of mysql_init() zn werk wel heeft gedaan */
  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  /* Kijk of er een verbinding mogelijk is met de database */
  if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, NULL, 0, NULL, 0) == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }  

  /* Maak de database aan */
  if (mysql_query(con, "CREATE DATABASE domotica")) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }

  printf("db zou nu moeten bestaan");

  mysql_close(con);
  exit(0);
}
