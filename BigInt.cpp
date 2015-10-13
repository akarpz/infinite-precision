
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;


class BigInt{
    public:
       BigInt operator+(int num);
       BigInt operator*(int num);
       BigInt operator-(int num);
       double operator/(int num);
       BigInt fDiv(int num);
       void operator<<(int num);
       
       BigInt operator+(BigInt num);
       BigInt operator*(BigInt num);
       BigInt operator-(BigInt num);
       double operator/(BigInt num);
       BigInt fDiv(BigInt num);
       void operator<<(BigInt num);
       void operator>>(BigInt num);
       
       int getValue();
       void print();
       void setValue(int num);
       
       // constructors
       BigInt(int num, int zeros);
       BigInt(long num);
       BigInt(string num);
       BigInt();
       vector<int> value;
       bool neg;
       
    friend ostream& operator<<(ostream& os, const BigInt& num){ //formats the BigInt & displays it as a regular number
        if(num.neg){ //prints a '-' if the BigInt is negative
            os << "-"; 
        }
        for(int i=num.value.size()-1 ; i >= 0 ; i--){               // loops through the value vector backwards
            if(num.value.at(i) == 0 && i != num.value.size()-1){ //print three zeros if there is a zero at the index & it isn't the biggest index
                os << 0;
                os << 0;
            }
            else if(num.value.at(i)>9 && num.value.at(i)<100 && i != num.value.size()-1){ //just print one zero if it's a two-digit number
                os << 0;
            }
            if(num.value.at(i) != 0 || i != num.value.size()-1){
                os << num.value.at(i);
            }
            else if(num.value.at(i) == 0 && i == num.value.size()-1){  //0 case
                os << num.value.at(i);
            }
        }
        return os;
    }
    friend istream& operator>> (istream &in, BigInt &num) { // works with the string constructor. takes input, returns a BigInt
        string newValue; 
        in >> newValue; // newValue can only be as big as the biggest string, but this is just an exercise
        num = BigInt(newValue); // construct a new bigint of the desired size at &num
        return in;
    }
    
};

// Constructor
BigInt::BigInt(){
    value.resize(1); // size 1 is default
    return;
}

// Overloaded constructor. create new vector which holds 3-digit segments of an integer in order, smallest to largest magnitude

BigInt::BigInt(int num, int zeros){ // "zeros" indicates number of 3-digit groups in BigInt (typically separated by commas if written out; compare to sci. notation)
    
    if(num < 0){ // Sets the neg value and makes the number positive
        neg = true;
        num*=-1;
    }
    else{
        neg = false;
    }
    
    
    for(int i=0 ; i < zeros ; i++){ //initializes the value vector so that there is one 0 at every 3-digit segment
        value.push_back(0);
    }
    
    int nextThree = num % 1000; //nextThree is the penultimate index (if num >= 1000; final index if else); a group of up to three digits
    value.push_back(nextThree);
    
    if (num > 999){                     // make sure nextThree is penultimate - if not, it's final index
        int firstThree = num / 1000;
        value.push_back(firstThree);
    }
    
    return;
}

BigInt::BigInt(long num){
    int nextThree; //used to build the BigInt
    int addOne = 0; //accounts for rounding up
    int digits = 0; //Amount of digits in the number
    
    if(num == 0){
        value.resize(1);
        value.at(0) = 0;
        neg = false;
        return;
    }
    
    if(num < 0){ // Sets the neg value and makes the number positive
        neg = true;
        num*=-1;
    }
    else{
        neg = false;
    }
    
    long newNum = num; //makes a copy of the number
    
    while (newNum > 0) { //Calculates amount of digits is in the number
        newNum /= 10;
        digits++;
    }
    if(digits % 3 !=0){ //Acounts for rounding up so the proper amount of rounds are used to build the BigInt
        addOne = 1;
    }
    newNum = num; //Reset the original number
    
    nextThree = newNum % 1000; //Grabs the last three numbers in the big num
    for(int i=0 ; i < digits/3 + addOne ; i++){
        value.push_back(nextThree); //adds them to the BigInt
        newNum = newNum / 1000; //Chop off the last three numbers with floor division
        nextThree = newNum % 1000; //set the new last three digits
    }
    
    return;
}

BigInt::BigInt(string num){ 
    string piece;
    int nextThree;
    
    char firstChar = num.at(0);
    if(firstChar == '-'){ // Sets the neg value and makes the number positive
        neg = true;
        num.erase(0,1);
    }
    else{
        neg = false;
    }
    
    int length = num.length();              //format the string to a BigInt
    for(int i=length-1 ; i>=0 ; i-=3){
        if(i>=2){
            piece = num.substr(i-2,3);
            istringstream (piece) >> nextThree;
            value.push_back(nextThree);
        }
        else if(i == 1){
            piece = num.substr(i-1,2);
            istringstream (piece) >> nextThree;
            value.push_back(nextThree);
        }
        else{
            piece = num.substr(i,1);
            istringstream (piece) >> nextThree;
            value.push_back(nextThree);
        }
    }
    
    return;
}

/////////////////////////////////////////// +

BigInt BigInt::operator+(BigInt num){
    BigInt bigIntTotal;    //call default constructor. we are going to build a new BigInt and return it
                                        // note that this function returns a new BigInt - the two objects that are passed into the function are not returned
    int selfSize = value.size();
    int otherSize = num.value.size();
    bool bigger = true; //true if self has more indices than other, or the two are equal in magnitude (have same # of indices)
    bigIntTotal.neg = false;
    
    //1. depending on negative or positive parameters value & num.value, decide how to interact with them
    
    if(neg != num.neg){
        if(neg){
            neg = false;
            bigIntTotal = num - *this;
            neg = true;
            return bigIntTotal;
        }
        else{
            num.neg = false;
            bigIntTotal = *this - num;
            num.neg = true;
            return bigIntTotal;
        }
    }
    else if(neg && num.neg){
        bigIntTotal.neg = true;
    }
    
    //2. decide if self is bigger than other, or vice versa. this will determine size of total vector
    
    if(selfSize < otherSize){ 
        bigger = false;
        bigIntTotal.value.resize(selfSize);
    }
    else{
        bigIntTotal.value.resize(otherSize);
    }
    
    //3. addition
    
    if(bigger){
        for(int i=0 ; i < otherSize ; i++){
            bigIntTotal.value.at(i) = value.at(i) +  num.value.at(i);
        }
        for(int i=0 ; i < selfSize-otherSize ; i++){
            bigIntTotal.value.push_back(value.at(i+otherSize));
        }
    }
    else{
        for(int i=0 ; i < selfSize ; i++){
            bigIntTotal.value.at(i) = value.at(i) +  num.value.at(i);
        }
        for(int i=0 ; i < otherSize-selfSize ; i++){
            bigIntTotal.value.push_back(num.value.at(i+selfSize));
        }
    }
    
    //4.
    //numbers at some indices will be >=1000, aka more than three digits, so:
    
    //If a number is over 1000, this creates the carry over and makes the next value in the vector one larger
    for(int i=0 ; i < bigIntTotal.value.size() ; i++){
            if(bigIntTotal.value.at(i) > 999){
                if(i == bigIntTotal.value.size() - 1){
                    bigIntTotal.value.push_back(1);
                }
                else{
                    bigIntTotal.value.at(i+1) ++;
                }
                bigIntTotal.value.at(i) = bigIntTotal.value.at(i) % 1000;
            }
        }
    
    return bigIntTotal;
}

BigInt BigInt::operator+(int num){
    BigInt bigIntTotal;
    BigInt newNum = BigInt(num);
    bigIntTotal = *this + newNum;
    return bigIntTotal;
}

////////////////////////////////////////////////////// -

BigInt BigInt::operator-(BigInt num) {

    BigInt bigIntTotal;    //call default constructor. we are going to build a new BigInt and return it
                                        // note that this function returns a new BigInt - the two objects that are passed into the function are not returned
    int selfSize = value.size();
    int otherSize = num.value.size();
    bool bigger = true; //true if self has more indices than other, or the two are equal in magnitude (have same # of indices)
    
    if(neg != num.neg){
        if(neg){
            num.neg = true;
            bigIntTotal = *this + num;
            num.neg = false;
            return bigIntTotal;
        }
        else{
            num.neg = false;
            bigIntTotal = *this + num;
            num.neg = true;
            return bigIntTotal;
        }
    }
    
    //1. determine which number is bigger. this will determine size of total vector
    
    if(selfSize < otherSize){
        bigger = false;
        bigIntTotal.value.resize(selfSize);
    }
    else{
        bigIntTotal.value.resize(otherSize);
    }
    
    //2. negativity operation: decide how the two numbers will interact 

    if(bigger){
        if(selfSize == otherSize){ //Sets the neg value according to which number is bigger
            for(int i=selfSize-1 ; i>=0 ; i--){
                if(value.at(i) > num.value.at(i) || (i==0 && value.at(i)==num.value.at(i))){ //sets neg to false for the 0 case and positive cases
                    bigIntTotal.neg = false;
                    break;
                }
                else if(value.at(i) < num.value.at(i)){ // Sets neg to true for negative cases
                    bigIntTotal.neg = true;
                    bigger = false;
                    break;
                }
            }
        }
        else{
            bigIntTotal.neg = false;//SelfSize > OtherSize
        }
    }
    
    //3. subtraction
    
    if(bigger){   // selfSize>otherSize 
        for(int i=0 ; i < otherSize ; i++){
            bigIntTotal.value.at(i) = value.at(i) -  num.value.at(i);
        }
        for(int i=0 ; i < selfSize-otherSize ; i++){
            bigIntTotal.value.push_back(value.at(i+otherSize));
        }
    }
    else{
        bigIntTotal.neg = true; // otherSize > selfSize
        for(int i=0 ; i < selfSize ; i++){
            bigIntTotal.value.at(i) = num.value.at(i) -  value.at(i);
        }
        for(int i=0 ; i < otherSize-selfSize ; i++){
            bigIntTotal.value.push_back(num.value.at(i+selfSize));
        }
    }
    
    //If a number is below 0, this creates the carry over and makes the next value in the vector one smaller
    for(int i=0 ; i < bigIntTotal.value.size() ; i++){
        if(bigIntTotal.value.at(i) < 0 ){
            bigIntTotal.value.at(i+1) --;
            bigIntTotal.value.at(i) = bigIntTotal.value.at(i) + 1000;
        }
    }
    
    int lastIndex = bigIntTotal.value.size() - 1;
    while(bigIntTotal.value.at(lastIndex) == 0 && bigIntTotal.value.size()!=1){ //If the highest place in the value vector is 0, erase it
        bigIntTotal.value.erase(bigIntTotal.value.begin() + lastIndex);
        lastIndex = bigIntTotal.value.size() - 1;
    }
    return bigIntTotal;
}

BigInt BigInt::operator-(int num){
    BigInt bigIntTotal;
    BigInt newNum = BigInt(num);
    bigIntTotal = *this - newNum;
    return bigIntTotal;
}

////////////////////////////////////////////// *

BigInt BigInt::operator*(BigInt num){
    BigInt bigIntTotal;    //call default constructor. we are going to build a new BigInt and return it
                           // note that this function returns a new BigInt - the object that is passed into the function are not returned
    int selfSize = value.size();
    int otherSize = num.value.size();
    vector<BigInt> temp; //This vector will hold BigInt values that will be added together in the multiplication process
    
    bigIntTotal.value.resize(1); //start just with 1 index and keep on adding on
    
    //If either input is zero, the result is zero
    if(num.value.at(num.value.size()-1) == 0 || value.at(value.size()-1) == 0){
        bigIntTotal.value.at(0) = 0;
        bigIntTotal.neg = false;
        return bigIntTotal;
    }
    
    for(int i=0 ; i<selfSize ; i++){
        for(int j=0 ; j<otherSize ; j++){
            temp.push_back(BigInt(value.at(i)*num.value.at(j),i+j)); //multiply each of the 3 digit values in the two BigInts accounting for the place values
        }
    }

    bigIntTotal.value.at(0) = 0; //start with 0
    for(int i=0 ; i < temp.size() ; i++){
        bigIntTotal = temp.at(i) + bigIntTotal; //add every value created in the step above
    }
    
    if(neg != num.neg){ //This is basically an exclusive Or. If one is negative and the other isnt, the answer is negative
        bigIntTotal.neg = true;
    }
    else{
        bigIntTotal.neg = false;
    }
    
    return bigIntTotal;
}

BigInt BigInt::operator*(int num){
    BigInt bigIntTotal;
    BigInt newNum = BigInt(num);
    bigIntTotal = *this * newNum;
    return bigIntTotal;
}
///////////////////////////////////////////// Regular Division

double BigInt::operator/(BigInt num){
    double bigIntTotal;    //this is a double!!!
                           // note that this function returns a new BigInt - the object that is passed into the function are not returned
    int selfSize = value.size();
    int otherSize = num.value.size();
    vector<double> temp; //This vector will hold BigInt values that will be added together in the division process
    double newNum = 0;
    
    for(int i=0; i < num.value.size() ; i++){
        newNum += num.value.at(i) * pow(10,i*3);
    }
    //If either input is zero, the result is zero
    if(num.value.at(num.value.size()-1) == 0){
        cout << "Divide by 0 error!" << endl;
        bigIntTotal = 0;
        return bigIntTotal;
    }
    else if(value.at(value.size()-1) == 0){
        bigIntTotal = 0;
        return bigIntTotal;
    }
    temp.resize(0);
    for(int i=0 ; i<selfSize ; i++){
        temp.push_back((double)value.at(i)*(double)pow(10,i*3) / newNum); //multiply each of the 3 digit values in the two BigInts accounting for the place values
    }
    bigIntTotal=0;
    for(int i=0 ; i < temp.size() ; i++){
        bigIntTotal = temp.at(i) + bigIntTotal; //add every value created in the step above
    }
    
    if(neg != num.neg){ //This is basically an exclusive Or. If one is negative and the other isnt, the answer is negative
        bigIntTotal *= -1;
    }
    else{
        bigIntTotal *= 1; //do nothing, it's pos
    }
    
    return bigIntTotal;
}

double BigInt::operator/(int num){
    double bigIntTotal;
    BigInt newNum = BigInt(num);
    bigIntTotal = *this / newNum;
    return bigIntTotal;
}

/////////////////////////////////////// Floor Division

BigInt BigInt::fDiv(BigInt num){
    BigInt bigIntTotal;
    double ans = *this / num;
    int iAns = ans/1;
    bigIntTotal = BigInt(iAns);
    return bigIntTotal;
}

BigInt BigInt::fDiv(int num){
    BigInt bigIntTotal;
    double ans = *this / num;
    int iAns = ans/1;
    bigIntTotal = BigInt(iAns);
    return bigIntTotal;
}

/////////////////////////////////////// Matrix Multiplication

vector< vector<BigInt> > matMult(vector< vector<BigInt> > num1 , vector<vector <BigInt> > num2){
    vector< vector<BigInt> > ans;
    int row1 = num1.size();
    int row2 = num2.size();
    int col1 = num1.at(0).size();
    int col2 = num2.at(0).size();
    
    ans.resize(row1);
    for(int i=0 ; i<ans.size() ; i++){
        ans.at(i).resize(row1);
    }
    
    //Set all values in the matrix to 0
    for(int row=0 ; row<row1 ; row++){
        for(int col=0 ; col<row1 ; col++){
            ans.at(row).at(col) = 0;
        }
    }
    
    if(col1 != row2){
        cout << "Check Matrix Dimensions! Multiplication Invalid!" << endl;
        return ans;
    }
    
    //add on to each value in the matrix appropriatly
    for(int i=0 ; i<row1 ; i++){
        for(int j=0 ; j<col2 ; j++){
            for(int k=0 ; k<row2 ; k++){
                ans.at(i).at(j) = num1.at(i).at(k) * num2.at(k).at(j) + ans.at(i).at(j);
            }
        }
    }
    return ans;
}

vector< vector<BigInt> > matMult(vector<BigInt> num1 , vector<vector <BigInt> > num2){
    vector< vector<BigInt> > ans;
    vector< vector<BigInt> > wrapper;
    wrapper.resize(1);
    wrapper.at(0).resize(num1.size());
    wrapper.at(0) = num1;
    ans = matMult(wrapper,num2);
    return ans;
}

/////////////////////////////////// Matrix Addition

vector< vector<BigInt> > matAdd(vector< vector<BigInt> > num1 , vector<vector <BigInt> > num2){
    vector< vector<BigInt> > ans;
    int row1 = num1.size();
    int row2 = num2.size();
    int col1 = num1.at(0).size();
    int col2 = num2.at(0).size();
    
    ans.resize(row1); //Size the answer
    for(int i=0 ; i<ans.size() ; i++){
        ans.at(i).resize(row1);
    }
    
    //Set all values in the matrix to 0
    for(int row=0 ; row<row1 ; row++){
        for(int col=0 ; col<row1 ; col++){
            ans.at(row).at(col) = 0;
        }
    }
    
    if(row1 != row2 || col1 !=col2){ //Makes sure the addition is possible
        cout << "Matricies must be the same dimension!" << endl;
        return ans;
    }
    
    for(int i=0; i< row1 ; i++){
        for(int j=0 ; j<col1 ; j++){
            ans.at(i).at(j) = num1.at(i).at(j) + num2.at(i).at(j);
        }
    }
    return ans;
}

vector< vector<BigInt> > matAdd(vector<BigInt> num1 , vector<vector <BigInt> > num2){
    vector< vector<BigInt> > ans;
    vector< vector<BigInt> > wrapper;
    wrapper.resize(1);
    wrapper.at(0).resize(num1.size());
    wrapper.at(0) = num1;
    ans = matAdd(wrapper,num2);
    return ans;
}


int main(){
    
    BigInt test  = BigInt(1);
    
    BigInt test2 = BigInt("2");
    
    BigInt test3 = BigInt(3);
    
    BigInt test4 = BigInt(4);
    
    
    //double ans = test3 / 2;
    //double ans2 = test2 / test;
    //double ans3 = test4 / test3;
    
    //cout << ans << endl;// << ans2 << endl << ans3 << endl;
    
    // BigInt numerator = BigInt("123453223455");
    
    // BigInt denominator = BigInt("-93487509867097809879");
    // BigInt denominator2 = BigInt("93487509867097809879");
    
    // cout << numerator / denominator << endl;
    // cout << numerator / denominator2 << endl;
    
    // cout << numerator.fDiv(denominator) << endl;
    // cout << numerator.fDiv(denominator2) << endl;
    
    
    
    
    /*vector< vector<BigInt> > mat1;
    vector< vector<BigInt> > mat2;
    mat1.resize(3);//amount of rows in mat1
    for(int i=0 ; i < mat1.size() ; i++){
        mat1.at(i).resize(3);//amount of cols in mat1
    }
    mat2.resize(3);//amount of rows in mat2
    for(int i=0 ; i < mat2.size() ; i++){
        mat2.at(i).resize(3); //amount of cols in mat2
    }
    mat1.at(0).at(0) = test;
    mat1.at(0).at(1) = test2;
    mat1.at(0).at(2) = test3;
    mat1.at(1).at(0) = test4;
    mat1.at(1).at(1) = test;
    mat1.at(1).at(2) = test2;
    mat1.at(2).at(0) = test3;
    mat1.at(2).at(1) = test4;
    mat1.at(2).at(2) = test4;
    
    mat2.at(0).at(0) = test;
    mat2.at(0).at(1) = test2;
    mat2.at(0).at(2) = test3;
    mat2.at(1).at(0) = test;
    mat2.at(1).at(1) = test;
    mat2.at(1).at(2) = test2;
    mat2.at(2).at(0) = test;
    mat2.at(2).at(1) = test4;
    mat2.at(2).at(2) = test4;
    vector< vector<BigInt> > matAns = matAdd(mat1,mat2);
    
    cout << matAns.at(0).at(0) <<" "<< matAns.at(0).at(1) <<" "<< matAns.at(0).at(2) << endl;
    cout << matAns.at(1).at(0) <<" "<< matAns.at(1).at(1) <<" "<< matAns.at(1).at(2) << endl;
    cout << matAns.at(2).at(0) <<" "<< matAns.at(2).at(1) <<" "<< matAns.at(2).at(2) << endl;*/
    
    
    //int limit = 30;
    /*BigInt first = BigInt(1);
    BigInt second = BigInt(limit);
    BigInt answer;
    for(int i=0; i<limit; i++){
            answer = first*second;
            first =  first + BigInt("1000000000000000000000000000000000");
            second = second - BigInt("1000000000000000000000000000000000");
    }
    cout << answer.value.size(); */
    
    /*
    vector< vector<BigInt> > mat1;
    vector< vector<BigInt> > mat2;
    mat1.resize(limit);
    for(int i=0 ; i < mat1.size() ; i++){
        mat1.at(i).resize(limit);
    }
    mat2.resize(limit);
    for(int i=0 ; i < mat2.size() ; i++){
        mat2.at(i).resize(limit);
    }
    for(int i=0 ; i < limit ; i++){
        for(int j=0 ; j < limit ; j++){
            mat1.at(i).at(j) = BigInt(10);
            mat2.at(i).at(j) = BigInt(10);
        }
    }
    for(int i=0 ; i < 500 ; i++){
        matMult(mat1,mat2);
    }
    */
    /*
    cout << "Please enter number a" << endl;
    BigInt a;
    cin >> a;
    cout << "number a is: " << a << endl;
    cout << "Please enter number b" << endl;
    BigInt b;
    cin >> b;
    cout << "number b is: " << b << endl;
    
    cout << "a+b = " << a+b << endl;
    cout << "a-b = " << a-b << endl;
    cout << "b-a = " << b-a << endl;
    cout << "a*b = " << a*b << endl;
    cout << "a/b = " << a/b << endl;
    cout << "b/a = " << b/a << endl;
    cout << "a floor div on b = " << a.fDiv(b) << endl;
    cout << "b floor div on a = " << b.fDiv(a) << endl;
    */
    
    BigInt tester = BigInt("10000000000000000000340000000234652346000000000435600000000000000000000");
    BigInt tester2 = BigInt("20000000000045367000000000000000001234532460000000000000000000000");
    BigInt answer = tester * tester2;
    
    cout << answer << endl;
    
    
    return 0;
}
