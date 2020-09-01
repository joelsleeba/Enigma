#include <iostream>
#include <string>
using namespace std;

class Rotor{
    string data;
    string index = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int pos;

    public:
    Rotor(string a, int b): data(a){
        pos = b%26;
    }
    
    int movePos(int delta){
        pos = (pos+delta)%26;
        return pos;
    }

    void incPos(){
        pos = (pos+1)%26;
    }

    int getPos(){
        return pos;
    }

    char textIn(char text){
        int inpos, outpos, tempos = 0;
        char tempchar;
        for(unsigned int i = 0; i<index.length(); i++){
            if (text == index[i]){
                tempos = i;
                break;
            }
        }
        inpos = (tempos+pos)%26;
        tempchar = data[inpos];
        for(unsigned int i = 0; i<index.length(); i++){
            if (tempchar == index[i]){
                tempos = i;
                break;
            }
        }
        outpos = (((tempos-pos)%26)+26)%26;
        return index[outpos];
    }

    char textOut(char text){
        int inpos, outpos, tempos = 0;
        char tempchar;
        for(unsigned int i = 0; i<index.length(); i++){
            if (text == index[i]){
                tempos = i;
                break;
            }
        }
        inpos = (tempos+pos)%26;
        tempchar = index[inpos];
        for(unsigned int i = 0; i<data.length(); i++){
            if (tempchar == data[i]){
                tempos = i;
                break;
            }
        }
        outpos = (((tempos-pos)%26)+26)%26;

        return index[outpos];
    }
};

class Gear{
    Rotor r1, r2, r3, r4, r5;
    Rotor rot[5] = {r1, r2, r3, r4, r5};

    void updatePos(){
        for(int i = 0; i<5; i++){
            rot[i].incPos();
            if (rot[i].getPos() != 0){
                break;
            }
        }
    }

    public:
 Gear(const Rotor &a, const Rotor &b, const Rotor &c, Rotor &d, Rotor &e): r1(a), r2(b), r3(c), r4(d), r5(e) {}

    char In(char txt){
        char temp = txt;
        for(int i = 0; i<5; i++){
            temp = rot[i].textIn(temp);
        }
        return temp;
    }

    char Out(char txt){
        char temp = txt;
        for(int i = 4; i>-1; i--){
            temp = rot[i].textOut(temp);
        }
        updatePos();
        return temp;
    }

};


class PlugBoard{
    string data1;
    string data2;
    public:
    PlugBoard(const string &a, const string &b) : data1(a), data2(b){}

    char inputMap(char text){
        for(unsigned int i = 0; i<data1.length(); i++){
            if (data1[i] == text)
            return data2[i];
        }
        return text;
    }

    char outputMap(char text){
        for(unsigned int i = 0; i<data2.length(); i++){
            if (data2[i] == text)
            return data1[i];
        }
        return text;
    }

    string inputMap(const string &text){
        string temp = "";
        bool flag = true;
        for (unsigned int j = 0; j<text.length(); j++){
            for(unsigned int i = 0; i<data1.length(); i++){
                if (data1[i] == text[j]){
                    temp += data2[i];
                    flag = false;
                    break;
                }
            }
            if (flag){
                temp += text[j];
            }
        }
        return temp;
    }

    string outputMap(const string &text){
        string temp = "";
        bool flag = true;
        for (unsigned int j = 0; j<text.length(); j++){
            for(unsigned int i = 0; i<data2.length(); i++){
                if (data2[i] == text[j]){
                    temp += data1[i];
                    flag = false;
                    break;
                }
            }
            if (flag){
                temp += text[j];
            }
        }
        return temp;
    }
};

class Reflector : public PlugBoard{

    public:
    Reflector(const string &a, const string &b): PlugBoard(a,b){}

};

class Enigma{

    Gear rotMeter;
    PlugBoard plug;
    Reflector refl;

    string txtalone(const string &text){
        string temp = "";
        for(unsigned int i = 0; i<text.length(); i++){
            if (isalpha(text[i]))
                temp += text[i];
        }
        return temp;
    }

    string upper(string &text){
        string temp = "";
        for(unsigned int i = 0; i< text.length(); i++){
            temp += toupper(text[i]);
        }
        return temp;
    }

    public:
    Enigma(Gear &a, const PlugBoard &b, const Reflector &c): rotMeter(a), plug(b), refl(c){}

    string encrypt(string text){
        string cipher = "";
        char temp;
        text = txtalone(text);
        text = upper(text);
        for(unsigned int i = 0; i<text.length(); i++){
            temp = plug.inputMap(text[i]);
            temp = rotMeter.In(temp);
            temp = refl.inputMap(temp);
            temp = rotMeter.Out(temp);
            temp = plug.inputMap(temp);
            cipher += temp;
        }
        return cipher;
    }
};


int main(){
    const string rotorData[5] = {"ZLRVAKNPTYQCESIHFUWMBOGJDX",
                                 "CFKJRHPTMQNLOUVBZDXWESYGAI", 
                                 "TXNLQIHWBJERAMUPVDFZGOCKYS", 
                                 "RZCVYWFLGISQUAMOKDNBPXTHEJ", 
                                 "YVREQLZJSTXGMWIOKBNUHPDCAF"};
    const string plug1 = "AMRTHQXEGY";
    const string plug2 = "XRMQETAHYG";
    const string refl1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string refl2 = "LROGPMDKQNHAFJCEIBWZXYSUVT";
    string text, crypt;
    int pos[5];
    
    cout<<"Welcome to the 5 rotorclassic german enigma"<<endl;
    cout<<"===========================================\n\n"<<endl;
    for(int i = 0; i<5; i++){
        cout<<"Enter the initial position of rotor "<<i+1<<" (a number between 1 and 26) :";
        cin>>pos[i];
    }
    Rotor r1(rotorData[0], pos[0]), r2(rotorData[1], pos[1]), r3(rotorData[2], pos[2]), r4(rotorData[3], pos[3]), r5(rotorData[4], pos[4]);
    Gear rotMeter(r1, r2, r3, r4, r5);
    PlugBoard plug(plug1, plug2);
    Reflector refl(refl1, refl2);
    Enigma engine(rotMeter, plug, refl);
    cout<<"Enter the string to encrypt:";
    cin.clear(); cin.ignore(1000,'\n');
    getline(cin, text);
    crypt = engine.encrypt(text);
    cout<<"\nEncrypted Text"<<endl;
    cout<<crypt<<endl;
    return 0;
}
