//Program to make API call for weather report.

#include <stdio.h>
#define LENGTH 30
#define URL_LENGTH 500

void readUserInput(char* cityName)
{
    printf("Get weather report:\n");
    printf("Enter city name: ");
    scanf("%s", cityName);
}

void getWeatherReport(char* cityName)
{
    char urlFirstPart[URL_LENGTH] = {"https://api.openweathermap.org/data/2.5/weather?q="};
    char urlSecondPart[URL_LENGTH] = {"&appid=abe3a0f4d0b6cebfbe7393b4b4e3aa28&units=metric"};
    char url[URL_LENGTH];

    sprintf(url, "%s%s%s", urlFirstPart, cityName, urlSecondPart);
    char command[URL_LENGTH];
    sprintf(command, "curl \"%s\" > weather.txt", url);
    // printf("%s", command);
    system(command);
}

void main()
{
	char cityName[LENGTH];
    readUserInput(cityName);
    getWeatherReport(cityName);
	
}