#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include <string>
#include<vector>

#include"EncodeAndDecode.cpp"
#include"Encryption.cpp"

void AddPad(int noOfPad, FILE *fp, FILE *fq);

using namespace std;

struct RGB{
    BYTE R;
    BYTE G;
    BYTE B;
};
struct BitMap{

    short Type;
    long Size;
    short Reserve1;
    short Reserve2;
    long OffBits;
    long biSize;
    long biWidth;
    long biHeight;
    short biPlanes;
    short biBitCount;
    long biCompression;
    long biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    long biClrUsed;
    long biClrImportant;
} Header;
struct BitMapV5{
    short bV5RedMask;
    short bV5GreenMask;
    short bV5BlueMask;
    short bV5AlphaMask;
    short bV5CSType;
    RGB   bV5Endpoints;
    short bV5GammaRed;
    short bV5GammaGreen;
    short bV5GammaBlue;
    short bV5Intent;
    short bV5ProfileData;
    short bV5ProfileSize;
    short bV5Reserved;
}HeaderV5;

/* struct StegBMP{
    string sSteg;
    string sPwd;
    long sLength;
}StegHeader; */

enum Mode{e, d};

EncodeAndDecode Encode;
Encryption Encrypt;

class RGBReader{
    const string STEG ="STEG";
    string checksum = "PASSWORD";
    int msgLength;

public:
    static vector<BYTE> data;
    static vector<BYTE> pad;
    
//FOR ENCODE
    void GetPixels(long h,long w,long offset,long bit,FILE * fp,FILE *fq,string msg,string key){

        string encChecksum = Encrypt.EncryptMsg(checksum,key);
        string Initial = STEG + encChecksum;
        string encMsg = Encrypt.EncryptMsg(msg,key);
        int noOfPad;
            
        //string msg = Encryption::GetEncryptedMsg();
        if(bit==24){
            noOfPad = 4 - ((w*3)%4);
            data.resize(w*3);
        }
        else if(bit == 32){
            data.resize(w*4);
            noOfPad  = 0;
        }
            
        pad.resize(noOfPad);
    
        BYTE c=0;

        //Go to the offset position  
        fseek(fp,offset,SEEK_SET);

        msgLength = msg.size();
        //Initial += to_string(msgLength);
        //Initial += endCheck;

        if(msg.size()*8 > ((data.size()*h)-(Initial.length() * 8))){
            cout<<"Message too long"<<endl;
            return;
        }

    //For ENCODE
        
        //To know if encoding is complete or not
        bool isComplete= false;
        bool isCompleteCheck = false;

        //TEST
        bool check1 = false, check2 =false;

        for(int i = 0; i < h; i++)
        {
            fread(&data[0], sizeof(BYTE), data.size(), fp);

            if(!isCompleteCheck){

                
                if(!check1 ){
                    //ENCODE HEADER
                    //make check1 true if complete
                    check1 = Encode.Encode(Initial,data);
                    fwrite(&data[0],sizeof(BYTE),data.size(),fq);
                    
                }else if(check1 ){
                    //ENCODE length
                    isCompleteCheck = Encode.Encode(msgLength,data);
                    fwrite(&data[0],sizeof(BYTE),data.size(),fq);
                    //make check2 true if complete
                }
         
            }
            else{
                
                if(!isComplete)
                    isComplete = Encode.Encode(encMsg,data);   // if msg is more than h, cannot fit, so
                                                            //making sure that it is less

                fwrite(&data[0],sizeof(BYTE),data.size(),fq);
                
                data.clear();
                if(bit==24)
                    data.resize(w*3);
                else if(bit == 32)
                    data.resize(w*4);

            }
            AddPad(noOfPad,fp,fq);
            
        }
     
    }
//FOR DECODE
    void GetPixels(long h, long w, long offset,long bit, FILE * fp, string key){
        int noOfPad = 4 - ((w*3)%4);

        bool ISCOMPLETECHECK = false;
        bool isComplete = false;


        bool check1 = false;
        bool allConditionMeet = false;
        
        if(bit==24)
            data.resize(w*3);
        else if(bit == 32)
            data.resize(w*4);

        pad.resize(noOfPad);
        string value = "";
        string encMsg = "";
        string msg = "";
        int _length = 12;

        BYTE c=0;

        fseek(fp,offset,SEEK_SET);

        
        for(int i=0;i<h;i++){
            fread(&data[0],sizeof(BYTE),data.size(),fp);
          
            if(!ISCOMPLETECHECK){
                if(!check1){
                    string temp =Encode.Decode(data,_length,check1);
                    value += temp;
                }else{
                    msgLength = Encode.Decode(data,ISCOMPLETECHECK);
                }    
            }
            else{
                if(STEG == value.substr(0,STEG.size())){
                    if(!allConditionMeet){
                        string _value = value.substr(4,checksum.size());
                        string _value_ = Encrypt.EncryptMsg(_value, key);
                        if(checksum == _value_){
                            allConditionMeet = true;
                        }else{
                            //PASSWORD INCORRECT
                            cout<<"PASSWORDD INCORRECT"<<endl;
                        }
                    }
                        
                }
                else{
                    //NOT STEG IMAGE
                    cout<<"NOT STEG IMAGE"<<endl;
                }              
            }
            if(allConditionMeet){
                    if(!isComplete){
                        encMsg += Encode.Decode(data, msgLength, isComplete);
                    }
                    else break;
            }
            data.clear();
            if(bit==24)
                data.resize(w*3);
            else if(bit == 32)
                data.resize(w*4);

            GetPad(noOfPad,fp);

            //cout<<"HERE 2 length  "<<length<<" "<<ftell(fp)<<endl;

        }
        msg = Encrypt.EncryptMsg(encMsg, key);
        cout << "Your msg is " << msg << endl;
    }
    void AddPad(int noOfPad, FILE *fp, FILE *fq){
        for(int j=0;j<noOfPad;j++)
            {
            //  pad[(i*h)+j] = getc(fp);
                BYTE c = getc(fp);
                fwrite(&c,sizeof(BYTE),1,fq);
            }
    }
    void GetPad(int noOfPad,FILE *fp){
        for(int j=0; j<noOfPad; j++)
            BYTE c=getc(fp);
    }
    void ReadHeader(BitMap& Header, FILE* fp){
        fread(&Header.Type, 2, 1, fp);//2 equivalent to sizeof(Header.Type)
        fread(&Header.Size, 4, 1, fp);
        fread(&Header.Reserve1, 2, 1, fp);
        fread(&Header.Reserve2, 2, 1, fp);
        fread(&Header.OffBits, 4, 1, fp);
        fread(&Header.biSize, 4, 1, fp);
        fread(&Header.biWidth, 4, 1, fp);
        fread(&Header.biHeight, 4, 1, fp);
        fread(&Header.biPlanes, 2, 1, fp);
        fread(&Header.biBitCount, 2, 1, fp);
        fread(&Header.biCompression, 4, 1, fp);
        fread(&Header.biSizeImage, 4, 1, fp);
        fread(&Header.biXPelsPerMeter, 4, 1, fp);
        fread(&Header.biYPelsPerMeter, 4, 1, fp);
        fread(&Header.biClrUsed, 4, 1, fp);
        fread(&Header.biClrImportant, 4, 1, fp);
    }
    void ReadHeaderBV5(BitMapV5& Header, FILE* fp){
        fread(&HeaderV5.bV5RedMask, sizeof(&HeaderV5.bV5RedMask) , 1, fp);//2 equivalent to sizeof(Header.Type)
        fread(&HeaderV5.bV5GreenMask, sizeof(&HeaderV5.bV5GreenMask) , 1, fp);
        fread(&HeaderV5.bV5BlueMask, sizeof(&HeaderV5.bV5BlueMask) , 1, fp);
        fread(&HeaderV5.bV5AlphaMask, sizeof(&HeaderV5.bV5AlphaMask) , 1, fp);
        fread(&HeaderV5.bV5CSType, sizeof(&HeaderV5.bV5CSType) , 1, fp);
        fread(&HeaderV5.bV5Endpoints, sizeof(&HeaderV5.bV5Endpoints) , 1, fp);
        fread(&HeaderV5.bV5GammaRed, sizeof(&HeaderV5.bV5GammaRed) , 1, fp);
        fread(&HeaderV5.bV5GammaGreen, sizeof(&HeaderV5.bV5GammaGreen) , 1, fp);
        fread(&HeaderV5.bV5GammaBlue, sizeof(&HeaderV5.bV5GammaBlue) , 1, fp);
        fread(&HeaderV5.bV5Intent, sizeof(&HeaderV5.bV5Intent) , 1, fp);
        fread(&HeaderV5.bV5ProfileData, sizeof(&HeaderV5.bV5ProfileData) , 1, fp);
        fread(&HeaderV5.bV5ProfileSize, sizeof(&HeaderV5.bV5ProfileSize) , 1, fp);
        fread(&HeaderV5.bV5Reserved, sizeof(&HeaderV5.bV5Reserved) , 1, fp);
       
    }
    void PrintHeader(BitMap& Header){
        
        printf("%d\n",Header.Type);
        printf("%ld\n",Header.Size);
        printf("%ld\n",Header.Reserve1);
        printf("%ld\n",Header.Reserve2);
        printf("%ld\n",Header.OffBits);
        printf("%ld\n",Header.biSize);
        printf("%ld\n",Header.biWidth);
        printf("%ld\n",Header.biHeight);
        printf("%ld\n",Header.biPlanes);
        printf("%ld\n",Header.biBitCount);
        printf("%ld\n",Header.biCompression);
        printf("%ld\n",Header.biSizeImage);
        printf("%ld\n",Header.biXPelsPerMeter);
        printf("%ld\n",Header.biYPelsPerMeter);
        printf("%ld\n",Header.biClrUsed);
        printf("%ld\n",Header.biClrImportant);
    }
    void WriteFileHeader(BitMap& Header, FILE* fq){
        fwrite(&Header.Type,sizeof Header.Type,1,fq);
        fwrite(&Header.Size,sizeof Header.Size,1,fq);
        fwrite(&Header.Reserve1,sizeof Header.Reserve1,1,fq);
        fwrite(&Header.Reserve2,sizeof Header.Reserve2,1,fq);
        fwrite(&Header.OffBits,sizeof Header.OffBits,1,fq);
        fwrite(&Header.biSize,sizeof Header.biSize,1,fq);
        fwrite(&Header.biWidth,sizeof Header.biWidth,1,fq);
        fwrite(&Header.biHeight,sizeof Header.biHeight,1,fq);
        fwrite(&Header.biPlanes,sizeof Header.biPlanes,1,fq);
        fwrite(&Header.biBitCount,sizeof Header.biBitCount,1,fq);
        fwrite(&Header.biCompression,sizeof Header.biCompression,1,fq);
        fwrite(&Header.biSizeImage,sizeof Header.biSizeImage,1,fq);
        fwrite(&Header.biXPelsPerMeter,sizeof Header.biXPelsPerMeter,1,fq);
        fwrite(&Header.biYPelsPerMeter,sizeof Header.biYPelsPerMeter,1,fq);
        fwrite(&Header.biClrUsed,sizeof Header.biClrUsed,1,fq);
        fwrite(&Header.biClrImportant,sizeof Header.biClrImportant,1,fq);
    }
    void WriteFileHeaderBV5(BitMapV5& Header, FILE* fp){
        fread(&HeaderV5.bV5RedMask, sizeof(&HeaderV5.bV5RedMask) , 1, fp);//2 equivalent to sizeof(Header.Type)
        fread(&HeaderV5.bV5GreenMask, sizeof(&HeaderV5.bV5GreenMask) , 1, fp);
        fread(&HeaderV5.bV5BlueMask, sizeof(&HeaderV5.bV5BlueMask) , 1, fp);
        fread(&HeaderV5.bV5AlphaMask, sizeof(&HeaderV5.bV5AlphaMask) , 1, fp);
        fread(&HeaderV5.bV5CSType, sizeof(&HeaderV5.bV5CSType) , 1, fp);
        fread(&HeaderV5.bV5Endpoints, sizeof(&HeaderV5.bV5Endpoints) , 1, fp);
        fread(&HeaderV5.bV5GammaRed, sizeof(&HeaderV5.bV5GammaRed) , 1, fp);
        fread(&HeaderV5.bV5GammaGreen, sizeof(&HeaderV5.bV5GammaGreen) , 1, fp);
        fread(&HeaderV5.bV5GammaBlue, sizeof(&HeaderV5.bV5GammaBlue) , 1, fp);
        fread(&HeaderV5.bV5Intent, sizeof(&HeaderV5.bV5Intent) , 1, fp);
        fread(&HeaderV5.bV5ProfileData, sizeof(&HeaderV5.bV5ProfileData) , 1, fp);
        fread(&HeaderV5.bV5ProfileSize, sizeof(&HeaderV5.bV5ProfileSize) , 1, fp);
        fread(&HeaderV5.bV5Reserved, sizeof(&HeaderV5.bV5Reserved) , 1, fp);
       
    }

};
vector<BYTE> RGBReader::data =  vector<BYTE>();
vector<BYTE> RGBReader::pad = vector<BYTE>();




