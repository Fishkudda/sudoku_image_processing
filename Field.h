#ifndef Field_h
#define Field_h

#ifndef Number
#include "Number.h"
#include <string>
#include <stdio.h>
#include "Jpeg.h"

#endif

using namespace std;
typedef unsigned char BYTE;

struct ByteIn{
    long pos;
    BYTE value;
};

struct SetFile{
    string name;
    vector <ByteIn> values;
};

class Field{
public:
    Field(char * path){
        ReadFile(path);
        
        long mark = getMarker(SOF0);
        
        setSOF(mark);
    }
    
    int checkPng(){
        if ((int)fileo.values[0].value != 137){
            return 0;
        }else if ((int)fileo.values[1].value != 80){
            return 0;
        }else if ((int)fileo.values[2].value != 78){
            return 0;
        }else if ((int)fileo.values[3].value != 71){
            return 0;
        }else if ((int)fileo.values[4].value != 13){
            return 0;
        }else if ((int)fileo.values[5].value != 10){
            return 0;
        }else if ((int)fileo.values[6].value != 26){
            return 0;
        }else if ((int)fileo.values[7].value != 10){
            return 0;
        }else{
            return 1;
        }
    }
    
    void consoleOut(){
        for(long i = 0; i < fileo.values.size();i++){
            BYTE *value = &fileo.values[i].value;
            
            if (letter_check(value)){
                cout << i << ": "<<*value;
                if (!letter_check(&fileo.values[i+1].value)){
                    cout << endl;
                }
            }else{
                cout << i << ": " << (int)*value << endl;
            }
        }
    }
    
    
    
    long getMarker(int marker){
        long start = 0;
        long end = 0;
        
        
        for(long i = 0; i < fileo.values.size();i++){
            BYTE *value = &fileo.values[i].value;
            
            if ( (start != 0) and ((int)*value == IDENT) ){
                end = i;
                break;
            }
            
            if ( ((int)*value == IDENT) and (int)fileo.values[i+1].value == marker){
                start = i;
            }
        }
        
        return start;
    }
    
    int letter_check(BYTE *value){
        int aCode = (int)*value;
        if (  (aCode >= 65 and aCode <=90) or ( aCode >=97 and aCode <=122) ){
            return 1;
        }else{
            return 0;
        }
    }
    
    
    void BuildFile(){
        FILE *file;
        file = fopen("/Users/sadrian/dev/DCPP/DCPP/file_copy.jpg","wb");
        for(int i = 0;i < fileo.values.size();i++){
            BYTE toWrite;
            toWrite = fileo.values[i].value;
            fwrite(&toWrite, sizeof(BYTE), 1, file);
        }
    }
    void sof_out(){
        cout << "Length: "<<(int)sof0.length[0] << " " << (int)sof0.length[1]<< endl;
        cout << "Image height: " << (int)sof0.image_height[0] << " " << (int)sof0.image_height[1] << endl;
        cout << "Image width: " << (int)sof0.image_width[0] << " " << (int)sof0.image_width[1] << endl;
        cout << "Number of components: " << (int)sof0.number_of_components[0] << endl;
        cout << "Component: " << (int)sof0.components[0] << " " << (int)sof0.components[1] << " " << (int)sof0.components[2] << endl;
        
    }
    
    void set_grey(){
        *sof0.number_of_components = 1;
    }
    
private:
    
    void setSOF(long mark){
        
        sof0.length = new BYTE[2];
        sof0.data_precision = new BYTE[1];
        sof0.image_height = new BYTE[2];
        sof0.image_width = new BYTE[2];
        sof0.number_of_components = new BYTE[1];
        sof0.components = new BYTE[3];
        
        sof0.length[0] = fileo.values[mark+2].value;
        sof0.length[1] = fileo.values[mark+3].value;
        sof0.data_precision[0] = fileo.values[mark+4].value;
        sof0.image_height[0] = fileo.values[mark+5].value;
        sof0.image_height[1] = fileo.values[mark+6].value;
        sof0.image_width[0] = fileo.values[mark+7].value;
        sof0.image_width[1] = fileo.values[mark+8].value;
        sof0.number_of_components[0] = fileo.values[mark+9].value;
        sof0.components[0] = fileo.values[mark+10].value;
        sof0.components[1] = fileo.values[mark+11].value;
        sof0.components[2] = fileo.values[mark+12].value;
    }
    
    void ReadFile(char *name){
        BYTE buffer;
        FILE *file;
        file = fopen(name,"rb");
        fileo.name = name;
        while(!feof(file)){
            fread(&buffer,sizeof(BYTE),1,file);
            ByteIn temp;
            temp.value = buffer;
            temp.pos = ftell(file);
            fileo.values.push_back(temp);
        }
        
        fclose(file);
    }
    SetFile fileo;
    SOF sof0;
};


#endif /* Field_h */
