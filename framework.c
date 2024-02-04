//Program for CRUDS operations.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MENU_ITEM_LENGTH 100
#define FIELD_LENGTH 30
#define FIELD_VALUE_LENGTH 30
#define CUSTOMIZED_MESSAGE_LENGTH 100

FILE *fpMenu;
FILE *fpFields;
FILE *fpData;
FILE *fpMessages;

int fieldsCount	= 0;

void showCustomizedMessages(int messageNumber, char* recordId)
{
	fpMessages = fopen("userMessages.cfg", "r");
	char customizedMessage[CUSTOMIZED_MESSAGE_LENGTH];
	int counter = 0;
	while(fgets(customizedMessage, sizeof(customizedMessage), fpMessages))
	{
		counter++;
		if(counter == messageNumber)
		{
			break;
		}
	}
	customizedMessage[strlen(customizedMessage) - 1] = '\0';
	printf("%s %s.\n", recordId, customizedMessage);
	fclose(fpMessages);

}

void getCountOfFields()
{
	fpFields = fopen("fields.cfg", "r");
	char fieldName[FIELD_LENGTH];

	while(fgets(fieldName, sizeof(fieldName), fpFields) != NULL)
	{
		fieldsCount += 1;
	}
	fclose(fpFields);
}

void readUserInput(char *fieldName, char *fieldValue)
{
	printf("Enter %s ", fieldName);
	scanf("%s", fieldValue);
}


char** getFieldsArray()
{
	fpFields = fopen("fields.cfg", "r");
	char** fieldsArray;	
	fieldsArray = (char**)malloc(fieldsCount * sizeof(char*));
	
	char fieldName[FIELD_LENGTH];
	int counter = 0;
	while(fgets(fieldName, sizeof(fieldName), fpFields) != NULL)
	{
		fieldsArray[counter] = (char*)malloc(FIELD_LENGTH * sizeof(char));
		fieldName[strlen(fieldName) - 1] = '\0';
		strcpy(fieldsArray[counter], fieldName);
		counter++;
	}
	return fieldsArray;
}

int getRecordPosition(char* recordId)
{
	int recordPosition = -1;
	char fieldValue[FIELD_VALUE_LENGTH];
	char recordStatus[FIELD_VALUE_LENGTH];
	fpData = fopen("framework.dat", "r");
	int counter = 0;
	while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
	{
		counter++;
		if(strcmp(fieldValue, recordId) == 0)
		{
			fseek(fpData, -(2 * sizeof(fieldValue)), SEEK_CUR);
			fread(recordStatus, sizeof(recordStatus), 1, fpData);
			fseek(fpData, -sizeof(recordStatus), SEEK_CUR);
			if(strcmp(recordStatus, "A") == 0)
			{
				recordPosition = ftell(fpData);
			}
			break;
		}
	}
	fclose(fpData);
	return recordPosition;
}

void readNewFieldValue(char* fieldName, char* newFieldValue)
{
	printf("Enter %s ", fieldName);
	scanf("%s", newFieldValue);
}

void searchRecord()
{
	char recordId[FIELD_VALUE_LENGTH];
	char** fieldNames = getFieldsArray();
	readUserInput(fieldNames[1], recordId);
	int recordPosition = getRecordPosition(recordId);
	char fieldValue[FIELD_VALUE_LENGTH];
	fpData = fopen("framework.dat", "r");
	fseek(fpData, recordPosition, SEEK_SET);
	int counter;
	if(recordPosition != -1)
	{
		for(counter = 0; counter < fieldsCount; counter++)
		{
			fread(fieldValue, sizeof(fieldValue), 1, fpData);
			if(strcmp(fieldValue, "A") == 0)
			{
				continue;
			}
			printf("%s %s\n", fieldNames[counter], fieldValue);
		}
	}
	else
	{
		int messageNumber = 4;
		showCustomizedMessages(messageNumber, recordId);
	}
	fclose(fpData);
}

void deleteRecord()
{
	char recordId[FIELD_VALUE_LENGTH];
	char** fieldNames = getFieldsArray();
	readUserInput(fieldNames[1], recordId);

	int recordPosition = getRecordPosition(recordId);
	fpData = fopen("framework.dat", "r+");

	char fieldValue[FIELD_VALUE_LENGTH];
	if(recordPosition != -1)
	{
		fseek(fpData, recordPosition, SEEK_SET);
		fread(fieldValue, FIELD_VALUE_LENGTH, 1, fpData);
		strcpy(fieldValue, "I");
		fseek(fpData, -sizeof(fieldValue), SEEK_CUR);
		fwrite(fieldValue, sizeof(fieldValue), 1, fpData);
		int messageNumber = 3;
		showCustomizedMessages(messageNumber, recordId);

	}
	else
	{
		int messageNumber = 4;
		showCustomizedMessages(messageNumber, recordId);
	}
	fclose(fpData);
}

void updateRecord()
{
	char recordId[FIELD_VALUE_LENGTH];
	char** fieldNames = getFieldsArray();
	readUserInput(fieldNames[1], recordId);

	int recordPosition = getRecordPosition(recordId);

	fpData = fopen("framework.dat", "r+");
	char fieldValue[FIELD_VALUE_LENGTH];
	char newFieldValue[FIELD_VALUE_LENGTH];

	if(recordPosition != -1)
	{
		int fieldValuePosition = recordPosition + ((fieldsCount - 1) * FIELD_VALUE_LENGTH);
		fseek(fpData, fieldValuePosition, SEEK_SET);
		fread(fieldValue, FIELD_VALUE_LENGTH, 1, fpData);
		readNewFieldValue(fieldNames[fieldsCount - 1], newFieldValue);
		fseek(fpData, -sizeof(fieldValue), SEEK_CUR);
		fwrite(newFieldValue, sizeof(newFieldValue), 1, fpData);
		int messageNumber = 2;
		showCustomizedMessages(messageNumber, recordId);
	}
	else
	{
		int messageNumber = 4;
		showCustomizedMessages(messageNumber, recordId);
	}
	fclose(fpData);
}

void showAllRecords()
{
	char fieldValue[FIELD_VALUE_LENGTH];
	char** fieldNames = getFieldsArray();
	fpData = fopen("framework.dat", "r");
	int counter;

	while(fread(fieldValue, sizeof(fieldValue), 1, fpData))
	{
		if(strcmp(fieldValue, "A") == 0)
		{
			for(counter = 0; counter < fieldsCount; counter++)
			{
				// printf("%s\n", fieldValue);
				// printf("%d\n", counter);
				if(strcmp(fieldValue, "A") == 0)
				{
					fread(fieldValue, sizeof(fieldValue), 1, fpData);
					continue;
				}
				printf("%s %s\n", fieldNames[counter], fieldValue);
				fread(fieldValue, sizeof(fieldValue), 1, fpData);
				// printf("%s\n", fieldValue);
			}
			printf("\n");
			fseek(fpData, -sizeof(fieldValue), SEEK_CUR);
		}
	}
		
	fclose(fpData);
}

void createRecord()
{
	char **fieldNames = getFieldsArray();
	fpData = fopen("framework.dat", "a");
	int counter;
	char fieldValue[FIELD_VALUE_LENGTH];	
	char recordStatus[FIELD_VALUE_LENGTH] = {"A"};
	for(counter = 0; counter < fieldsCount; counter++)
	{
		if(counter == 0)
		{
			fwrite(recordStatus, sizeof(recordStatus), 1, fpData);
			continue;
		}
		else
		{
			readUserInput(fieldNames[counter], fieldValue);
			fwrite(fieldValue, sizeof(fieldValue), 1, fpData);

		}
	}
	fclose(fpData);
}

int getUserChoice()
{
	int choice;
	scanf("%d", &choice);
	return choice;
}

void showMenu()
{
	char menuName[MENU_ITEM_LENGTH];
	fpMenu = fopen("menu.cfg", "r");
	while(fgets(menuName, sizeof(menuName), fpMenu) != NULL)
	{
		printf("%s ", menuName);
	}
	
	int choice = getUserChoice();
	fclose(fpMenu);

	switch(choice)
	{
	case 0:
		exit(0);
	case 1:
		createRecord();
		break;
	case 2:
		showAllRecords();
		break;
	case 3:
		updateRecord();
		break;
	case 4:
		searchRecord();
		break;
	case 5:
		deleteRecord();
		break;
	default:
		printf("Please select a correct option.\n");
	}
}


void main()
{
	getCountOfFields();
	do
	{
		showMenu();
	}while(1);
}
