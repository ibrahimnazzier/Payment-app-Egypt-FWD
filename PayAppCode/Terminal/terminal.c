#include<stdio.h>
#include "../../PayApp/Terminal/terminal.h"


// 1- getting transaction date
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint8_t tranDate[11];
	printf("Enter transaction date (format  DD/MM/YYYY ): ");
	gets(tranDate);
	if (tranDate != NULL && strLen(tranDate) == 10)
	{
		if (tranDate[2] != '/' && tranDate[5] != '/')
			return WRONG_DATE;
		else
		{
			for (int i = 0; tranDate[i] != '\0'; i++)
				termData->transactionDate[i] = tranDate[i];
			termData->transactionDate[10] = '\0';
			return TERMINAL_OK;
		}
	}
	else return WRONG_DATE;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	int termTempY, cardTempY, termTempM, cardTempM;			// two temp variables to be compared with each others
	termTempY = (int)(termData->transactionDate[8] - 48) * 10 + (int)termData->transactionDate[9] - 48; // assuming years are in 
																									   //the same century
	cardTempY = (int)(cardData->cardExpirationDate[3] - 48) * 10 + (int)cardData->cardExpirationDate[4] - 48;
	if (cardTempY > termTempY)
		return TERMINAL_OK;
	else if (cardTempY == termTempY)
	{
		termTempM = (int)(termData->transactionDate[3] - 48) * 10 + (int)termData->transactionDate[4] - 48;
		cardTempM = (int)(cardData->cardExpirationDate[0] - 48) * 10 + (int)cardData->cardExpirationDate[1] - 48;
		if (cardTempM >= termTempM)
			return TERMINAL_OK;
		else
			return EXPIRED_CARD;

	}
	else
		return EXPIRED_CARD;
}
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	if (getCardPAN(cardData) == WRONG_PAN)
		return INVALID_CARD;
	else
		return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float temp;
	printf("Enter your transaction amount: ");
	scanf_s("%f", &temp);
	if (temp > 0)
	{
		termData->transAmount = temp;
		return TERMINAL_OK;
	}
	else
		return INVALID_AMOUNT;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->maxTransAmount >= termData->transAmount)
		return TERMINAL_OK;
	else
		return EXCEED_MAX_AMOUNT;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float Max)
{
	if (Max > 0)
	{
		termData->maxTransAmount = Max;
		return TERMINAL_OK;
	}
	else
		return INVALID_MAX_AMOUNT;
}

