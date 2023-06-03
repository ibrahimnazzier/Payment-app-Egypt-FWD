#include "../../PayApp/Server/server.h"

ST_accountsDB_t AccdataBase[255];
ST_transaction_t tranDataBase[255] = { 0 };
uint32_t client = 0; // for searching in account database 
uint32_t seq = 0 ;	// for storing sequence number
clock_t TTL; //simulatting time to live
uint32_t no = 0;	// for transDataBase

void initDB()
{
	for (int i = 0; i < 5; i++)
	{
		AccdataBase[i].balance = i * 1000.0 + 1000.0;
		if (i % 2)
			AccdataBase[i].state = BLOCKED;
		else
			AccdataBase[i].state = RUNNING;
	
	}
	
	strcpy_s(AccdataBase[0].primaryAccountNumber, 20 * sizeof(char),"123456789987654321");
	strcpy_s(AccdataBase[1].primaryAccountNumber, 20 * sizeof(char), "147258369147258369");
	strcpy_s(AccdataBase[2].primaryAccountNumber, 20 * sizeof(char), "123654789123654789");
	strcpy_s(AccdataBase[3].primaryAccountNumber, 20 * sizeof(char), "369258147369258147");
	strcpy_s(AccdataBase[4].primaryAccountNumber, 20 * sizeof(char), "147369258147369258");
	
}

ST_transaction_t* tranDB() {
	ST_transaction_t* temp = tranDataBase;
	return temp;
}
ST_accountsDB_t* accountReference()
{
	ST_accountsDB_t* temp;
	temp = &AccdataBase;
	return temp;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)			//take the address of trransdatabase
{
	if (isValidAccount(&transData[no].cardHolderData, AccdataBase) == ACCOUNT_NOT_FOUND) 
	// use transData to store data directly rather than store it in another variable.
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;
	}
	else if (isBlockedAccount(&AccdataBase[client]) == BLOCKED_ACCOUNT || getTransactionDate(&transData[no].terminalData) == WRONG_DATE || isCardExpired(&transData[no].cardHolderData, &transData[no].terminalData) == EXPIRED_CARD)
	// take tranaction date through get transaction date.
	// check if date is ok through is card expired 
	{
		transData->transState = FRAUD_CARD;
		return FRAUD_CARD;
	}
	else if (isAmountAvailable(&(transData[no].terminalData)) == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	else if (saveTransaction(transData,50) == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}
	no++;
	transData->transState = APPROVED;
	AccdataBase[client].balance -= transData[no - 1].terminalData.transAmount;
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	while (getCardHolderName(cardData) == WRONG_NAME);
	while (getCardExpiryDate(cardData) == WRONG_EXP_DATE);
	while (getCardPAN(cardData) == WRONG_PAN);

	for (int i = 0; i < 255; i++)
	{
		if (strcmp(&cardData->primaryAccountNumber, accountRefrence[i].primaryAccountNumber) == 0)
		{
			client = i;
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if ((accountRefrence+client)->state == BLOCKED)
		return BLOCKED_ACCOUNT;
	else
		return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
		setMaxAmount(termData, 10000.0);			//setting max amount 
		if (getTransactionAmount(termData) == TERMINAL_OK)		
			if (AccdataBase[client].balance >= termData->transAmount)	// checking for valid balance 
					return SERVER_OK;
			else
				return LOW_BALANCE;
}
EN_serverError_t saveTransaction(ST_transaction_t* transData, int arg)
{
	TTL = clock();
	for (int i = 0; i < arg; i++);
	TTL = clock() - TTL;
	if (TTL > 0.60)
	{
		return SAVING_FAILED;
	}
	else
	{
		seq++;
		transData[seq - 1 ].transactionSequenceNumber = seq;
		printf("Your transaction No. %d\n", seq);
		return SERVER_OK;
	}
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	printf("%d", transactionSequenceNumber);
		if (transactionSequenceNumber <= transData[no - 1].transactionSequenceNumber && transactionSequenceNumber >= 0)	// if for checking if the seq
		{																											// seq No. existed in trans
			printf("\n********************************** Transaction information ********************************");										//database or not 
			printf("\nCard holder name: %s\n", transData[transactionSequenceNumber - 1].cardHolderData.cardHolderName); //transseq - 1 as the seq 
			printf("Card holder PIN: %s\n", transData[transactionSequenceNumber - 1].cardHolderData.primaryAccountNumber); // starts from 1
			printf("Trans amount: %f\n", transData[transactionSequenceNumber - 1].terminalData.transAmount);
			printf("Sequence No. : %d\n", transData[transactionSequenceNumber - 1].transactionSequenceNumber);
			printf("Trans Date: %s\n", transData[transactionSequenceNumber - 1].terminalData.transactionDate);
			printf("Balance: %f\n", AccdataBase[client].balance);

			return SERVER_OK;
		}
	else
	{
		return SAVING_FAILED;
	}
}