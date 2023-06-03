
#include "app.h"
void appStart(void)
{
	char x,y,m;									//handling STDIN errors
	char Again = 'y';							// for another tranaction 
	char view = 'y';							// for viewing transaction information 
	uint32_t transNo;							// storing trans NO.
	initDB();

	do {
		EN_transState_t temp = recieveTransactionData(tranDB());					//	store transaction status
		 if (temp == DECLINED_STOLEN_CARD)
			printf("\nTransaction failed (stolen card). ");
		else if (temp == FRAUD_CARD)
			printf("\nTransaction failed (fraud card). ");
		else if (temp == DECLINED_INSUFFECIENT_FUND)
			printf("\nTransaction failed (insuffecient fund). ");
		else if (temp == INTERNAL_SERVER_ERROR)
			printf("\nTransaction failed (server error). ");
		else if (temp == APPROVED)
				printf("%s","Transaction done. ");
		 // if transaction succeed ask user to view info.
		if (temp == APPROVED)
			{
				printf("%s", "\nDo you want to view transaction information (y/n)? ");
				 x = getchar();
				view = getchar();
				if (view == 'y')
				{
					printf("%s", "Enter your transaction sequence number: ");
					int c = scanf("%d", &transNo);
					if (getTransaction(transNo, tranDB()) == SAVING_FAILED)
						printf("\nWrong sequence NO. ");
				}
			}
		printf("%s","\nDo you want to do another transaction(y/n)? ");
		if ( temp == DECLINED_STOLEN_CARD || temp == FRAUD_CARD)
			 y = 'n';
		else
			 y = getchar();
		Again = getchar();
		if (Again == 'y')
		{
			m = getchar();
			system("cls");
			printf("%s", "\n********************************** New transaction  ******************************** \n\n");
		}
	} while(Again == 'y');

}