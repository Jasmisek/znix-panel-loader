#include "main.h"
#define PROCESS XorStr("GFXTest64.exe").c_str() // set to process name you're injecting into

std::string getpass(const char* prompt, bool show_asterisk = true)
{
    const char BACKSPACE = 8;
    const char RETURN = 13;

    std::string password;
    unsigned char ch = 0;

    std::cout << prompt << " ";

    DWORD con_mode;
    DWORD dwRead;

    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hIn, &con_mode);
    SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

    while (ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN)
    {
        if (ch == BACKSPACE)
        {
            if (password.length() != 0)
            {
                if (show_asterisk)
                    std::cout << _xor_("");
                password.resize(password.length() - 1);
            }
        }
        else
        {
            password += ch;
            if (show_asterisk)
                std::cout << _xor_("*");
        }
    }
    std::cout << std::endl;

    return password;
}

int main()
{

    std::cout << _xor_("[loader] connecting...") << std::endl;
    Sleep(1000);
    system(_xor_("cls").c_str());
    std::cout << _xor_("[loader] connected") << std::endl;
    std::cout << _xor_("[loader] enter username - ");
    std::string username;
	std::cin >> username;
    std::cout << _xor_("[loader] enter password -");
    std::string password = getpass(_xor_("").c_str());
	std::string password_hash = Base64::Encode(password);
	authentication::login(username, password_hash);
	Sleep(-1);
}