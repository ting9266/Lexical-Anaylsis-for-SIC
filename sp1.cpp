//*******************************************************************************************/
//   10827233 林庭 資工三乙  Lexical Analysis
//*******************************************************************************************/

#include <iostream>     // cout, endl
#include <fstream>      // open, is_open, close, ignore
#include <string>       // string, find_last_of, substr
#include <string.h>       
#include <vector>       // vector, push_back
#include <cstdlib>      // system, atoi
#include <cstring>      // strcpy
#include <iomanip>      // setw

using namespace std; 

typedef char STRING20 [20]; //短字串，最多19個字元
typedef char STRING200 [200]; //長字串，最多199個字元

//******************************************** struct **************************************************************//

#define MAX_TOKENS 30 //估計一行程式最多有幾個 tokens
struct Tokens//Tokens 是一個 struct，用來儲存 lexical_analyzer 所拆解出來的 tokens，
{
    unsigned amount; //紀錄這行 source 中有實際有幾個 token
    STRING20 tokens[MAX_TOKENS]; //實際紀錄 token 的地方
};

typedef struct sTable  // type of a student record
{      STRING20                  table;           
       int                       tnum;          
}      tableType;

struct tablenum { 
  int tablenumber;  
  int position;
};

//********************************************* class **************************************************************//

class Readfile
{
public:
	vector <struct tablenum> answer ; 
    int readfile( string fileName ) ;
    
    //下面這個函式，用來將註解刪除 它可以吃入一行原始碼，然後傳回刪除註解後的原始碼
    void comment_cutter(STRING200 &source_line);

    //下面這個函式，用來將一行無註解的原始碼切成 tokens 以 Tokens 的型態傳回
    Tokens lexical_analyzer(STRING200 source_line);
    
    string to_hiorlo(STRING20 target);
    string to_upper(STRING20 target);
    string to_lower(STRING20 target);
    void find_tablenumber(Tokens tokens);
    void put_in_newtable(Tokens token,int place );
    int which_table(Tokens token,int place);
    bool allnum1( STRING20 target );
    bool allnum2( STRING20 target );
    int find_hash(STRING20 target);  
};

class Table
{  
public:
	tableType **table1; 
	tableType **table2; 
	tableType **table3; 
	tableType **table4; 
	tableType **table5; 
	tableType **table6; 
	tableType **table7; 
	int tablenumber(string fileName);
	void createtable(string fileName);		
};

//下面這兩個類別，用來記錄有哪些 white space，以及有哪些 delimiter
class White_Space_Table
{
	vector<char> whitespacelist;  
public:
    void insert(char space);//加入一個 white space 入表中
    bool contain(char space);//查詢某一字元是否是 white space
};

class Delimiter_Table
{
	vector<char> delimiterlist;
public:
    void insert(char delimiter);//加入一 delimiter 入表中
    bool contain(char delimiter);//查詢某一字元是否是 delimiter
};

//*********************************************** 宣告全域  ************************************************************//

White_Space_Table space_table;
Delimiter_Table delimiter_table;
Table table;

//******************************************* Readfile function **************************************************************//
   
int Readfile::readfile( string fileName )                               // get records from a text file 
{       ifstream inFile;                                                // file handles
        ofstream outFile;
        Tokens tokens ;
        
        inFile.open(fileName.c_str() , ifstream::in );
        

        fileName = "SIC_output.txt"; 
        
        outFile.open(fileName.c_str(), ifstream::out); 
        if (outFile.is_open() ) {
             STRING200     rBuf;                       // buffer to keep one whole record
             cout << "\n\n********** Output  ************\n\n" ;
             while (inFile.getline(rBuf, 200, '\n')) {    // retrieve one entire record from file
               outFile << rBuf << endl ;
               cout << rBuf << endl ;
               comment_cutter(rBuf);
               tokens = lexical_analyzer(rBuf) ;
               find_tablenumber(tokens);
               for( int i = 0 ;  i < answer.size() ; i++ ){
               	outFile << "("<< answer[i].tablenumber << "," <<  answer[i].position << ")" ;
               	cout << "("<< answer[i].tablenumber << "," <<  answer[i].position << ")" ;
			   }
               outFile <<  endl ;
               cout << endl; 
               answer.clear() ;
             }      //end outer while
	    }  //end if
         outFile.close(); 
         inFile.close();// close the text file
         
         cout << "\n\n************** Check the file:SIC_output.txt  *********\n\n" ;
           
}  //end readfile

void  Readfile::comment_cutter(STRING200 &source_line){
  for( int i = 0 ; i < strlen( source_line ) ; i ++ ){
	if ( source_line[i] == '.' ){
	  for(int j = i+1 ; j < strlen( source_line ) ; j++ )
		  source_line[j] = '\0';
	}	
  }
} // comment_cutter

Tokens Readfile::lexical_analyzer(STRING200 source_line){
  Tokens token ;
  STRING20 temp = "\0" ;
  token.amount = 0 ;

  for( int i = 0 ; i < strlen( source_line ) ; i ++ ){
  	//取一字元出來 
  	
  	if( space_table.contain( source_line[i] ) ){   //此字元為white_space，若TEMP內有東西，則將其歸為一個TOKEN
  		if( strcmp(temp , "\0") != 0 ){ 
  		  strcpy( token.tokens[token.amount] ,(char*)&temp ) ; 
  	      token.amount++;
  	      strcpy( temp ,"\0" ) ;
		}
		//cout << endl;
	  }
  	else if( delimiter_table.contain( source_line[i] ) ){   //此字元為delimiter，若TEMP內有東西，則將其歸為一個TOKEN後，再將此字元歸為另一個TOKEN  
  		if( strcmp(temp , "\0") != 0 ){
  		  strcpy( token.tokens[token.amount] ,(char*)&temp ) ; 
  	      token.amount++;
  	      strcpy( temp ,"\0" ) ;
		}
		string now ;
		now.push_back(source_line[i]);
		strcpy( token.tokens[token.amount] ,now.c_str() ); 
	    token.amount++;
	}
	else{      //此字元為字母或數字，將其放入TEMP 
		string now ;
		now.push_back(source_line[i]);
		strcat( temp , now.c_str());
	}	
  }
 
  //將還未處理完的TEMP處理完畢 
  if( temp != "\0" ){
  	strcpy( token.tokens[token.amount] ,(char*)&temp ) ;
  	token.amount++;
  	strcpy( temp ,"\0" ) ;
  }
  /* 
  //結果測試 
  cout << token.amount << ":" ;
  for(int i = 0 ; i < token.amount ; i ++ )
    cout << token.tokens[i] <<"|" ; 
   cout << endl << endl ;
   */ 
   return token;
   
} //lexical_analyzer

string Readfile::to_hiorlo(STRING20 target){
	string result = "\0" ;
	for(int i = 0 ; i < strlen(target) ; i++) {
		if( target[i]>= 'A' && target[i] <= 'Z' )
		  result.push_back(target[i]+32);
		else if( target[i]>= 'a' && target[i] <= 'z' )
		  result.push_back(target[i]-32);	
	}
	return result ;
	
} 
string Readfile::to_upper(STRING20 target){
	string result = "\0" ;
	for(int i = 0 ; i < strlen(target) ; i++) {	
	    if( target[i]>= 'a' && target[i] <= 'z' )
		  result.push_back(target[i]-32);	
	}
	return result ;	
} 
string Readfile::to_lower(STRING20 target){
	string result = "\0" ;
	for(int i = 0 ; i < strlen(target) ; i++) {
		if( target[i]>= 'A' && target[i] <= 'Z' )
		  result.push_back(target[i]+32);	
	}
	return result ;	
} 

void Readfile::find_tablenumber(Tokens token){
  bool found = false ;
  tablenum current ;
    
  for(int i = 0 ; i < token.amount ; i ++ ){
  	if ( strcmp( token.tokens[i] , "C" )== 0 && strcmp( token.tokens[i+1], "\'" )== 0 ){
      put_in_newtable(token,i);	
      found = true ;
	}
	else if ( strcmp( token.tokens[i] , "X" )== 0 && strcmp( token.tokens[i+1], "\'" )== 0 ) {
	  put_in_newtable(token,i);
	  found = true ;	
	}
	else if( i > 0  && strcmp(token.tokens[i-1], "\'") == 0 && strcmp(token.tokens[i], "\'") != 0  )
	   put_in_newtable(token,i);
	else{

  	  for(int j = 1 ;  table.table1[j] != NULL && found == false ;j++ ){
  		  if( strcmp(token.tokens[i], table.table1[j]->table) == 0 || strcmp(token.tokens[i], to_hiorlo(table.table1[j]->table).c_str()) == 0  ){
  			found = true ;
  			current.tablenumber = 1 ;
  			current.position = table.table1[j]->tnum ;
  			answer.push_back(current) ;
		  }
   	  }
	  for(int j = 1 ; table.table2[j] != NULL && found == false ;j++ ){
  		  if( strcmp(token.tokens[i], table.table2[j]->table)== 0 || strcmp(token.tokens[i], to_hiorlo(table.table2[j]->table).c_str()) == 0 ){
  			found = true ;
  			current.tablenumber = 2 ;
  			current.position = table.table2[j]->tnum ;
  			answer.push_back(current) ;
		  }
	  }
	  for(int j = 1 ; table.table3[j] != NULL && found == false ;j++ ){
  		  if( strcmp(token.tokens[i], table.table3[j]->table)== 0 || strcmp(token.tokens[i], to_hiorlo(table.table3[j]->table).c_str()) == 0 ){
  			found = true ;
  			current.tablenumber = 3 ;
  			current.position = table.table3[j]->tnum ;
  			answer.push_back(current) ;
		  }
	  }
	  for(int j = 1 ; table.table4[j] != NULL && found == false ;j++ ){
  		  if( strcmp(token.tokens[i], table.table4[j]->table)== 0 ){
  			found = true ;
  			current.tablenumber = 4 ;
  			current.position = table.table4[j]->tnum ;
  			answer.push_back(current) ;
		  }
	  }
	  if( found == false)
	    put_in_newtable(token,i);
	
    } // end else
    found = false ;
  } // end for 
} // find_tablenumber

void Readfile::put_in_newtable(Tokens token,int place){
	int number = 0 ;
	tablenum current ;
	//判斷哪一類
	number = which_table(token,place) ;
	
	//放進TABLE並輸出
	int hvalue = find_hash(token.tokens[place]) ;
	tableType* temp = new tableType; //建立一新節點放入當筆資料 
    strcpy( temp->table,strupr(token.tokens[place]) );    
    
	if ( number == 5 ){
		while(table.table5[hvalue] != NULL ){
		  if ( strcmp(table.table5[hvalue]->table,temp->table ) == 0 )
		    break ;
          hvalue = (hvalue + 1)% 100;
        }
        temp->tnum = hvalue;
        table.table5[hvalue] = temp; 
		current.tablenumber = 5 ;
  		current.position = table.table5[hvalue]->tnum ;
  		answer.push_back(current) ;	
	} 
	else if ( number == 6 ){
		while(table.table6[hvalue] != NULL  ){
		  if ( strcmp(table.table6[hvalue]->table,temp->table ) == 0 )
		    break ; 
          hvalue = (hvalue + 1)% 100;
        }
        temp->tnum = hvalue;
        table.table6[hvalue] = temp;
		current.tablenumber = 6 ;
  		current.position = table.table6[hvalue]->tnum ;
  		answer.push_back(current) ;	  	
	}
	else if ( number == 7 ){
		while(table.table7[hvalue] != NULL ){
		  if ( strcmp(table.table7[hvalue]->table,temp->table ) == 0 )
		    break ;
          hvalue = (hvalue + 1)% 100;
        }
        temp->tnum = hvalue;
        table.table7[hvalue] = temp;
		current.tablenumber = 7 ;
  		current.position = table.table7[hvalue]->tnum ;
  		answer.push_back(current) ;		
	}
	
}  //put_in_newtable

int Readfile::which_table(Tokens token,int place){
	STRING20 target = "\0",front = "\0", back = "\0";
	
	strcpy( front ,token.tokens[place-1] );
	strcpy( target ,token.tokens[place] );
	strcpy( back ,token.tokens[place+1] );
	
    if ( strcmp( target , "C" )== 0 && strcmp( back, "\'" )== 0 ) 
	  return 0 ;
	else if ( strcmp( target , "X" )== 0 && strcmp( back, "\'" )== 0 ) 
	  return 0 ;
	else if( strcmp( front, "\'" )== 0 && strcmp( back, "\'" )== 0 ){		
	  strcpy( front ,token.tokens[place-2] );
	  if( strcmp( front, "C" )== 0 )
	    return 7 ; 
	  if( strcmp( front, "X" )== 0 && allnum1(target) )
	    return 6 ;
	  else 
	    return 5 ; 
	}
	else if ( allnum2(target) )
	  return 6;  
	else if ( strcmp( target , "\0" )!= 0) 
	  return 5 ; 
	
}  //which_table

bool Readfile::allnum1( STRING20 target ) {  
    for (int i = 0; target[i] != '\0' ; i++){
      if( !isdigit( target[i] ) &&  target[i] < 'A'  && target[i] > 'F' )
	    return false;   
    } 
    if(strcmp( target , "\0" )== 0)
      return false ;
    return true;
    
} // allisNum1()

bool Readfile::allnum2( STRING20 target ) {  
    for (int i = 0; target[i] != '\0' ; i++){
      if( !isdigit( target[i] )  )
	    return false;   
    } 
    if(strcmp( target , "\0" )== 0)
      return false ;
    return true;
    
} // allisNum2()

int Readfile::find_hash(STRING20 target){
    int sum = 0 ; 
	for(int i = 0 ; target[i] != '\0' ; i++){
	  sum = sum + (target[i]%100) ;
	  if(sum > 100)
	    sum = sum %100;
	}  
	sum = sum %100;
	return sum ; 
	
} // end find_hash



//******************************************** Table function **************************************************************//

int Table::tablenumber(string fileName){
  for(int i = 0 ; i < sizeof(fileName) ; i++ ){
  	if(isdigit(fileName[i]) ){
  		return fileName[i]-'0';
	  }
  }
  return 0 ;
  
} // tablenumber


void Table::createtable( string fileName )                              // get records from a text file 
{       ifstream inFile;                                                // file handles
        int num = 0,number = 0 ;
        
        number = tablenumber(fileName);
        inFile.open(fileName.c_str() , ifstream::in );
        
        STRING200     rBuf;                     // buffer to keep one whole record
        tableType oneT; // keep one record with the required fields
        int i=1 ;
             
        inFile.seekg(0, inFile.end); //指針定位在文件結束處
        num = inFile.tellg() / sizeof( 2); // 所有/一個學生所需 = 共有幾個學生
        inFile.seekg(0, inFile.beg);  //指針定位在文件開始處
            
             
        num = num *2 ;
        if ( number == 1  ) {
            
             table1 = new tableType* [num]; // build empty table
             for (int j = 0; j < num ; j++){
                table1[j] = NULL;
             }
             
             while (inFile.getline(rBuf, 200, '\n')) {    // retrieve one entire record from file
                 tableType* temp = new tableType; //建立一新節點放入當筆資料 
                 strcpy( temp->table,strupr(rBuf ));
                 temp->tnum = i ;
                 
                 table1[i]=temp;
                 
                 i++;
              }      //end inner while  
                 
	    }  //end if
	    else if ( number == 2  ) {
    
             table2 = new tableType* [num]; // build empty table
             for (int j = 0; j < num ; j++){
                table2[j] = NULL;
             }
             
             while (inFile.getline(rBuf, 200, '\n')) {    // retrieve one entire record from file
                 tableType* temp = new tableType; //建立一新節點放入當筆資料 
                 strcpy( temp->table,rBuf );
                 temp->tnum = i ;
                 
                 table2[i]=temp;
                
                 i++;
              }      //end inner while 
	    }  //end if
	    else if ( number == 3  ) {
            
             table3 = new tableType* [num]; // build empty table
             for (int j = 0; j < num ; j++){
                table3[j] = NULL;
             }
             
             while (inFile.getline(rBuf, 200, '\n')) {    // retrieve one entire record from file
                 tableType* temp = new tableType; //建立一新節點放入當筆資料 
                 strcpy( temp->table,rBuf );
                 temp->tnum = i ;
                 
                 table3[i]=temp;
                 
                 i++;
              }      //end inner while           
	    }  //end if
	    else if ( number == 4 ) {
             
             
             table4 = new tableType* [num]; // build empty table
             for (int j = 0; j < num ; j++){
                table4[j] = NULL;
             }
   
             while (inFile.getline(rBuf, 200, '\n')) {    // retrieve one entire record from file
                 tableType* temp = new tableType; //建立一新節點放入當筆資料 
                 strcpy( temp->table,rBuf );
                 temp->tnum = i ;
                 
                 table4[i]=temp;
                 
                 i++;
              }      //end inner while  //end outer while       
	    }  //end if
         
         inFile.close();// close the text file
         
         table5 = new tableType* [100]; 
         for (int j = 0; j < 100 ; j++){
                table5[j] = NULL;
         }
         table6 = new tableType* [100];
		 for (int j = 0; j < 100 ; j++){
                table6[j] = NULL;
         } 
         table7 = new tableType* [100];
		 for (int j = 0; j < 100 ; j++){
                table7[j] = NULL;
         } 
           
}  //end createtable

//******************************************* White_Space_Table function ****************************************************//

void White_Space_Table::insert(char space){
	whitespacelist.push_back(space);
}
bool White_Space_Table::contain(char space){
	if ( whitespacelist.empty())
	  return 0 ;
	for(int i = 0 ; i < whitespacelist.size() ; i++ ){
		if(whitespacelist[i] == space)
		  return true ;
	}
	return 0;
}

//******************************************* Delimiter_Table function **************************************************//

void Delimiter_Table::insert(char delimiter){
	delimiterlist.push_back(delimiter);
}
bool Delimiter_Table::contain(char delimiter){
	if ( delimiterlist.empty() )
	  return 0 ;
	for(int i = 0 ; i < delimiterlist.size() ; i++ ){
		if( delimiterlist[i] == delimiter)
		  return true ;
	}
	return 0;
}

//*********************************************** Main **********************************************************//

int main( ) {
  string fileName;
  Readfile read;
  
  space_table.insert(' ');
  space_table.insert('\t');
  space_table.insert('\n');
   
  delimiter_table.insert(';');
  delimiter_table.insert(',');
  delimiter_table.insert('['); 
  delimiter_table.insert(']'); 
  delimiter_table.insert(':');
  delimiter_table.insert('\'');
  delimiter_table.insert('+');
  delimiter_table.insert('-');
  delimiter_table.insert('*');
  delimiter_table.insert('/');
  delimiter_table.insert('?');
  delimiter_table.insert('.');
  delimiter_table.insert('=');
  delimiter_table.insert('#');
  delimiter_table.insert('@');
  
  
  table.createtable("Table1.table");
  table.createtable("Table2.table");
  table.createtable("Table3.table");
  table.createtable("Table4.table");
  
  cout << "****** Lexical Analysis *******";
  cout << endl << "\nInput a file name: ";
 
  cin >> fileName;
  read.readfile(fileName);
  
  return 0 ;
}

