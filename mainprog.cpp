#include"rgbreader.h"

#include<limits>

using namespace std;

typedef unsigned char BYTE;


int main()
{
    //BitMap Header;
    FILE * fp;
    FILE * fq;
    struct BitMap Header;

    string fileAddress;
    string fileName;
    string key;
    string msg;

    vector<BYTE> data();
    vector<BYTE> pad();

    RGBReader rgbReader;

    int mode;
    bool success = true;

    //For filename copy
    int posIndex;
    cout<<"Welcome to Image Steganography."<<endl;
    cout<<"Enter the file address of image you want to encode or decode: "<<endl;
    cin>>fileAddress;

    //sizeof(FileAddress);

    fp = fopen(fileAddress.c_str(),"rb");//FileAddress.c_str is used to convert string to char*
    if(fp==NULL){
        printf("File Not Found");
        system("Pause");
        return -1;
    }

    posIndex = fileAddress.rfind("\\");
    if(posIndex == string::npos){
        fileName = fileAddress;
    }else{
        fileName = fileAddress.substr(posIndex);
    }


   rgbReader.ReadHeader(Header,fp);

   // rgbReader.PrintHeader(Header);


    long h=Header.biHeight;
    long w=Header.biWidth;
    long o=Header.OffBits;

    cout<<"Enter 0 or 1\n0. Encode\n1. Decode "<<endl;
    cin>>mode;
    if(mode == Mode::e){

        fileName = "Steg_"+fileName;
        fq = fopen(fileName.c_str(),"wb");
        rgbReader.WriteFileHeader(Header,fq);


        cout<<"Enter the message you want to hide"<<endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin,msg);


        cout<<"Enter the encryption key"<<endl;
        getline(cin,key);
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        rgbReader.GetPixels(h,w,o,fp,fq,msg,key,success);



    }else if(mode == Mode::d){
        cout<<"Enter the encryption key"<<endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin,key);
        //Encrypt public key

        rgbReader.GetPixels(h,w,o,fp,key,success);


    }
    if(!success){

        if(posIndex != string::npos){

            remove ((fileAddress.substr(0,posIndex)+fileName).c_str());

        }
        else{
            fclose(fq);
            remove(fileName.c_str());
        }
    }
    system("Pause");


    fclose(fp);
    if(success)
        fclose(fq);

    return 0;

}


