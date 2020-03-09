#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>

#define MYSQL_HOST "localhost"
#define MYSQL_USERNAME "root"
#define MYSQL_PASSWORD "1234"
#define MYSQL_DATABASE "domotica"

void exec_query(MYSQL*con, char query[]);

int main(int argc, char **argv)
{
  char keuze_query;

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

  printf("De query zal opnieuw een database:(Domotica) verwijderen en aanmaken. \n");
  printf("Gaat u hiermee akkoord? [j/n] \t");

  scanf("%c", &keuze_query);


  if(tolower(keuze_query) != 'j')
  {
      exit(1);
  }

  printf("Ik ga nu aan het werk!\n");
  /* Verwijdert de database */
  if(mysql_query(con, "DROP DATABASE domotica"))
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      /*mysql_close(con);
      exit(1);*/
      /* De database kan niet worden verwijderd, maar we moeten wel gewoon doorgaan */
      printf("Database bestaat niet, dus er valt weinig te verwijderen. We gaan wel gewoon door.\n");
  }
    printf("HET DB is foetsie\n");
  /* Maak de database aan */
  if (mysql_query(con, "CREATE DATABASE domotica"))
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }
printf("Het  DB is aangemaakt\n");
con = mysql_init(NULL);
if (mysql_real_connect(con, MYSQL_HOST, MYSQL_USERNAME, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0) == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }

  /* Je hoeft niets te droppen als de database net nieuw is, dus dit commenten we even */
  /*if (mysql_query(con, "DROP TABLE IF EXISTS USER")){
      mysql_close(con);
      exit(1);
  }
  printf("Table USER wordt gedropt\n");*/

  if (mysql_query(con,"CREATE TABLE USER(USER_ID INT AUTO_INCREMENT, "
                                        "USER_PWD VARCHAR(255) NOT NULL, "
                                        "USER_IS_ADMIN BOOL DEFAULT 0, "
                                        "USER_LOGIN_TIME TIME NULL, "
                                        "USER_LOGOUT_TIME TIME DEFAULT 0, primary key(USER_ID))"))
  {
      printf("%s", mysql_error(con));
      mysql_close(con);
      exit(1);
  }


  printf("Table USER is nu aangemaakt met de querie statements\n");

  if (mysql_query(con,"CREATE TABLE DEVICE(DEVICE_ID INT AUTO_INCREMENT, " 
                                        "DEVICE_NAME VARCHAR(255) NOT NULL, "
                                        "DEVICE_TYPE VARCHAR(255) DEFAULT 0, "
                                        "DEVICE_CUR_VALUE INT NULL, "
                                        "DEVICE_DIS_VALUE INT DEFAULT 0, primary key(DEVICE_ID))"))
  {
      printf("%s", mysql_error(con));
      mysql_close(con);
      exit(1);
  }

  printf("Table DEVICE is nu aangemaakt met de querie statements\n");

  if (mysql_query(con,"CREATE TABLE TIME(TIME_ID INT AUTO_INCREMENT, " 
                                        "TIME_TRIGGER_TIME TIME NOT NULL, "
                                        "TIME_DATETIME DATETIME DEFAULT NULL, "
                                        "TIME_DAY_OF_WEEK INT NOT NULL, primary key(TIME_ID))"))
  {
      printf("%s", mysql_error(con));
      mysql_close(con);
      exit(1);
  }

  printf("Table TIME is nu aangemaakt met de querie statements\n");



  mysql_close(con);
  exit(0);
}
/*
void exec_query(MYSQL*con, char query[]) {
  if (mysql_query(con, query))
  {
      printf("%s", mysql_error(con));
      mysql_close(con);
      exit(1);
  }
  printf("DEVICE Table zou nu moeten bestaan\n");
} */
