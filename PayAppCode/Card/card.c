   
#include "card.h"
//*************** String length *************************************

uint32_t strLen(uint8_t* str)
	{
		uint32_t count;
		for (count = 0; str[count] != '\0'; count++);
		return count;
	}

//*************** Card Name *****************************************
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)			//defination of getCardHolderName
	{
	uint8_t CardName[25];
	printf("Enter card holder name: ");
	gets(CardName);
	//scanf_s("%[^\n]s", CardName, (int)25*sizeof(char));		//use '\n' as a delimiter for scanf(), Cast sizeof() to integer
	if (CardName != NULL && strLen(CardName) > 20 && strLen(CardName) < 24)
		{
		for (int i = 0; CardName[i] != '\0'; i++)
			cardData->cardHolderName[i] = CardName[i];
			cardData->cardHolderName[24] = '\0';
			return CARD_OK;
		}
		else
			return WRONG_NAME;
	}
//*************** Card Expiry date***********************************
	EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
		{
			uint8_t exDate[6];
			printf("Enter card expiry date (Format \"MM/YY\", e.g, \"05/25\") : ");
			gets(exDate);
			if (exDate != NULL && strLen(exDate) == 5)
			{
				if (exDate[2] == '/')
				{
					for (int i = 0; exDate[i] != '\0'; i++)
						cardData->cardExpirationDate[i] = exDate[i];
					cardData->cardExpirationDate[5] = '\0';
					return CARD_OK;
				}
					return WRONG_EXP_DATE;
			}
			else
				return WRONG_EXP_DATE;
		}
//*************** Card PAN ******************************************
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
	{
		uint8_t PAN[20];
		printf("Enter card PAN: ");
		gets(PAN);
		if (PAN != NULL && strLen(PAN) >= 16 && strLen(PAN) <= 19)
		{
			int i;
			for ( i = 0; PAN[i] != '\0'; i++)
				cardData->primaryAccountNumber[i] = PAN[i];
			cardData->primaryAccountNumber[i] = '\0';
			return CARD_OK;
		}
		else return WRONG_PAN;
	}
	