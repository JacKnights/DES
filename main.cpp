#include <iostream>
#include "DES.cpp"

using namespace std;

int main() {
	string Text_Str = "0101000101010111010001010101001001010100010110010101010101001001";
	bitset<64> text(Text_Str);
	cout << "Text: " << text << endl;
	string Key_Str = "0101101001011000010000110101011001000010010011100100110101001100";
	bitset<64> key(Key_Str);
	cout << " Key: " << key << endl;
	DES des(key);
	bitset<64> C = des.Encrypt(text);
	cout << "Cipher_Text: " << C << endl;
	bitset<64> M = des.Decrypt(C);
	cout << "Plain_Text: " << M << endl;
	cout << "Original Text = Decoded Text? " << ((text == M)?"Yes.": "No.") << endl;
}
