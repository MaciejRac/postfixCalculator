// ONPcalculator.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//sources
//"Wprowadzenie do algorytmow" Autor: Cormen Thomas H., Leiserson Charles E., Rivest Ronald L
//wykład Algorithms and data structures Subject: Recursive structures array, list, stack, queue
// wikipedia Shunting yard algorithm https://en.wikipedia.org/wiki/Shunting_yard_algorithm




#include <iostream>
#include <string.h>
#include <cmath>

struct element
{
    int number;
    char sign;
    bool isOperator;
    struct element* next;
    struct element* prev;
};

void insert_first(int num, struct element** begin)
{
    struct element* temp = (struct element*)malloc(sizeof(struct element));
    if (temp == NULL) {
        return; //blad przypisania pamieci
    }
    temp->number = num;
    temp->sign = NULL;
    temp->isOperator = true;
    temp->next = NULL;
    temp->prev = NULL;
    *begin = temp;
}
void insert_first(char sign, struct element** begin)
{
    struct element* temp = (struct element*)malloc(sizeof(struct element));
    if (temp == NULL) {
        return;
    }
    temp->sign = sign;
    temp->number = NULL;
    temp->isOperator = false;
    temp->next = NULL;
    temp->prev = NULL;
    *begin = temp;
}
void push(int num, struct element** ptr)
{
    if (*ptr == NULL) {
        return insert_first(num, ptr);
    }
    else {
        struct element* p = (struct element*)malloc(sizeof(struct element));
        if (p == NULL) {
            return;
        }
        p->number = num;
        p->sign = NULL;
        p->isOperator = true;
        p->prev = *ptr;
        p->next = NULL;
        (*ptr)->next = p;//prev na next
        *ptr = p;
    }
}
void push(char sign, struct element** ptr)
{
    if (*ptr == NULL) {
        return insert_first(sign, ptr);
    }
    else {
        struct element* p = (struct element*)malloc(sizeof(struct element));
        if (p == NULL) {
            return;
        }
        p->sign = sign;
        p->number = NULL;
        p->isOperator = false;
        p->prev = *ptr;
        p->next = NULL;
        (*ptr)->next = p;//prev na next
        *ptr = p;
    }
}

int pop(struct element** begin)
{
    struct element* current = *begin;
    struct element* prev = NULL;
    struct element* next = NULL;
    int number;
    char sign;
    if (*begin == NULL) {
       // std::cout << "List empty" << std::endl;
        return 0;
    }
    if (current->isOperator == true) {
        if (current->prev == NULL && current->next == NULL) {
            number = current->number;
            current->next = NULL;
            current->prev = NULL;
            current->number = NULL;
            free(current);
            *begin = NULL;
            return number;
        }
        if (current->prev == NULL) {
            number = current->number;
            next = current->next;
            next->prev = NULL;
            free(current);
            *begin = next;
            return number;
        }
        else {
            number = current->number;
            prev = current->prev;
            prev->next = NULL;
            free(current);
            *begin = prev;
            return number;
        }
    }
    else if (current->isOperator == false) {
        if (current->prev == NULL && current->next == NULL) {
            sign = current->sign;
            current->next = NULL;
            current->prev = NULL;
            current->number = NULL;
            free(current);
            *begin = NULL;
            return sign;
        }
        if (current->prev == NULL) {
            sign = current->sign;
            next = current->next;
            next->prev = NULL;
            free(current);
            *begin = next;
            return sign;
        }
        else {
            sign = current->sign;
            prev = current->prev;
            prev->next = NULL;
            free(current);
            *begin = prev;
            return sign;
        }
    }
    else {
        std::cout << "blad zwiazany z isOperator" << std::endl;
        return NULL;
    }
}

void printFIFO(struct element** begin) {
    element* temp = *begin;
    element* last = temp;
    if (*begin == NULL) {
       // std::cout << "List empty" << std::endl;
        return;
    }
    while (temp != NULL) {
        //std::cout << temp->number << " ";
        temp = temp->prev;  //next
        if (temp != NULL) {
            last = temp;
        }
    }
    *begin = last;
    while (last != NULL) {
        if (last->isOperator == true) {
            std::cout << last->number << " ";
        }
        else if (last->isOperator == false) {
            if (last->sign == 'I')
                std::cout << "IF" << " ";
            else {
                std::cout << last->sign << " ";
            }
        }
        last = last->next;  //next
    }
    std::cout << std::endl;
}
void printStack(struct element** begin) {
    element* temp = *begin;
    if (*begin == NULL) {
        // std::cout << "List empty" << std::endl;
        return;
    }
    while (temp != NULL) {
        std::cout << temp->number << " ";
        temp = temp->prev;  //next
    }
    std::cout << std::endl;
}

int getElements(struct element** ptr) {
    element* temp = *ptr;
    int counter = 0;
    if (temp == NULL) {
       // std::cout << "List empty" << std::endl;
        return 0;
    }
    while (temp != NULL) {
        temp=temp->prev;
        counter++;
    }
    return counter;
} 


//tutaj koniec funkcji stosowych

char* getline(bool& endOfFile) {
    size_t lenmax = 100, count = 0;
    char* buffer = (char*)malloc(lenmax);
    int c;
    if (buffer == NULL)
        return NULL;
    endOfFile = false;
    while (true) {
        if (count >= lenmax) {
            lenmax += 100;
            char* newBuffer = (char*)realloc(buffer, lenmax);
            if (newBuffer == NULL) {
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }
        c = fgetc(stdin);
        if (c == EOF) {
            endOfFile = true;
            buffer[count] = '\0';
            //koniec wczytywania
            break;
        }
        if (c == '\n') {
           /* buffer[count] = '\0';
            break;*/
            continue;
        }
        if (c == '.') {
            buffer[count] = '\0';
            break;
        }
        buffer[count] = c;
        ++count;
    }
    return buffer;
}

bool isOperand(char token) {
    if (token == '+' || token == '-' || token == '/' || token == '*'|| token == '('|| token == ')'||token=='N'||token=='I'||token =='M'||token==',')
        return false;
    else
        return true;
}

int getPriority(char token) {
    if (token == '+' || token == '-') {
        return 1;
    }
    else if (token == '*' || token == '/')
        return 2;
    else if (token == 'I'|| token == 'N'|| token == '$' || token == '%')
        return 3;
    return 0;
}

struct element* ONPConv(char* expression) {
    element* stack = NULL;
    element* onp = NULL;
    int i = 0, priority = 0, end;
    char token, item, top;
    while (i != strlen(expression)) {
        token = expression[i];
        if (token == ' ') {
            i++;
            continue;
        }
        if (isOperand(token)) {
            int start = i;
            while (expression[i]!=' ') {
                end = i;
                i++;
            }
            int quantity = end - start;
            int value = 0;
            for (int j = quantity; j >=0; j--) {
                value += (expression[end - j]-'0') * pow(10, j);
            }
            push(value, &onp); //dodaje na finalny stos
        }
        else if (token == '(') {
            push(token, &stack);
        }
        else if (token == ')') {
            while (0 != getElements(&stack)) {
                token = pop(&stack);//token uzyskac
                if (token == '(') {
                    break;
                }
                else {
                    push(token, &onp);
                }
            }
        }
        else if (token == ',') {
            while (getElements(&stack) != 0) {
                top = pop(&stack);
                if (top == '(') {
                    push(top, &stack);
                    break;
                }
                push(top, &onp);
            }
        }
        else {
        if (token == 'I') {
            i++;
        }
        else if (token == 'M') {
            i++;
            if (expression[i] == 'A') {
                i++;
                token = '$';
            }
            else {
                i++;
                token = '%';
            }
        }
            priority = getPriority(token);
            while (getElements(&stack) != 0) {
                top = pop(&stack);
                if (top == '(' || getPriority(top) < priority|| getPriority(top) == priority && (top == 'N'||top=='I'||top=='$'||top=='%')) {
                    push(top, &stack);
                    break;
                }
                push(top, &onp);
            }
            push(token, &stack);
        }
        i++;
    }
    while (getElements(&stack) != 0) {
        item = pop(&stack);
        push(item, &onp);

    }
    return onp;
}

void evaluate(struct element** pointer) {
    element* stack = NULL;
    element* current = *pointer;
    int result = 0, operand1, operand2,operand3;
    while (current != NULL) {
        if (current->isOperator) {
            push(current->number, &stack);
        }
        else {
            if(current->sign=='I')
                std::cout << "IF" << " ";
            else {
                std::cout << current->sign << " ";
            }
            printStack(&stack);
            
            switch (current->sign) {
            case '+':
                operand2 = pop(&stack);
                operand1 = pop(&stack);
                result = operand1 + operand2;
                break;
            case '-':
                operand2 = pop(&stack);
                operand1 = pop(&stack);
                result = operand1 - operand2;
                break;
            case '*':
                operand2 = pop(&stack);
                operand1 = pop(&stack);
                result = operand1 * operand2;
                break;
            case '/':
                operand2 = pop(&stack);
                operand1 = pop(&stack);
                if (operand2 == 0) {
                    std::cout << "ERROR" << std::endl;
                    return ;  // błąd dzielenia przez zero
                }
                result = operand1 / operand2;
                break;
            case'N':
                operand1 = pop(&stack);
                result = -operand1;
                break;
            case 'I':
                operand1 = pop(&stack);
                operand2 = pop(&stack);
                operand3 = pop(&stack);
                if (operand3 > 0) {
                    result = operand2;
                }
                else {
                    result = operand1;
                }
                break;
            case '$':
                operand1 = pop(&stack);
                while (getElements(&stack) != 0) {
                    operand2 = pop(&stack);
                    if (operand1 < operand2) {
                        operand2 = operand1;
                    }
                }
                result = operand1;
                break;
            case '%':
                operand1 = pop(&stack);
                while (getElements(&stack) != 0) {
                    operand2 = pop(&stack);
                    if (operand1 > operand2) {
                        operand2 = operand1;
                    }
                }
                result = operand1;
                break;
            }
            push(result, &stack);
        }
        current = current->next;
    }
    result = pop(&stack);
    std::cout<<result<<std::endl;
}

int main()
{
    bool endOfFile = NULL;
    element* begin=NULL;//stos 
    char* wyraz;
    int liczba_rownan;
    std::cin >> liczba_rownan;
    for (int i = 0; i < liczba_rownan; i++) {
        wyraz = getline(endOfFile);
        begin = ONPConv(wyraz);
        free(wyraz);
        printFIFO(&begin);
        evaluate(&begin);
    }
    return 0;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
