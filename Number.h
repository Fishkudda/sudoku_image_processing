#ifndef Number_h
#define Number_h


typedef short unsigned int num;

class Number{
public:
    Number(){}
    
    Number(num x, num y){
        value = 0;
        fixed = 0;
    }
    
    Number(num x, num y, num value){
        value = 0;
        this->x = x;
        this->y = y;
        this->value = value;
    }
    num getValue(){
        return value;
    }
    void setValue(num x){
        value = x;
    }
private:
    num x;
    num y;
    num value;
    num fixed;
};


#endif /* Number_h */
