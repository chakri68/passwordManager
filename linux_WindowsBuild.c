#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define clrBuffer() while ((getchar()) != '\n')
#ifdef _WIN32
#define clrscr() system("cls")
#else
#include <stdio.h>
#define clrscr() printf("\e[1;1H\e[2J")
#endif

void prompt(void);
void encrypt(char sen[], char key[]);
int random_number(int min_num, int max_num);
void decrypt(char sen[], char key[]);

int main(void) {
    char mRes = 'c';
    srand(time(NULL));
    while (mRes != 'e') {
        prompt();
        printf("\n\n\n\n$: ");
        scanf("%c", & mRes);
        clrBuffer();
        if (mRes == 'n') {
            int min = 33, max = 126, l = 100;
            char pwdRes, response;
            char pwd[l + 1], ch;
            clrscr();
            printf("Do you have the password (Y/n): ");
            scanf("%c", & pwdRes);
            clrBuffer();
            if (pwdRes == 'Y') {
                printf("Enter the password (< 40 characters): ");
                fgets(pwd, 40, stdin);
                strtok(pwd, "\n");
            } else {
                printf("Enter the length of the password you need (< 100): ");
                scanf("%d", & l);
                clrBuffer();
                for (int i = 0; i < l; i++) {
                    pwd[i] = random_number(min, max);
                }
                pwd[l] = '\0';
                printf("%s", pwd);
            }
            printf("\nDo you want to save the password? (Y/n): ");
            scanf("%s", & response);
            clrBuffer();
            if (response == 'Y') {
                char pname[21], fname[] = "pwdCiphered.txt";
                char encPwd[31], inpPwd[31];
                FILE * fileO, * fileR;
                fileO = fopen(fname, "a");
                fileR = fopen(fname, "r");
                if (fileO == NULL) {
                    printf("ERROR!!\n");
                    fclose(fileO);
                    fclose(fileR);
                    continue;
                }
                printf("Enter the name by which you want to save the password (< 40 characters): ");
                fgets(pname, 40, stdin);
                strtok(pname, "\n");
                ch = fgetc(fileR);
                rewind(fileR);
                if (ch == EOF) {
                    char impwd[51], pres[51], tempImpwd[51];
                    printf("No Master password has been set! Set it now (< 50): ");
                    scanf("%50s", & impwd);
                    clrBuffer();
                    strcpy(tempImpwd, impwd);
                    encrypt(tempImpwd, impwd);
                    fprintf(fileO, "%s", tempImpwd);
                    fprintf(fileO, "%c", '\n');
                    encrypt(pwd, impwd);
                    fprintf(fileO, "%s", pname);
                    fprintf(fileO, "%c", ' ');
                    fprintf(fileO, "%s", pwd);
                    fprintf(fileO, "%c", '\n');
                    clrscr();
                    printf("    ====== Saved Successfully! ======\n\n");
                } else {
                    fgets(encPwd, 50, fileR);
                    strtok(encPwd, "\n");
                    printf("Enter Your Master Password: ");
                    scanf("%30s", & inpPwd);
                    clrBuffer();
                    decrypt(encPwd, inpPwd);
                    if (strcmp(encPwd, inpPwd) == 0) {
                        printf(" === Login successful! ===\n");
                        encrypt(pwd, encPwd);
                        fprintf(fileO, "%s", pname);
                        fprintf(fileO, "%c", ' ');
                        fprintf(fileO, "%s", pwd);
                        fprintf(fileO, "%c", '\n');
                        clrscr();
                        printf("    ====== Saved Successfully! ======\n\n");
                    } else {
                        clrscr();
                        printf("**** Wrong Login!!! ****\n");
                        fclose(fileR);
                        continue;
                    }
                }
                fclose(fileO);
                fclose(fileR);
            } else clrscr();
        } else if (mRes == 's') {
            char fname[] = "pwdCiphered.txt";
            char ch, inpPwd[51], encPwd[100];
            FILE * fileR;
            fileR = fopen(fname, "r");
            clrscr();
            if (fgets(encPwd, 100, fileR) == NULL) {
                printf(" === You have no saved passwords! ===\n\n");
                continue;
            }
            strtok(encPwd, "\n");
            printf("Enter Your Master Password: ");
            scanf("%50s", & inpPwd);
            clrBuffer();
            decrypt(encPwd, inpPwd);
            if (strcmp(encPwd, inpPwd) == 0) {
                clrscr();
                printf(" === Login successful! ===\n");
                char line[100], res[100], * sptr, resName[21];
                int nameInd = 1, nameRes;
                while (fgets(line, 100, fileR) != NULL) {
                    sptr = strrchr(line, ' ');
                    memset(res, '\0', 100);
                    strncpy(res, line, sptr - line);
                    printf(" %d -- %s\n", nameInd++, res);
                }
                printf("Enter the corresponding number: ");
                scanf("%d", & nameRes);
                clrBuffer();
                rewind(fileR);
                memset(res, '\0', 100);
                fgets(line, 100, fileR);
                nameInd = 1;
                while (nameInd <= nameRes) {
                    fgets(line, 100, fileR);
                    nameInd++;
                }
                strtok(line, "\n");
                sptr = strrchr(line, ' ');
                strncpy(res, sptr + 1, strlen(line) - (sptr - line) - 1);
                memset(resName, '\0', 21);
                strncpy(resName, line, sptr - line);
                decrypt(res, inpPwd);
                clrscr();
                printf("\n\nSaved password for %s - %s\n\n", resName, res);
            } else {
                clrscr();
                printf("**** Wrong Login!!! ****\n");
                fclose(fileR);
                continue;
            }
            fclose(fileR);
        }
    }
    return 0;
}
int random_number(int min_num, int max_num) {
    int result = 0, low_num = 0, hi_num = 0;
    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num + 1; // include max_num in output
    } else {
        low_num = max_num + 1; // include max_num in output
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}
void encrypt(char sen[], char key[]) {
    int kl = strlen(key), j, sl = strlen(sen);
    for (int i = 0; i < sl; i++) {
        j = i % kl;
        if ((int) sen[i] + (int) key[j] > 126) {
            sen[i] = (int) sen[i] + (int) key[j] - (93 * (int)(((int) sen[i] + (int) key[j]) / 93));
        } else sen[i] = sen[i] + key[j];
        if (sen[i] < 33) {
            sen[i] += 93;
        }
    }
}
void decrypt(char sen[], char key[]) {
    int kl = strlen(key), j, sl = strlen(sen);
    for (int i = 0; i < sl; i++) {
        j = i % kl;
        if ((int) sen[i] - (int) key[j] < 33) {
            sen[i] = (int) sen[i] - (int) key[j] + (int)(93 * ceil((33 - (int) sen[i] + (int) key[j]) / 93.0));
        } else sen[i] = sen[i] - key[j];
        if (sen[i] > 126) {
            sen[i] -= 93;
        }
    }
}
void prompt(void) {
    printf("    n - Get and Store New Passwords\n    s - Get Stored Passwords\n    e - Exit");
}

