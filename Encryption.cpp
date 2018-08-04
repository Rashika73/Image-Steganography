#include<string>

using namespace std;

static const string KEY = "a1233";

class Encryption{
    private:
    static string encMsg;
    static string encKey;
    public:
    static string EncryptMsg(string msg,string key){
        encMsg = msg;
        encKey = key;
        encKey = encryptDecrypt(key,KEY);
        // cout<<"Encrypted key = " << encKey<<" and length "<<encKey.size()<<" //end" <<endl;
        encMsg = encryptDecrypt(msg,encKey);
        // cout<<"Encrypted msg = " << encMsg<<" and length "<<encMsg.size()<<" //end" <<endl;
        //encMsg = encryptDecrypt(encMsg,encKey);
        // cout<<"Decrypted msg = "<< encMsg <<endl;
        return encMsg;
    }
/*     static string GetEncryptedMsg(){
        //cout<<encMsg<<endl;
        if(!encMsg.empty())
            return encMsg;
        return msg;
    } */
/*     static void SetMsg(string _m){
        //cout<<encMsg<<endl;
        msg = _m;
    } */

    private:
    static string encryptDecrypt(string msg, string key){
        string s = msg;
        int key_len = key.size(),i;

        for(i=0;i<msg.size();i++){
            s[i] = msg[i] ^ key[i% key_len];
        }
        return s;
    }
};
string Encryption::encMsg = "";
string Encryption::encKey = "";




