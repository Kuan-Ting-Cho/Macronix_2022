#include <termio.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

using namespace std;

int *convertToASCII(string s)
{
    int a;
    int *array = new int[s.length()];
    for (int i = 0; i < s.length(); i++)
    {
        a = s[i];
        array[i] = a;
    }
    return array;
}

int main(int argc, char *argv[])
{
    fstream test;
    string Command;
    string Function_Key1 = "l";
    string Function_Key2 = "n";
    string list;
    int Line = 0;
    while (1)
    {
        test.open("/home/potter/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/test.txt", ios::in | ios::out | ios::app);
        cin >> Command;
        if (Command == Function_Key1)
        {
            while (test.peek() != EOF)
            {
                getline(test, list);
                cout << Line + 1 << ". " << list << endl;
                Line++;
            }
            if (Line == 0) // if no command in test.txt
            {
                cout << "No any command." << endl;
            }
        }
        else if (Command == Function_Key2)
        {
            test.close();
            Line = 0; // must set 0 when test.txt was closed!
            test.open("/home/potter/I_want_to_fly-vel_plan_humanoid_new/Motor_Driver/src/test.txt", ios::out | ios::trunc);
        }
        else
        {
            test << Command;
            test << "\n";
        }
        test.close();
    }
}

/*int *ControlButton()
{
    int *array = new int[2];
    array[0] = scanKeyboard();
    array[1] = scanKeyboard();
    return array;
}

int main(int argc, char *argv[]) {


  while(1){
    int* arr=ControlButton();
    printf("\n%d\n",*arr);
    printf("%d\n",*(arr+1));
    delete[] arr;
  }

}*/