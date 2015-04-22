#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ANTALKORTLEKAR 2
#define ANTALKORT (ANTALKORTLEKAR*52)

struct kort
{
    char farg[10];
    int kortnummer; /*varje korts "ID"*/

    int kortvarde;  /*kortetsvärde*/

    bool upptagen;
};
typedef struct kort Kort;


int randomfunktio(int minimum, int maximum)
{
    int ran = 0;
    do
    {
        ran = (rand() %maximum) + minimum;
    }while((ran > maximum && ran != maximum) || (ran < minimum && ran != minimum)); /*s� l�nge detta �r sant k�r loopen. N�r vilkoret st�mmer �r det ett tal som �r o�nskat, d�
                                                                                    //k�rs loopen igen och slumpar d� fram ett nytt tal. Detta tills den ger ett tal som �r �nskat*/
    return ran;
}

void huvudord()
{
    printf("Black Jack");
    printf("\t\t\t");
}

int meny()
{
    system("cls");
    int run = 1;
    int menyval = 0;
    while(run)
    {
        huvudord();
        char temp[] = "abc";
        printf("\n\n\n\n\n\n");

        printf("                            Hej och välkommen!\n\n");

        printf("                            1. Spela Black Jack\n");
        printf("                            2. Regler\n");
        printf("                            3. Avsluta\n");
        printf("                            Val: ");
        gets(temp);
        menyval = atoi(temp);

        if(menyval > 0 && menyval < 4)
        {
            run = 0;
        }
        else
        {

            printf("Fel inmatat värde, försök igen\n\n");

            system("pause");
        }
        system("cls");
    }
    return menyval;
}

void regler()
{
    system("cls");
    huvudord();

    printf("\n\n\nBlack Jack går ut på att man ska få 21 eller så nära 21 som möjligt.\nMan spelar mot banken (datorn).\nSpelet gör till så att man fort får 2 kort, summan av dessa kort läggs ihop och du har ett tal. Efter det får du välja genom att antingen ta ett till kort\neller stanna.\n\n");

    system("pause");
}

void ritaBord(int bankens_hand, int din_hand, int satsade_pengar, int pengar_kvar)
{
    system("cls");
    huvudord();
    /*
    printf("\n\t\t\t\t\tBANKEN\n");
    printf("\t\t\t\t\t  %d\n\n\n\n\n\n",bankens_hand);
    printf("\t\t\t\t     Din hand\n");
    printf("\t\t\t\t+---------------+\n");
    printf("\t\t\t\t|\t%d\t|\n",bankens_hand);
    printf("\t\t\t\t+---------------+\n");
    printf("Pengar kvar:\t%d $\n",pengar_kvar);
    printf("Satsade pengar:\t%d $\n", satsade_pengar);
    */

    printf("\n\n");
    printf("Pengar kvar:\t%d $\n",pengar_kvar);
    printf("Satsade pengar:\t%d $\n\n", satsade_pengar);
    printf("Bankens hand:\t%d\n", bankens_hand);
    printf("Din hand:\t%d\n",din_hand);

    printf("\n\n");




}
/*randomfunktio(1,52)
int gekort(Kort kortlek[])
{
    //int kortledig[52];
    kortlek[0].kortnummer = randomfunktio(1,ANTALKORT);
    return kortlek[0].kortnummer;
}*/

void zeroz(int arr[])
{
    for(int i = 0;i<ANTALKORT;i++)
    {
        arr[i] = 0;
    }
}

void blanda_kortleken(Kort kortlek[])
{
    char tempfarg[10];
    int tempnummer = 0;
    int temp_kortvarde = 0;
    bool tempupptagen = false;

    for(int i = 0;i<100000;i++)
    {
        int temp1 = randomfunktio(0,ANTALKORT);
        int temp2 = randomfunktio(0,ANTALKORT);

        strcpy(tempfarg, kortlek[temp1].farg);
        tempnummer = kortlek[temp1].kortnummer;
        tempupptagen = kortlek[temp1].upptagen;
        temp_kortvarde = kortlek[temp1].kortvarde;

        strcpy(kortlek[temp1].farg, kortlek[temp2].farg);
        kortlek[temp1].kortnummer = kortlek[temp2].kortnummer;
        kortlek[temp1].upptagen = kortlek[temp2].upptagen;
        kortlek[temp1].kortvarde = kortlek[temp2].kortvarde;

        strcpy(kortlek[temp2].farg, tempfarg);
        kortlek[temp2].kortnummer = tempnummer;
        kortlek[temp2].upptagen = tempupptagen;
        kortlek[temp2].kortvarde = temp_kortvarde;
    }
}

void initiera_kortleken(Kort kortleken[])
{
    int farg = 0;
    int temp_kortvarde = 1;
    for(int i = 0;i<ANTALKORT;i++)
    {
        kortleken[i].kortnummer = i;


        if(temp_kortvarde == 14)
        {
            temp_kortvarde = 1;
        }
        kortleken[i].kortvarde = temp_kortvarde;


        if(farg == 52)
        {
            farg = 0;
        }

        if(farg <= 13)
        {

            strcpy(kortleken[i].farg, "Hjärter");

            kortleken[i].farg[7] = '\0';
        }
        else if(farg > 13 && farg <= 26)
        {
            strcpy(kortleken[i].farg,"Spader");
            kortleken[i].farg[6] = '\0';
        }
        else if(farg > 26 && farg <= 39)
        {
            strcpy(kortleken[i].farg,"Ruter");
            kortleken[i].farg[5] = '\0';
        }
        else if(farg > 39 && farg <= 51)
        {

            strcpy(kortleken[i].farg,"Klöver");

            kortleken[i].farg[6] = '\0';
        }
        kortleken[i].upptagen = false;
        farg++;
        temp_kortvarde++;
    }
}

void checka_kort(int kortleksplats, Kort kortlek[])
{

    printf("Färg: \t\t%s (%d)\n", kortlek[kortleksplats].farg, kortleksplats);
    printf("Kortnummer: \t%d \n", kortlek[kortleksplats].kortnummer);
    printf("Kortvärde: \t%d \n", kortlek[kortleksplats].kortvarde);

    if(kortlek[kortleksplats].upptagen == true)
    {
        printf("Upptagen: \tTrue\n");
    }
    else
    printf("Upptagen: \tFalse\n");
}

int dra_kort(Kort kortlek[])
{

    int blackjackvarde = 0; /*alla klädda kort är värda 10 i blackjack*/

    for(int i = 0;0<ANTALKORT;i++)
    {
        if(kortlek[i].upptagen == false) /*hittar ett ledigt kort*/
        {
            kortlek[i].upptagen = true; /*kortet är nu draget*/

            blackjackvarde = kortlek[i].kortvarde;
            if(blackjackvarde > 10)
            {
                blackjackvarde = 10;
            }
            break;
        }
    }
    return blackjackvarde;
}

void blackjack()
{
    Kort kortlek[ANTALKORT];    /*strukten med alla kort*/
    initiera_kortleken(kortlek);/*initiera korten*/
    blanda_kortleken(kortlek);  /*blanda korten*/

    int spelares_pengar = 100;  /*spelarens start pot*/
    int bank = 0;               /*bankens hand*/
    int spelares_hand = 0;      /*spelarens hand*/
    int spelarens_satsade = 0;  /*spelarens satsade pengar per runda*/

    /*Start bordet*/
    ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);

    bool spela = true;
    int kort_kvar = ANTALKORT;

    char tmp[10];
    int temp = 0;

    bool spelare_forlorat = false;

    bool nyttkort = true; /*temporäre flagga för loopar*/

    bool spelare_dubbla = true;

    while(spela)
    {
        /*satsa pengar*/
        while(true)
        {
            printf("(Tryck \"0\" för att avsluta)\n");

            printf("Hur mycket vill du satsa: ");
            gets(tmp);
            temp = atoi(tmp);

            if(temp > spelares_pengar)
            {
                printf("Du kan inte satsa mer pengar än vad du har!\n\n");

                system("pause");
                ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);
            }
            else if(temp == 0)
            {
                spela = false;
                break;
            }
            else
            {
                break;
            }
        }
        if(spela == false)/*avslutar eftersom spelarren gett över 21 i kortvärde*/

        {
            break;
        }

        spelarens_satsade = temp + spelarens_satsade;
        spelares_pengar = spelares_pengar - temp;
        ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);

        /*ge spelare 2 kort*/

        int kort_varde_spelare_1 = dra_kort(kortlek);
        int kort_varde_spelare_2 = dra_kort(kortlek);

        spelares_hand = kort_varde_spelare_1 + kort_varde_spelare_2;

        /*spelares_hand = dra_kort(kortlek) + dra_kort(kortlek);*/

        /*ge huset 2 kort*/
        int kort_varde_dator_1 = dra_kort(kortlek);
        int kort_varde_dator_2 = dra_kort(kortlek);

        bank = kort_varde_dator_1;

        /*bank = dra_kort(kortlek) + dra_kort(kortlek);*/


        ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);

        /*se till så spelaren får så många kort denne vill ha*/


        while(nyttkort)
        {

            if(spelares_hand > 21)
            {

                /*spelaren har förlorat!*/

                spelare_forlorat = true;
                break;
            }
            else if(spelares_hand >= 7 && spelares_hand <= 11)
            {

                /*spelare får dubbla!*/

                printf("Nytt kort = 1, Stanna = 2, Dubbla = 3 \n\n");
                printf("Nu kan du dubbla!\n\n");
                printf("Ange val: ");
            }
            else
            {
                printf("Nytt kort = 1, stanna = 2\n\n");
                printf("Ange val: ");
            }


            gets(tmp);
            temp = atoi(tmp);


            if(temp == 1)
            {
                spelares_hand = spelares_hand + dra_kort(kortlek);
            }
            else if(temp == 2)
            {
                nyttkort = false;
            }
            else if(temp == 3)
            {
                spelares_pengar = (spelares_pengar - spelarens_satsade);
                spelarens_satsade = (spelarens_satsade*2);
                spelares_hand = spelares_hand + dra_kort(kortlek);
                nyttkort = false;
            }
            ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);
        }

        if(spelare_forlorat == true)
        {
            /*printf("Du har förlorat!!\n");*/
        }
        else
        {
            /*sedan får huset ta kort

            //nyttkort = true;*/
            bank = bank + kort_varde_dator_2;
            while(true)
            {
                if(bank < 17) /*nytt kort*/
                {
                    bank = bank + dra_kort(kortlek);
                    ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);
                }
                else if(bank >= 17)
                {
                    /*nyttkort = false;*/
                    break;
                }
            }
            /*ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);*/
        }
        /*regler, vem vann?*/
            printf("\n");
            ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);
            if(spelare_forlorat == true)
            {
                printf("Spelaren har förlorat!");

                spelarens_satsade = 0;
            }
            else if(bank > 22)
            {
                printf("Spelaren har vunnit!");
                spelares_pengar = spelares_pengar + (spelarens_satsade * 2);
            }
            else if(bank >= 22 && spelares_hand < 22)
            {
                printf("Spelaren har vunnit!");
                spelares_pengar = spelares_pengar + (spelarens_satsade * 2);
            }
            else if(spelares_hand > bank)
            {
                printf("Spelaren har vunnit!");
                spelares_pengar = spelares_pengar + (spelarens_satsade * 2);
            }
            else if(spelares_hand < bank && bank < 22)
            {
                printf("Spelaren har förlorat!");

                spelarens_satsade = 0;
            }
            else if(spelares_hand == bank)
            {
                printf("Det blev lika");
                spelares_pengar = spelares_pengar + spelarens_satsade;
            }


            /*Blanda om korten*/
            initiera_kortleken(kortlek);/*initiera korten*/
            blanda_kortleken(kortlek);  /*blanda korten*/

            /*IGEN!*/
            printf("\n\nSpela igen? (Ja = 1, Nej = 2): ");
            gets(tmp);
            temp = atoi(tmp);
            if(temp == 1)
            {
                spela = true; /*spelar igen*/
                spelare_forlorat = false;
                nyttkort = true;
                spelares_hand = 0;
                bank = 0;
                spelarens_satsade = 0;
                ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);
            }
            else
            {
                spela = false;
                ritaBord(bank,spelares_hand,spelarens_satsade,spelares_pengar);
            }
    }
    system("pause");
}


int main(void)
{
    srand(time(NULL));
    huvudord();


    int menyval = 0;

    while(menyval != 3)
    {
        menyval = meny();
        if(menyval == 1)
        {
            blackjack();
        }
        else if(menyval == 2)
        {
            regler();
        }

        else if(menyval == 3)
        {

        }
    }
    return 0;
}
