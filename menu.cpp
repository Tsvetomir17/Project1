#include "menu.hpp"

Menu::Menu() : choice(-1), choiceAfterLogIn(0) {}

void Menu::setUsername(const char* username)
{
    strcpy(this->username, username);
}

int Menu::checkInput(int choice) const
{
    while(choice != 1 && choice != 2 && choice != 0)
    {
        std::cout << "Bad input! Try again: ";
        std::cin >> choice;
    }

    return choice;
}

int Menu::menuFirst()
{
    std::cout << std::endl << std::endl << "Hello! Choose one of the following" << std::endl;
    std::cout << "1. Register new user\n2. Log in as already created user\n\n0. Exit" << std::endl;

    std::cin >> choice;

    checkInput(choice);

    while(choice != 0 && choiceAfterLogIn == 0)
    {    
        while(choice == 1)
        {
            std::cout << std::endl << "Create accont" << std::endl << std::endl;
            User newUser;
            std::cin >> newUser;

            std::ofstream userDataBase("UserDataBase.txt", std::ios::app);
            if(!userDataBase.is_open())
            {
                std::cout << "Problem opening file" << std::endl;
                return 1;
            }

            userDataBase << newUser;

            userDataBase.close();

            std::cout << "1. Register new user\n2. Log in as already created user\n\n0. Exit" << std::endl;

            std::cin >> choice;

            checkInput(choice);
        }

        while(choice == 2)
        {
            std::cout << std::endl << "Login as" << std::endl << std::endl;

            char bufferUsername[MAX_LENGTH] = {'\0',}; 
            char bufferPassword[MAX_LENGTH] = {'\0',}; 
            std::cout << "Username: ";
            std::cin.ignore();
            std::cin.getline(bufferUsername, 65);

            std::cout << "Password: ";
            std::cin >> bufferPassword;

            std::ifstream userDataBase("UserDataBase.txt");
            if(!userDataBase.is_open())
            {
                std::cout << "Problem opening file" << std::endl;
                return 1;
            }

            char currentUsername[MAX_LENGTH] = {'\0',};
            char currentPassword[MAX_LENGTH] = {'\0',};
            char tempRow[MAX_TEMP_ROW] = {'\0',};
            int counterForSwitch = 1;

            bool flagIfThereIsSuchUser = false;

            while(!userDataBase.eof())
            {
                userDataBase >> tempRow;

                switch (counterForSwitch % 3)
                {
                    case 1: strcpy(currentUsername, tempRow); break;
                    case 2: strcpy(currentPassword, tempRow); break;
                    default:
                        if((strcmp(currentUsername, bufferUsername) == 0 && strcmp(currentPassword, bufferPassword) == 0))
                        {
                            flagIfThereIsSuchUser = true;
                        }
                        break;
                }

                counterForSwitch++;
                if(flagIfThereIsSuchUser) break;
            }

            userDataBase.close();

            if(!flagIfThereIsSuchUser)
            {
                std::cout << "Wrong username or password" << std::endl;
                std::cout << "1. Register new user\n2. Log in as already created user\n\n0. Exit" << std::endl;

                std::cin >> choice;

                checkInput(choice);
            }
            else
            {
                choiceAfterLogIn = -1;
                setUsername(bufferUsername);
                choice = -1;
            }
        }
    }

    return choiceAfterLogIn;
}

int Menu::menuSecond(int choiceAfterLogIn)
{
    while(choiceAfterLogIn != 0)
    {
        std::cout << std::endl << "Hello, " << username << "!" << std::endl;
        std::cout << "Choose one of the following: " << std::endl;
        std::cout << "1. Add trip\n2. Check destination\n\n0. Exit" << std::endl;

        std::cin >> choiceAfterLogIn;
        checkInput(choiceAfterLogIn);

        strcpy(fileName, username);
        strcat(fileName, ".db");
        fileName[sizeof(fileName)] = '\0';

        while(choiceAfterLogIn == 1)
        {
            Travel trip;
            std::cin >> trip;
            
            std::ofstream file(fileName, std::ios::app);
            if(!file.is_open())
            {
                std::cout << "Problem opening file" << std::endl;
                return 1;
            }

            file << trip;
            file.close();
            std::cout << std::endl << "1. Add trip\n2. Check destination\n\n0. Exit" << std::endl;

            std::cin >> choiceAfterLogIn;
            checkInput(choiceAfterLogIn);
        }

        while(choiceAfterLogIn == 2)
        {
            char bufferDestination[MAX_LENGTH_TRAVEL] = {'\0',};
            bool isVisited = false;
            float averageGrade = 0;
            int visits = 0;

            std::cout << "Which destination would you like to check: ";
            std::cin >> bufferDestination;
            std::cout << std::endl;

            std::ifstream file("UserDataBase.txt");

            if(!file.is_open())
            {
                std::cout << "Problem opening file" << std::endl;
                return 1;
            }

            char tempUser[MAX_LENGTH];
            char tempUserSave[MAX_LENGTH];
            char tempLine[MAX_LENGTH_TRAVEL2];
            
            do
            {
                file >> tempUser;
                if(strcmp(tempUserSave, tempUser) == 0) break;
                else strcpy(tempUserSave, tempUser);

                char* currentUserFileName = new char[strlen(tempUser) + 4];
                strcpy(currentUserFileName, tempUser);
                strcat(currentUserFileName, ".db");

                std::ifstream currentUserFile(currentUserFileName);
                if(!currentUserFile.is_open())
                {
                    std::cout << "Problem opening file!!!!!!!" << std::endl;
                    return 1;
                }

                char tempDestination[MAX_LENGTH_TRAVEL];
                char tempUserDestinationLine[MAX_LENGTH_TRAVEL2];
                int index = 0;

                while(!currentUserFile.eof())
                {
                    index = 0;
                    currentUserFile.getline(tempUserDestinationLine, MAX_LENGTH_TRAVEL2 - 1);

                    while(tempUserDestinationLine[index] != ' ')
                    {
                        tempDestination[index] = tempUserDestinationLine[index];
                        index++;
                    }
                    tempDestination[index] = '\0';

                    if(strcmp(tempDestination, bufferDestination) == 0)
                    {
                        isVisited = true;
                        averageGrade += getRating(tempUserDestinationLine);
                        visits++;
                        std::cout << tempUserDestinationLine << std::endl;
                    }
                }

                currentUserFile.close();

                delete[] currentUserFileName;

            } while (file.getline(tempLine, MAX_LENGTH_TRAVEL2 - 1));

            file.close();

            if(!isVisited)
            {
                std::cout << "No one has been there!" << std::endl;
            }
            else
            {
                std::cout << "Average rating: " << averageGrade / visits<< std::endl;
            }

            std::cout  << std::endl << "1. Add trip\n2. Check destination\n\n0. Exit" << std::endl;

            std::cin >> choiceAfterLogIn;
            checkInput(choiceAfterLogIn);
        }
    }

    return 0;
}

int Menu::getRating(const char* arr)
{
    int whitespaces = 0;
    int index = 0;
    while(whitespaces < 3)
    {
        if(arr[index] == ' ')
            whitespaces++;
        
        index++;
    }

    return arr[index] - '0';
}