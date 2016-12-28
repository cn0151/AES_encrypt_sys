/***
	Author: Nwankwo Chukwuebuka Justin
	Program: AES encryption system
***/


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <bitset>

using namespace std;
const int BLOCK_LEN = 16;

void display_ver_one(string, string &, ofstream &);
void display_ver_two(string, string &, ofstream &);



void preprocess(string &);
void substitution(string &, const string &);
void padding(string &);
void shiftrows(string &);
void paritybitcheck(string &);

unsigned int gethex(char);
string tohex(char);
int checkMSB(char);
char multbytwo(char);
char multbythree(char);
char multcolumn(char, int);
char normalizecell(char, char);
string hexvalue(char);
void gmix_column(string & );
int main(){
	ifstream infile; //plain text file handler
	ifstream keyfile; //key file handler
	ofstream outfile; //output file handler

	string filename; // holds filenames
	string key;     // holds the encryption key

	cout<<"Enter the name of the input plaintext file: ";
	cin >> filename;


	infile.open(filename.c_str());
	if(!infile){
		cout<<"Could not open file";
		exit(EXIT_FAILURE);
	}



	cout<<"Enter the name of the input key file: ";
	cin >> filename;

	keyfile.open(filename.c_str());
	if(!keyfile){
		cout<<"Could not open file";
		exit(EXIT_FAILURE);
	}


	cout<<"Enter the name of the output ciphertext file: ";
	cin >> filename;

	outfile.open(filename.c_str());
	if(!outfile){
		cout<<"Could not open file";
		exit(EXIT_FAILURE);
	}

	getline(keyfile, key);
	keyfile.close();


	string text = "";  //holds the plaintext data
	string temp;

	while(getline(infile, temp))
		text += temp;


	infile.close();
	
	preprocess(text);
	outfile << "Preprocessing: " << endl;
	outfile << text << endl << endl;


	
	substitution(text, key);
	outfile << "Substitution: " <<endl;
	outfile << text << endl <<endl;


	padding(text);
	display_ver_one("Padding: ", text, outfile);


	shiftrows(text);
	display_ver_one("Shiftrows: ",text, outfile);	

	paritybitcheck(text);
	display_ver_two("Parity bit: ", text, outfile);


	gmix_column(text);
	display_ver_two("MixColumns: ", text, outfile);
}


void display_ver_one(string operation, string & data, ofstream & output){
	output << operation << endl;
        for(int i = 0; i < data.size(); i++ ){
		if(i != 0 && i % 4 == 0)  //used to seperate 4 characters
                        output << "\n";
                if(i != 0 && i % 16 == 0)  //used to seperate 16 characters
                        output << "\n";

                output << data[i];
        }

	output << endl << endl;


}

void display_ver_two(string operation, string & data, ofstream& output){
	output << operation << endl;
	for(int i = 0; i < data.size(); i++){
		if(i != 0 && i % 8 == 0)
			output << "\n";

		if(i != 0 && i % 2 != 0)
			output << data[i] << " ";
		else
			output << data[i];
	}

	output << endl << endl;
}

// This function removes punctuation and whitespace charactesrs
// from the input.

void preprocess(string & data){
	string temp = "";
	for(int i = 0;  i < data.size(); i++){
		int count = 0;
		if(isalpha(data[i]))
			temp += data[i];
		
	}

	data = temp;	

}

//This functions performs a polyalphabetic substitution using the
//Vigenere cipher on the input file with the key
void substitution(string & data, const string & key){
	int k = 0;
	int ascii;

	for(int i = 0; i < data.size(); i++){
		if(k > key.size() - 1)
			k = 0;
		
		ascii = (data[i] - 'A') + (key[k] - 'A');
		if(ascii > 25)
			ascii = ascii % 26;
	
	k += 1;
	data[i] = (ascii + 'A');

	}


}

//Here the data is padded with the character A so that 
//the length of the parameter is divisible by 16
void padding(string & data){
	if(data.size() % BLOCK_LEN == 0)
		return;

	int len = BLOCK_LEN - (data.size() % BLOCK_LEN);
	string temp(len, 'A');
	data += temp;
	


}

//This function circularly shifts the rows in each block. The
//first row remains the same, the second row is shifted by one position 
//to the left. The third row is shifted two positions to the left and 
//finally, the fourth row is shifted three positions to the left.

void shiftrows(string & data){
	int k = 0;
	char temp0, temp1, temp2, temp3;
	
	for(int i = 0; i < data.size(); i += 4){
		if(k > 3)
			k = 0;
		
		temp0 = data[i];
		temp1 = data[i + 1];
		temp2 = data[i + 2];
		temp3 = data[i + 3];


		switch(k){   // shifting takes place in the switch statement
			case 0:
				break;

			case 1:
				data[i] = temp1;
				data[i + 1] = temp2;
				data[i + 2] = temp3;
				data[i + 3] = temp0;
				break;

			case 2:
				data[i] = temp2;
				data[i + 1] = temp3;
				data[i + 2] = temp0;
				data[i + 3] = temp1;
				break;

			case 3:
				data[i] = temp3;
				data[i + 1] = temp0;
				data[i + 2] = temp1;
				data[i + 3] = temp2;

		}

		k += 1;
	}
}

char gethexchar(int num){
	if(num == 10)
		return 'a';
	else if (num == 11)
		return 'b';
	else if (num == 12)
		return 'c';
	else if (num == 13)
		return 'd';
	else if (num == 14)
		return 'e';
	else if (num == 15)
		return 'f';
	else 
		return 'z';
}



//Convert each character to ite numeric value and then use the
//most significant bit as a parity bit (even parity).
void paritybitcheck(string & data){
	
	string result = "";
	char temp[2];
	for(int i = 0; i < data.size(); i++){
		unsigned int ch = data[i];
		int count = 0;
		int even = 0;
		int val = 0;
		int mask = 0x1;
		for(int i = 0; i < 8; i ++){
			val = ch & mask;
			if(val >= 1){
				count += 1;
				val = 0;
			}

			mask <<= 1;
			
		}


		if(count % 2 != 0){
			ch = ch | 0x80;
		}

		int f_char = (ch >> 4) & 0xf;
		if(f_char > 9){
			f_char = gethexchar(f_char);
			result += char(f_char);
		}

		else{
			sprintf(temp, "%d", f_char);
			result += temp;
		}

		int s_char = ch & 0xf;
		if(s_char > 9){
			s_char = gethexchar(s_char);
			result += char(s_char);
		}

		else{
			sprintf(temp, "%d", s_char);
			result += temp;
		}

	}
	
	data = result;
}

int checkMSB(char n){
	int ans = (n >> 7) & 0x01;
	if (ans == 1)
		return 1;
	else 
		return 0; 
}


char multbytwo(char ch){
	int msb_dropped = checkMSB(ch);
	ch <<= 1;	//multiplies by 2
	ch &= 0xfe;
	if(msb_dropped)
		ch ^= 0x1B;
	return ch; 
}

char multbythree(char  ch){
	char temp = ch;
	int msb_dropped = checkMSB(ch);
	ch <<= 1;  //multiplies by 2
	ch &= 0xfe;
	
	ch ^= temp;  //adds one to the result of the multiplication

	if(msb_dropped)
		ch ^= 0x1B;
	return ch;
}

char  multcolumn(char  ch, int shift){
	if(shift== 2)
		return multbytwo(ch);

	else
		return multbythree(ch);
		
}




unsigned int  gethex(char ch){
	if (ch >= '0' && ch <= '9')
		return ch - '0';

	else if (ch == 'a')
		return 10;

	else if (ch == 'b')
		return 11;

	else if (ch == 'c')
		return 12;

	else if (ch == 'd')
		return 13;

	else if (ch == 'e')
		return 14;

	else
		return 15;

}



char normalizecell(char a, char b){
	char result = gethex(a);

	result <<= 4;
	result &= 0xf0;

	result |= gethex(b);
	return result;
	
}


string hexval(char ch){
	string result = "";
	char buf[2];
	int temp = (ch >> 4) & 0x0f;
	if(temp <= 9){
		sprintf(buf, "%d", temp);
                result += buf;
	}
	else
		result += gethexchar(temp);

	temp = ch << 4;
	temp >>= 4;
	temp &= 0x0f;

	if(temp <= 9){
		sprintf(buf, "%d", temp);
		result += buf;
	}
        else
                result += gethexchar(temp);

	return result;

}

void gmix_column(string & data){

	int block_size = 32;
	int block_end = 0;
	char a0, a1, a2, a3;
	char b0, b1, b2, b3;
	char c0, c1, c2, c3;
	char d0, d1, d2, d3;

	char ch;
	string result = "";
	
	for(int i = 0; i < data.size(); i+= block_size){
		block_end += 32;
		string block(data.begin()+i, data.begin()+ block_end);

	
	
		
		//column 1
		char temp1A = normalizecell(block[0], block[1]);
		char temp1B = normalizecell(block[8],  block[9]);
		char temp1C = normalizecell(block[16], block[17]);
		char temp1D = normalizecell(block[24], block[25]);
		

		//column 2
		char temp2A = normalizecell(block[2],  block[3]);
                char temp2B = normalizecell(block[10], block[11]);
                char temp2C = normalizecell(block[18], block[19]);
                char temp2D = normalizecell(block[26], block[27]);


		//cloumn 3
		char temp3A = normalizecell(block[4],  block[5]);
                char temp3B = normalizecell(block[12], block[13]);
                char temp3C = normalizecell(block[20], block[21]);
                char temp3D = normalizecell(block[28], block[29]);

	
		//column 4
		char temp4A = normalizecell(block[6],  block[7]);
		char temp4B = normalizecell(block[14], block[15]);
                char temp4C = normalizecell(block[22], block[23]);
                char temp4D = normalizecell(block[30], block[31]);

		
	

		a0 = multcolumn(temp1A,2) ^ multcolumn(temp1B,3) ^ temp1C ^ temp1D;	
		a1 = temp1A ^ multcolumn(temp1B,2) ^ multcolumn(temp1C,3) ^ temp1D;
		a2 = temp1A ^ temp1B ^ multcolumn(temp1C,2) ^ multcolumn(temp1D,3);
		a3 = multcolumn(temp1A,3) ^ temp1B ^ temp1C ^ multcolumn(temp1D,2);



		b0 = multcolumn(temp2A,2) ^ multcolumn(temp2B,3) ^ temp2C ^ temp2D;
                b1 = temp2A ^ multcolumn(temp2B,2) ^ multcolumn(temp2C,3) ^ temp2D;
                b2 = temp2A ^ temp2B ^ multcolumn(temp2C,2) ^ multcolumn(temp2D,3);
                b3 = multcolumn(temp2A,3) ^ temp2B ^ temp2C ^ multcolumn(temp2D,2);


               
		c0 = multcolumn(temp3A,2) ^ multcolumn(temp3B,3) ^ temp3C ^ temp3D;
                c1 = temp3A ^ multcolumn(temp3B,2) ^ multcolumn(temp3C,3) ^ temp3D;
                c2 = temp3A ^ temp3B ^ multcolumn(temp3C,2) ^ multcolumn(temp3D,3);
                c3 = multcolumn(temp3A,3) ^ temp3B ^ temp3C ^ multcolumn(temp3D,2);


              

                d0 = multcolumn(temp4A,2) ^ multcolumn(temp4B,3) ^ temp4C ^ temp4D;
                d1 = temp4A ^ multcolumn(temp4B,2) ^ multcolumn(temp4C,3) ^ temp4D;
                d2 = temp4A ^ temp4B ^ multcolumn(temp4C,2) ^ multcolumn(temp4D,3);
                d3 = multcolumn(temp4A,3) ^ temp4B ^ temp4C ^ multcolumn(temp4D,2);


		

		result += (hexval(a0) + hexval(b0) + hexval(c0) + hexval(d0));
		result += (hexval(a1) + hexval(b1) + hexval(c1) + hexval(d1));
		result += (hexval(a2) + hexval(b2) + hexval(c2) + hexval(d2));
		result += (hexval(a3) + hexval(b3) + hexval(c3) + hexval(d3));	

	}
	data = result;

} 
