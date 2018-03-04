#include <iostream>
#include <bitset>
#include "DES.h"

using namespace std;

DES::DES(bitset<64> Key) {
	Key_64 = Key;
}

bitset<64> DES::Encrypt(bitset<64> Text) {
	bitset<64> M0 = IP_Substitute(Text);
	bitset<48> * Key_Sub = new bitset<48>[16];
	Key_Gen(Key_64, Key_Sub);
	bitset<64> RL = T_Iteration(M0, Key_Sub);
	bitset<64> C = IP_Inv_Substitute(RL);
	return C;
}

bitset<64> DES::Decrypt(bitset<64> Text) {
	bitset<64> C0 = IP_Substitute(Text);
	bitset<48> * Key_Sub = new bitset<48>[16];
	Key_Gen(Key_64, Key_Sub);
	Key_Inv(Key_Sub);
	bitset<64> RL = T_Iteration(C0, Key_Sub);
	bitset<64> M = IP_Inv_Substitute(RL);
	return M;
}
 
void DES::Key_Gen(bitset<64> Key_64, bitset<48>* &Key_Sub) {
	bitset<56> Key_56 = Key_64_To_56(Key_64);
	bitset<28> C, D;
	for (int i = 0; i < 28; i++) {
		C[i] = Key_56[i];
		D[i] = Key_56[i + 28];
	}
	for (int i = 0; i < 16; i++) {
		if (i == 0 || i == 1 || i == 8 || i == 15) {
			C = Key_28_Shift_Left(C, 1);
			D = Key_28_Shift_Left(D, 1);
		} else {
			C = Key_28_Shift_Left(C, 2);
			D = Key_28_Shift_Left(D, 2);
		}
		for (int j = 0; j < 28; j++) {
			Key_56[j] = C[j];
			Key_56[j + 28] = D[j];
		}
		Key_Sub[i] = Key_56_To_48(Key_56);
		cout << "Key_Sub: " << Key_Sub[i] << endl;
	}
}

void DES::Key_Inv(bitset<48>* &Key_Sub) {
	for (int i = 0; i < 8; i++) {
		bitset<48> tmp = Key_Sub[i];
		Key_Sub[i] = Key_Sub[15 - i];
		Key_Sub[15 - i] = tmp;
	}
} 

bitset<64> DES::IP_Substitute(bitset<64> M) {
	cout << "IP_Substitute  M: " << M << endl;
	bitset<64> M0;
	for (int i = 0; i < 64; i++) {
		M0[i] = M[IP[i] - 1];
	}
	cout << "IP_Substitute M0: " << M0 << endl;
	return M0;
}

bitset<32> DES::Feistel(bitset<32> R, bitset<48> ki) {
	bitset<48> E_R = E_Expand(R);
	bitset<48> result = E_R ^ ki;
	
	bitset<32> F_S;
	for (int i = 0; i < 8; i++) {
		int row = result[6 * i] * 2 + result[6 * i + 5];
		int col = result[6 * i + 1] * 8 + result[6 * i + 2] * 4 + result[6 * i + 3] * 2 + result[6 * i + 4];
		bitset<4> tmp(S_BOX[i][row][col]);
		for (int k = 0; k < 4; k++) {
			F_S[i * 4 + k] = tmp[k];
		}
	}
	bitset<32> F_P = P_Substitute(F_S);
	return F_P;
}

bitset<48> DES::E_Expand(bitset<32> R) {
	bitset<48> E_R;
	for (int i = 0; i < 48; i++) {
		E_R[i] = R[E[i] - 1];
	}
	return E_R;
}

bitset<32> DES::P_Substitute(bitset<32> F_S) {
	bitset<32> F_P;
	for(int i = 0; i < 32; i++) {
        F_P[i] = F_S[P[i] - 1];
    }
    return F_P;
}

bitset<64> DES::T_Iteration(bitset<64> M0, bitset<48>* &Key_Sub) {
	bitset<32> L, R;
	for (int i = 0; i < 32; i++) {
		L[i] = M0[i];
		R[i] = M0[i + 32];
	}
	for (int i = 0; i < 16; i++) {
		bitset<32> L_Next(R);
		R = L ^ Feistel(R, Key_Sub[i]);
		L = L_Next;
		cout << "L: " << L << " R: " << R << endl;
	}
	bitset<64> RL;
	for (int i = 0; i < 32; i++) {
		RL[i] = R[i];
		RL[i + 32] = L[i];
	}
	cout << "T_Iteration RL: " << RL << endl;
	return RL;
}

bitset<64> DES::IP_Inv_Substitute(bitset<64> RL) {
	cout << "IP_Inv_Substitute RL: " << RL << endl;
	bitset<64> C;
	for (int i = 0; i < 64; i++) {
		C[i] = RL[IP_Inv[i] - 1];
	}
	cout << "IP_Inv_Substitute  C: " << C << endl;
	return C;
}

bitset<56> DES::Key_64_To_56(bitset<64> Key_64) {
	bitset<56> Key_56;
	for (int i = 0; i < 56; i++) {
		Key_56[i] = Key_64[PC_1[i] - 1];
	}
	cout << "Key_64_To_56  Key_56: " << Key_56 << endl;
	return Key_56;
}

bitset<28> DES::Key_28_Shift_Left(bitset<28> Key_56_Half, int num) {
	bitset<28> Key_28;
	Key_28 = (Key_56_Half << num) | (Key_56_Half >> (28 - num));
	//cout << "Key_28_Shift_Left  Key_28: " << Key_28 << endl;
	return Key_28;
}

bitset<48> DES::Key_56_To_48(bitset<56> Key_56) {
	bitset<48> Key_48;
	for (int i = 0; i < 48; i++) {
		Key_48[i] = Key_56[PC_2[i] - 1];
	}
	//cout << "Key_56_To_48  Key_48: " << Key_48 << endl;
	return Key_48;
}

