import java.io.*;
import java.util.Scanner;

/*********************************************************************
Data Flow in Enigma

1. Keypress
2. The Rotor Mechanism registers one step in keypress
3. Signal go through the plugboards
4. Signal goes through each of the five rotors in order 1-->5
5. Signal goes through the reflector
6. Signal goes through each of five rotors in the reverse order 5-->1
7. Singal goes through the plugboards again
8. Output
*********************************************************************/

class Rotor{
    private String data;
    private final static String alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    private int pos;

    Rotor(String a, int b){
        this.data = a;
        this.pos = b%26;
    }
    
    public int movePos(int delta){
        this.pos = (pos+delta)%26;
        return this.pos;
    }

    public void incPos(){
        this.pos = (pos+1)%26;
    }

    public int getPos(){
        return this.pos;
    }
    
    // individual rotor wiring for step 4 in Data Flow
    public char textIn(char text){
        int inpos, outpos, tempos; // inposition, outposition, and tempposition of the terminals
        char tempchar;
        
        // set tempos as the index of character 'text' in alphabet
        tempos = this.alphabet.indexOf(text);
        
        inpos = (tempos+this.pos)%26;
        tempchar = this.data.charAt(inpos);
        
        // set tempos as the index of character 'tempchar' in alphabet
        tempos = this.alphabet.indexOf(tempchar);
        
        outpos = (((tempos-this.pos)%26)+26)%26;
        return this.alphabet.charAt(outpos);
    }
    
    // individual rotor wiring for step 6 in Data Flow
    public char textOut(char text){
        int inpos, outpos, tempos;
        char tempchar;
        
        // set tempos as the index of character 'text' from alphabet
        tempos = this.alphabet.indexOf(text);

        inpos = (tempos+this.pos)%26;
        tempchar = this.alphabet.charAt(inpos);
        
        // set tempos as the index of character 'tempchar' in data of the rotor
        tempos = this.data.indexOf(tempchar);
        
        outpos = (((tempos-this.pos)%26)+26)%26;
        return alphabet.charAt(outpos);
    }
}


class Gear{
    private int size;
    private Rotor[] rot;

    // initiates the corkscrew action of the mechanical rotor
    private void updatePos(){
        for(int i = 0; i<size; i++){
            this.rot[i].incPos();
            if (this.rot[i].getPos() != 0){
                break;
            }
        }
    }
    
    // Five rotor Gear initialization
    Gear(Rotor a, Rotor b, Rotor c, Rotor d, Rotor e){
        this.size = 5;
        this.rot = new Rotor[this.size];
        this.rot[0] = a;
        this.rot[1] = b;
        this.rot[2] = c;
        this.rot[3] = d;
        this.rot[4] = e;
    }
    
    // Three rotor gear initialization
    Gear(Rotor a, Rotor b, Rotor c){
        this.size = 3;
        this.rot = new Rotor[this.size];
        this.rot[0] = a;
        this.rot[1] = b;
        this.rot[2] = c;
    }
    
    
    // text input to the rotor mechanism in step 4 of Data Flow
    public char In(char txt){
        char temp = txt;
        
        /* Step 2 updation procedure of rotor mechanism is done.
           Although this is supposed to happen at keypress, since
           this updation has no effect until the signal reaches the
           Rotors, an updation here won't make any difference in the 
           functioning of the machine
        */
        this.updatePos();
        
        for(int i = 0; i<5; i++){
            temp = this.rot[i].textIn(temp);
        }
        return temp;
    }

    // text input to the rotor mechanism in step 6 of Data Flow
    public char Out(char txt){
        char temp = txt;
        for(int i = 4; i>-1; i--){
            temp = this.rot[i].textOut(temp);
        }
        return temp;
    }
}


class PlugBoard{
    private String data1;
    private String data2;
    
    PlugBoard(String a, String b){
        this.data1 = a;
        this.data2 = b;
    }

    // text input to the plugboard as in step 3
    public char inputMap(char text){
    
        /* if text is in data1, return the corresponding character
           in data2 with the index that of text in data1. else return
           text itself
        */ 
        if (data1.contains(text+"")) // A quick conversion from char to char array
            return data2.charAt(data1.indexOf(text));
        else
            return text;
    }

    // text input to the plugboard as in step 7
    public char outputMap(char text){
    
        /* if text is in data2, return the corresponding character
           in data1 with the index that of text in data2. else return
           text itself
        */ 
        if (data2.contains(text+"")) // A quick conversion from char to char array
            return data1.charAt(data2.indexOf(text));
        else
            return text;
    }
    
    // Depreciated overloading of inputMap
    public String inputMap(String text){
        String temp = "";
        boolean flag = true;
        for (int j = 0; j<text.length(); j++){
            temp += this.inputMap(text.charAt(j));
        }
        return temp;
    }
    
    
    // Depreciated overloading of outputMap
    public String outputMap(String text){
        String temp = "";
        boolean flag = true;
        for (int j = 0; j<text.length(); j++){
            temp += this.outputMap(text.charAt(j));
        }
        return temp;
    }
}


class Reflector extends PlugBoard{
    
    Reflector(String a, String b){
        super(a,b);
    }
}

class Engine{
    private Reflector refl;
    private PlugBoard plug;
    private Gear rotMeter;
    
    Engine(Gear a, PlugBoard b, Reflector c){
        this.rotMeter = a;
        this.plug = b;
        this.refl = c;
    }
    
    // heart of the process
    public String encrypt(String text){
        String ciphertext = "";
        char temp;
        
        // text refinements
        text = text.replaceAll("[^a-zA-Z]","");
        text = text.toUpperCase();
        
        for(int i = 0; i<text.length(); i++){

            // Refer Data flow in Enigma
            temp = this.plug.inputMap(text.charAt(i)); // Plugboard, Step 3
            temp = this.rotMeter.In(temp); // Rotors, Step 4
            temp = this.refl.inputMap(temp); // Reflector, Step 5
            temp = this.rotMeter.Out(temp); // Rotors, Step 6
            temp = this.plug.inputMap(temp); // Plugboard, Step 7
            ciphertext += temp;
        }
        return ciphertext;
    }
}


public class Enigma{
    public static void main(String[] args){
    
        // initial configs for wiring of rotors, plugboard and reflector
        final String[] rotorData = {"ZLRVAKNPTYQCESIHFUWMBOGJDX",
                                     "CFKJRHPTMQNLOUVBZDXWESYGAI", 
                                     "TXNLQIHWBJERAMUPVDFZGOCKYS", 
                                     "RZCVYWFLGISQUAMOKDNBPXTHEJ", 
                                     "YVREQLZJSTXGMWIOKBNUHPDCAF"};
        final String plug1 = "AMRTHQXEGY";
        final String plug2 = "XRMQETAHYG";
        final String refl1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        final String refl2 = "LROGPMDKQNHAFJCEIBWZXYSUVT";
        
        Scanner sc = new Scanner(System.in);
        
        String text, ctext;
        int[] pos = new int[5];
        for(int i = 0; i<5; i++){
            int temp;
            System.out.print("Enter the initial position of rotor "+(i+1)+" :");
            pos[i] = sc.nextInt();
        }
        
        // Initializing components
        Rotor r1 = new Rotor(rotorData[0], pos[0]);
        Rotor r2 = new Rotor(rotorData[1], pos[1]);
        Rotor r3 = new Rotor(rotorData[2], pos[2]);
        Rotor r4 = new Rotor(rotorData[3], pos[3]);
        Rotor r5 = new Rotor(rotorData[4], pos[4]);
        Gear rotMeter = new Gear(r1, r2, r3, r4, r5);
        PlugBoard plug = new PlugBoard(plug1, plug2);
        Reflector refl = new Reflector(refl1, refl2);
        Engine engine = new Engine(rotMeter, plug, refl);
        
        sc.nextLine(); // Catch Enter from last input
        System.out.print("Enter the String to input:");
        text = sc.nextLine();
        System.out.println(text);
        ctext = engine.encrypt(text);
        System.out.println(ctext);
    }    
}
