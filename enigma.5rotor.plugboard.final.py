def alphabetArray(): #forAlphabetArray
        alphabet=[]
        
        for i in range(97,123,1):
                alphabet.append(str.upper(chr(i)))

        return alphabet


def textAlone(rawText): #givesOutputWithoutWhitespacesAndNumbers
        rawTextCopy=""

        for i in range(0,len(rawText),1):
                if str.isalpha(rawText[i]):
                        rawTextCopy+=rawText[i]

        return rawTextCopy


def rotorIn(wiring,config,pos):
        alphabet=alphabetArray()
        rotorInPos=(config+pos)%26
        rotorOut=wiring[rotorInPos]
        rotorOutPos=(alphabet.index(rotorOut)-config)%26

        return rotorOutPos


def rotorOut(wiring,config,pos):
        alphabet=alphabetArray()
        rotorInPos=(config+pos)%26
        rotorOutLetter=alphabet[rotorInPos]
        rotorOutPos=(wiring.index(rotorOutLetter)-config)%26

        return rotorOutPos




if __name__=='__main__':
        scrambler1=['Z', 'L', 'R', 'V', 'A', 'K', 'N', 'P', 'T', 'Y', 'Q', 'C', 'E', 'S', 'I', 'H', 'F', 'U', 'W', 'M', 'B', 'O', 'G', 'J', 'D', 'X']
        scrambler2=['C', 'F', 'K', 'J', 'R', 'H', 'P', 'T', 'M', 'Q', 'N', 'L', 'O', 'U', 'V', 'B', 'Z', 'D', 'X', 'W', 'E', 'S', 'Y', 'G', 'A', 'I']
        scrambler3=['T', 'X', 'N', 'L', 'Q', 'I', 'H', 'W', 'B', 'J', 'E', 'R', 'A', 'M', 'U', 'P', 'V', 'D', 'F', 'Z', 'G', 'O', 'C', 'K', 'Y', 'S']
        scrambler4=['R', 'Z', 'C', 'V', 'Y', 'W', 'F', 'L', 'G', 'I', 'S', 'Q', 'U', 'A', 'M', 'O', 'K', 'D', 'N', 'B', 'P', 'X', 'T', 'H', 'E', 'J']
        scrambler5=['Y', 'V', 'R', 'E', 'Q', 'L', 'Z', 'J', 'S', 'T', 'X', 'G', 'M', 'W', 'I', 'O', 'K', 'B', 'N', 'U', 'H', 'P', 'D', 'C', 'A', 'F']
        reflector={'A': 'J', 'J': 'A', 'B': 'O', 'O': 'B', 'C': 'V', 'V': 'C', 'D': 'Q', 'Q': 'D', 'E': 'M', 'M': 'E', 'F': 'R', 'R': 'F', 'G': 'H', 'H': 'G', 'I': 'P', 'P': 'I', 'K': 'Z', 'Z': 'K', 'L': 'Y', 'Y': 'L', 'N': 'T', 'T': 'N', 'U': 'S', 'S': 'U', 'X': 'W', 'W': 'X'}
        plugboard={'M': 'J', 'J': 'M', 'A': 'K', 'K': 'A', 'E': 'P', 'P':'E', 'H': 'R', 'R': 'H', 'W': 'O', 'O': 'W', 'Q': 'U', 'U': 'Q', 'R': 'T', 'T': 'R', 'B': 'C', 'C': 'B', 'L': 'S', 'S':'L', 'I': 'F', 'F': 'I'}
        alphabet=alphabetArray()
        cipherText=""
        n=int(input("Enter the first rotor configuration:"))%26
        m=int(input("Enter the second rotor configuration:"))%26
        l=int(input("Enter the third rotor configuration:"))%26
        k=int(input("Enter the forth rotor configuration:"))%26
        j=int(input("Enter thr fifth rotor configuration:"))%26
        plainText=input("Enter the string to encrypt:")
        plainTextCopy=str.upper(textAlone(plainText))
        for i in range (0,len(plainTextCopy),1):

                if plainTextCopy[i] in plugboard:
                        plugboardOut=plugboard[plainTextCopy[i]]
                else:
                        plugboardOut=plainTextCopy[i]
                
                rotor1OutPos=rotorIn(scrambler1,n,alphabet.index(plugboardOut))
                rotor2OutPos=rotorIn(scrambler2,m,rotor1OutPos)
                rotor3OutPos=rotorIn(scrambler3,l,rotor2OutPos)
                rotor4OutPos=rotorIn(scrambler4,k,rotor3OutPos)
                rotor5OutPos=rotorIn(scrambler5,j,rotor4OutPos)

                reflectorIn=alphabet[rotor5OutPos]
                reflectorOut=reflector[reflectorIn]
                reflectorOutPos=alphabet.index(reflectorOut)
                #print(reflectorOutPos)

                rotor5OutPos=rotorOut(scrambler5,j,reflectorOutPos)
                rotor4OutPos=rotorOut(scrambler4,k,rotor5OutPos)
                rotor3OutPos=rotorOut(scrambler3,l,rotor4OutPos)
                rotor2OutPos=rotorOut(scrambler2,m,rotor3OutPos)
                rotor1OutPos=rotorOut(scrambler1,n,rotor2OutPos)

                if alphabet[rotor1OutPos] in plugboard:
                        plugboardOut=plugboard[alphabet[rotor1OutPos]]
                else:
                        plugboardOut=alphabet[rotor1OutPos]
                        
                cipherText+=plugboardOut

                n+=1
                if (n>25):
                        n=n%26
                        m+=1
                        if (m>25):
                                m=m%26
                                l+=1
                                if(l>25):
                                        l=l%26
                                        k+=1
                                        if(k>25):
                                                k=k%26
                                                j+=1
                                                if (j>25):
                                                        j=j%26


        print("EncryptedText:"+cipherText)
