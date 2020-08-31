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
        pos = pos+1;
        if (pos>=26){
            pos = 0;
            try{
                cout<<"before inc"<<endl;
                (this+1)->incPos();
                cout<<"after inc"<<endl;
            }
            catch(...){
                cout<<"exception caught"<<endl;
            }
        }
    }
    
    void setPos(int newpos){
        pos = newpos%26;
    }

    char textIn(char text){
        int inpos, outpos, tempos;
        char tempchar;
        for(int i = 0; i<index.length(); i++){
            if (text == index[i]){
                tempos = i;
                break;
            }
        }
        inpos = (tempos+pos)%26;
        tempchar = data[inpos];
        for(int i = 0; i<index.length(); i++){
            if (tempchar == index[i]){
                tempos = i;
                break;
            }
        }
        outpos = (((tempos-pos)%26)+26)%26;

        incPos();
        return index[outpos];
    }

    char textOut(char text){
        int inpos, outpos, tempos;
        char tempchar;
        for(int i = 0; i<index.length(); i++){
            if (text == index[i]){
                tempos = i;
                break;
            }
        }
        inpos = (tempos+pos)%26;
        tempchar = index[inpos];
        for(int i = 0; i<data.length(); i++){
            if (tempchar == data[i]){
                tempos = i;
                break;
            }
        }
        outpos = (((tempos-pos)%26)+26)%26;

        incPos();
        return index[outpos];
    }
};


class PlugBoard{
    string data1;
    string data2;
    public:
    PlugBoard(const string &a, const string &b) : data1(a), data2(b){}

    char inputMap(char text){
        for(int i = 0; i<data1.length(); i++){
            if (data1[i] == text)
            return data2[i];
        }
        return text;
    }

    char outputMap(char text){
        for(int i = 0; i<data2.length(); i++){
            if (data2[i] == text)
            return data1[i];
        }
        return text;
    }

    string inputMap(const string &text){
        string temp = "";
        bool flag = true;
        for (int j = 0; j<text.length(); j++){
            for(int i = 0; i<data1.length(); i++){
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
        for (int j = 0; j<text.length(); j++){
            for(int i = 0; i<data2.length(); i++){
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
    Reflector refl;
    PlugBoard plug;
    Rotor r1, r2, r3, r4, r5;

    string txtalone(const string &text){
        string temp = "";
        for(int i = 0; i<text.length(); i++){
            if (isalpha(text[i]))
                temp += text[i];
        }
        return temp;
    }
    string upper(string &text){
        string temp = "";
        for(int i = 0; i< text.length(); i++){
            temp += toupper(text[i]);
        }
        return temp;
    }

    public:
    Enigma(Rotor &a, Rotor &b, Rotor &c, Rotor &d, Rotor &e, PlugBoard &f, Reflector &g): r1(a), r2(b), r3(c), r4(d), r5(e), plug(f), refl(g){}

    string encrypt(string text){
        string cipher = "";
        char temp;
        text = txtalone(text);
        text = upper(text);
        for(int i = 0; i<text.length(); i++){
            temp = plug.inputMap(text[i]);
            temp = r1.textIn(temp);
            temp = r2.textIn(temp);
            temp = r3.textIn(temp);
            temp = r4.textIn(temp);
            temp = r5.textIn(temp);
            temp = refl.inputMap(temp);
            temp = r5.textOut(temp);
            temp = r4.textOut(temp);
            temp = r3.textOut(temp);
            temp = r2.textOut(temp);
            temp = r1.textOut(temp);
            temp = plug.inputMap(temp);
            cipher += temp;
        }
        return cipher;
    }
};


int main(){
    const string rotorData[5] = {"ZLRVAKNPTYQCESIHFUWMBOGJDX", "CFKJRHPTMQNLOUVBZDXWESYGAI", "TXNLQIHWBJERAMUPVDFZGOCKYS", "RZCVYWFLGISQUAMOKDNBPXTHEJ", "YVREQLZJSTXGMWIOKBNUHPDCAF"};
    const string plug1 = "AMRTHQXEGY";
    const string plug2 = "XRMQETAHYG";
    const string refl1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string refl2 = "LROGPMDKQNHAFJCEIBWZXYSUVT";
    string text, crypt;
    int pos[5];
    for(int i = 0; i<5; i++){
        cout<<"Enter the configuration of rotor "<<i<<" :";
        cin>>pos[i];
    }
    Rotor r1(rotorData[0], pos[0]), r2(rotorData[1], pos[1]), r3(rotorData[2], pos[2]), r4(rotorData[3], pos[3]), r5(rotorData[4], pos[4]);
    PlugBoard plug(plug1, plug2);
    Reflector refl(refl1, refl2);
    Enigma engine(r1, r2, r3, r4, r5, plug, refl);
    cout<<"Enter the string to input:";
    cin>>text;
    crypt = engine.encrypt(text);
    cout<<crypt;
    return 0;
}