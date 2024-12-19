#include <iostream>
#include "Bank.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "BusinessAccount.h"
#include <limits>
#include <iomanip>
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

using namespace std;

void clearScreen() {
    system(CLEAR);
}

int main() {
    setlocale(LC_ALL, "rus");

    Bank bank;
    string id, password;
    
    if (bank.GetCustomer(0) == nullptr) {
        cout << "Система не содержит пользователей. Создайте первого пользователя." << endl;
        string name, id, password;
        cout << "Введите имя клиента: ";
        cin >> name;
        do {
            cout << "Введите ID клиента (ровно 9 чисел): ";
            cin >> id;
            if (!isValidId(id)) {
                cout << "Ошибка: ID должен состоять ровно из 9 цифр!" << endl;
            }
        } while (!isValidId(id));
        cout << "Введите пароль: ";
        cin >> password;
        bank.AddCustomer(name, id, password);

        cout << "Пользователь создан, войдите в систему" << endl;
    }

    cout << "Введите ID: ";
    cin >> id;
    cout << "Введите пароль: ";
    cin >> password;


    if (!bank.ValidateLogin(id, password)) {
        cout << "Неверный логин или пароль!" << endl;
        return 0;
    }
    cout << "Вход выполнен!" << endl;
    int choice;
    do {
        clearScreen();
        cout << "\n----- Меню -----" << endl;
        cout << "1. Добавить клиента" << endl;
        cout << "2. Добавить аккаунт" << endl;
        cout << "3. Показать клиентов" << endl;
        cout << "4. Показать счета" << endl;
        cout << "5. Удалить клиента" << endl;
        cout << "6. Удалить счет" << endl;
        cout << "7. Вывести общий баланс" << endl;
        cout << "8. Сортировать счета по балансу" << endl;
        cout << "9. Показать информацию об аккаунте" << endl;
        cout << "10. Внести деньги на счет" << endl;
        cout << "11. Снять деньги со счета" << endl;
        cout << "12. Начислить проценты на сберегательные счета" << endl;
        cout << "13. Перевести деньги между счетами" << endl;
        cout << "0. Выйти" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, id, password;
            cout << "Введите имя клиента: ";
            cin >> name;
            do {
                cout << "Введите ID клиента (ровно 9 чисел): ";
                cin >> id;
                if (!isValidId(id)) {
                    cout << "Ошибка: ID должен состоять ровно из 9 цифр!" << endl;
                }
            } while (!isValidId(id));
            cout << "Введите пароль: ";
            cin >> password;
            bank.AddCustomer(name, id, password);
            break;
        }
        case 2: {
            double initialBalance;
            int accountType;
            cout << "Введите начальный баланс для аккаунта: ";
            cin >> initialBalance;
            while (true) {
                cout << "Введите тип аккаунта (1-Сберегательный, 2-Расчетный, 3-Бизнес): ";
                cin >> accountType;

                if (accountType >= 1 && accountType <= 3) {
                    break;
                }
                else {
                    cout << "Такого варианта нет, попробуйте заново." << endl;
                }
            }

            if (accountType == 1) {
                double interestRate;
                cout << "Введите процентную ставку: ";
                cin >> interestRate;
                bank.AddAccount(make_shared<SavingsAccount>(initialBalance, interestRate));
            }
            else if (accountType == 2) {
                double overdraftLimit;
                cout << "Введите лимит овердрафта (максимальная сумма, на которую можно уходить в минус.): ";
                cin >> overdraftLimit;
                bank.AddAccount(make_shared<CheckingAccount>(initialBalance, overdraftLimit));
            }
            else if (accountType == 3) {
                bank.AddAccount(make_shared<BusinessAccount>(initialBalance));
            }
            break;
        }
        case 3: {
            bank.ShowCustomers();
            break;
        }
        case 4: {
            bank.ShowAccounts();
            break;
        }
        case 5: {
            int index;
            cout << "Введите индекс клиента для удаления: ";
            cin >> index;
            bank.DeleteCustomer(index);
            break;
        }
        case 6: {
            int index;
            cout << "Введите индекс счета для удаления: ";
            cin >> index;
            bank.DeleteAccount(index);
            break;
        }
        case 7: {
            cout << "------------------------------------------------" << endl;
            cout << "Всего средств в банке: " << fixed << setprecision(2) << bank.CalculateTotalAssets() << endl;
            cout << "------------------------------------------------" << endl;
            break;
        }
        case 8: {
            bank.SortAccountsByBalance();
            break;
        }
        case 9: {
            int index;
            cout << "Введите индекс аккаунта для просмотра информации: ";
            cin >> index;
            bank.ShowAccountInfo(index);
            break;
        }
        case 10: {
            int index;
            double amount;
            cout << "Введите индекс аккаунта для пополнения: ";
            cin >> index;
            cout << "Введите сумму для внесения: ";
            cin >> amount;
            auto account = bank.GetAccount(index);
            if (account) {
                account->Deposit(amount);
            }
            else {
                cout << "Аккаунт не найден!" << endl;
            }
            break;
        }
        case 11: {
            int index;
            double amount;
            cout << "Введите индекс аккаунта для снятия: ";
            cin >> index;
            cout << "Введите сумму для снятия: ";
            cin >> amount;
            bank.WithdrawFromAccount(index, amount);
            break;
        }
        case 12: {
            bank.ApplyInterestToAllSavings();
            break;
        }
        case 13: {
            int fromIndex, toIndex;
            double amount;
            cout << "Введите индекс счета, с которого нужно перевести деньги: ";
            cin >> fromIndex;
            cout << "Введите индекс счета, на который нужно перевести деньги: ";
            cin >> toIndex;
            cout << "Введите сумму для перевода: ";
            cin >> amount;
            bank.Transfer(fromIndex, toIndex, amount);
            break;
        }

        }
    } while (choice != 0);
    return 0;
}
