#include"RGDReader.cpp"

using namespace std;

typedef unsigned char BYTE;

int main()
{

    FILE * fp;
    FILE * fq;

    string fileAddress;
    string fileName;
    string key;
    string msg;

    vector<BYTE> data();
    vector<BYTE> pad();

    RGBReader rgbReader;
    int mode;

    //For filename copy
    int posIndex;

    cout<<"enter the file address:"<<endl;
    cin>>fileAddress;

    //sizeof(FileAddress);

    fp = fopen(fileAddress.c_str(),"rb");//FileAddress.c_str is used to convert string to char*
    if(fp==NULL){
        printf("File Not Found");
        return -1;
    }

    posIndex = fileAddress.rfind("\\");
    if(posIndex == string::npos){
        fileName = fileAddress;
    }else{
        fileName = fileAddress.substr(posIndex);
    }

    
    rgbReader.ReadHeader(Header,fp);
    
    rgbReader.PrintHeader(Header);
    

    long h=Header.biHeight;
    long w=Header.biWidth;
    long o=Header.OffBits;
    long bit = Header.biBitCount;

    if(o==138){
        rgbReader.ReadHeaderBV5(HeaderV5,fp);
    }

    cout<<"Mode:? "<<endl;
    cin>>mode;
    if(mode == Mode::e){

        fileName = "Steg_"+fileName;
        fq = fopen(fileName.c_str(),"wb");
        rgbReader.WriteFileHeader(Header,fq);
        if(o==138){
            rgbReader.WriteFileHeaderBV5(HeaderV5,fp);
        }

        cout<<"Enter the message you want to hide"<<endl;
        cin>>msg;

        cout<<"Enter the encryption key"<<endl;
        cin>>key;
        
        rgbReader.GetPixels(h,w,o,bit,fp,fq,msg,key);
        cout<<"Successfully encoded";

    }else if(mode == Mode::d){
        cout<<"Enter the encryption key"<<endl;
        cin>>key;
        //Encrypt public key

        rgbReader.GetPixels(h,w,o,bit,fp,key);
       
    }
    cout<<"Press any key to close"<<endl;
    int x;
    cin>>x;


    fclose(fp);
    fclose(fq);
    return 0;
    
}


